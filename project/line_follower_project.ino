const int LEFT_PWM = 5, LEFT_IN1 = 7, LEFT_IN2 = 8;
const int RIGHT_PWM = 6, RIGHT_IN1 = 9, RIGHT_IN2 = 10;
const int sensorPins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};

const int threshold = 500;
int baseLeft = 130;
int baseRight = 130;
float Kp = 7.0;
int crossForwardMs = 180;

void setMotorRaw(int pwm, int in1, int in2, int speed) {
  int s = constrain(speed, -255, 255);
  if (s >= 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(pwm, s);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(pwm, -s);
  }
}

void setMotorSpeeds(int left, int right) {
  setMotorRaw(LEFT_PWM, LEFT_IN1, LEFT_IN2, left);
  setMotorRaw(RIGHT_PWM, RIGHT_IN1, RIGHT_IN2, right);
}

bool sensor(int n) {
  return analogRead(sensorPins[n - 1]) < threshold;
}

int readLineError() {
  const int w[8] = {-7, -5, -3, -1, 1, 3, 5, 7};
  long sumW = 0;
  long active = 0;

  for (int i = 0; i < 8; i++) {
    bool onLine = analogRead(sensorPins[i]) < threshold;
    if (onLine) {
      sumW += w[i];
      active++;
    }
  }

  if (active == 0) return 0;
  return (int)(sumW / active);
}

void printControlDebug(int err, int corr, int l, int r, const char* action) {
  Serial.print("ERR: "); Serial.print(err);
  Serial.print(" | CORR: "); Serial.print(corr);
  Serial.print(" | L: "); Serial.print(l);
  Serial.print(" | R: "); Serial.print(r);
  Serial.print(" | ACTION: "); Serial.println(action);
}

void printCrossDebug(bool s3, bool s4, bool s5, bool s6, bool cross, const char* action) {
  Serial.print("S3-6: ");
  Serial.print(s3); Serial.print(" ");
  Serial.print(s4); Serial.print(" ");
  Serial.print(s5); Serial.print(" ");
  Serial.print(s6);
  Serial.print(" | CROSS: "); Serial.print(cross);
  Serial.print(" | ACTION: "); Serial.println(action);
}

void followLineWithP() {
  int err = readLineError();
  int corr = (int)(Kp * err);
  int left = constrain(baseLeft - corr, 0, 255);
  int right = constrain(baseRight + corr, 0, 255);

  const char* action = "FORWARD";
  if (err < 0) action = "SHIFT_LEFT";
  if (err > 0) action = "SHIFT_RIGHT";

  setMotorSpeeds(left, right);
  printControlDebug(err, corr, left, right, action);
}

void setup() {
  pinMode(LEFT_IN1, OUTPUT); pinMode(LEFT_IN2, OUTPUT); pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT); pinMode(RIGHT_IN2, OUTPUT); pinMode(RIGHT_PWM, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  bool s3 = sensor(3), s4 = sensor(4), s5 = sensor(5), s6 = sensor(6);
  bool cross = s3 && s4 && s5 && s6;

  if (cross) {
    printCrossDebug(s3, s4, s5, s6, true, "GO_STRAIGHT_THROUGH_CROSS");
    setMotorSpeeds(baseLeft, baseRight);
    delay(crossForwardMs);
  } else {
    printCrossDebug(s3, s4, s5, s6, false, "FOLLOW_LINE");
    followLineWithP();
  }

  delay(20);
}
