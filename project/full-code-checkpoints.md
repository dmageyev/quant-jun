# Повні кодові чекпоїнти (один і той самий файл проєкту)

Нижче — повні версії коду для послідовного розвитку `line_follower_project.ino`.

---

## Checkpoint 01 — Старт, рух, стоп

```cpp
const int LEFT_PWM = 5, LEFT_IN1 = 7, LEFT_IN2 = 8;
const int RIGHT_PWM = 6, RIGHT_IN1 = 9, RIGHT_IN2 = 10;

void setMotorRaw(int pwm, int in1, int in2, int speed) {
  int s = constrain(speed, -255, 255);
  if (s >= 0) { digitalWrite(in1, HIGH); digitalWrite(in2, LOW); analogWrite(pwm, s); }
  else { digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(pwm, -s); }
}

void setMotorSpeeds(int left, int right) {
  setMotorRaw(LEFT_PWM, LEFT_IN1, LEFT_IN2, left);
  setMotorRaw(RIGHT_PWM, RIGHT_IN1, RIGHT_IN2, right);
}

void printMotionDebug(const char* phase, int l, int r) {
  Serial.print("PHASE: "); Serial.print(phase);
  Serial.print(" | L: "); Serial.print(l);
  Serial.print(" | R: "); Serial.println(r);
}

void setup() {
  pinMode(LEFT_IN1, OUTPUT); pinMode(LEFT_IN2, OUTPUT); pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT); pinMode(RIGHT_IN2, OUTPUT); pinMode(RIGHT_PWM, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  printMotionDebug("START", 0, 0); delay(500);
  setMotorSpeeds(130, 130); printMotionDebug("FORWARD_2S", 130, 130); delay(2000);
  setMotorSpeeds(0, 0); printMotionDebug("STOP", 0, 0); delay(1000);
}
```

---

## Checkpoint 02 — Сенсори + базове слідування

```cpp
const int LEFT_PWM = 5, LEFT_IN1 = 7, LEFT_IN2 = 8;
const int RIGHT_PWM = 6, RIGHT_IN1 = 9, RIGHT_IN2 = 10;
const int sensorPins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int threshold = 500;

void setMotorRaw(int pwm, int in1, int in2, int speed) {
  int s = constrain(speed, -255, 255);
  if (s >= 0) { digitalWrite(in1, HIGH); digitalWrite(in2, LOW); analogWrite(pwm, s); }
  else { digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(pwm, -s); }
}
void setMotorSpeeds(int left, int right) {
  setMotorRaw(LEFT_PWM, LEFT_IN1, LEFT_IN2, left);
  setMotorRaw(RIGHT_PWM, RIGHT_IN1, RIGHT_IN2, right);
}
bool sensor(int n) { return analogRead(sensorPins[n - 1]) < threshold; }

void printLineDebug(bool s1, bool s2, bool s3, bool s4, bool s5, bool s6, bool s7, bool s8, const char* action) {
  Serial.print("SENSORS: ");
  Serial.print(s1); Serial.print(" "); Serial.print(s2); Serial.print(" ");
  Serial.print(s3); Serial.print(" "); Serial.print(s4); Serial.print(" ");
  Serial.print(s5); Serial.print(" "); Serial.print(s6); Serial.print(" ");
  Serial.print(s7); Serial.print(" "); Serial.print(s8);
  Serial.print(" | ACTION: "); Serial.println(action);
}

void setup() {
  pinMode(LEFT_IN1, OUTPUT); pinMode(LEFT_IN2, OUTPUT); pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT); pinMode(RIGHT_IN2, OUTPUT); pinMode(RIGHT_PWM, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  bool s1 = sensor(1), s2 = sensor(2), s3 = sensor(3), s4 = sensor(4);
  bool s5 = sensor(5), s6 = sensor(6), s7 = sensor(7), s8 = sensor(8);
  const char* action = "STOP";

  if (s4 || s5) { setMotorSpeeds(135, 135); action = "FORWARD"; }
  else if (s1 || s2 || s3) { setMotorSpeeds(95, 145); action = "TURN_LEFT"; }
  else if (s6 || s7 || s8) { setMotorSpeeds(145, 95); action = "TURN_RIGHT"; }
  else { setMotorSpeeds(0, 0); }

  printLineDebug(s1, s2, s3, s4, s5, s6, s7, s8, action);
  delay(50);
}
```

---

## Checkpoint 03 — P-контролер + калібрування

```cpp
const int LEFT_PWM = 5, LEFT_IN1 = 7, LEFT_IN2 = 8;
const int RIGHT_PWM = 6, RIGHT_IN1 = 9, RIGHT_IN2 = 10;
const int sensorPins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int threshold = 500;
int baseLeft = 135, baseRight = 135;
float Kp = 8.0;

void setMotorRaw(int pwm, int in1, int in2, int speed) {
  int s = constrain(speed, -255, 255);
  if (s >= 0) { digitalWrite(in1, HIGH); digitalWrite(in2, LOW); analogWrite(pwm, s); }
  else { digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(pwm, -s); }
}
void setMotorSpeeds(int left, int right) {
  setMotorRaw(LEFT_PWM, LEFT_IN1, LEFT_IN2, left);
  setMotorRaw(RIGHT_PWM, RIGHT_IN1, RIGHT_IN2, right);
}
bool sensor(int n) { return analogRead(sensorPins[n - 1]) < threshold; }

int readLineError() {
  const int w[8] = {-7, -5, -3, -1, 1, 3, 5, 7};
  long sumW = 0, active = 0;
  for (int i = 0; i < 8; i++) {
    bool onLine = analogRead(sensorPins[i]) < threshold;
    if (onLine) { sumW += w[i]; active++; }
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

void setup() {
  pinMode(LEFT_IN1, OUTPUT); pinMode(LEFT_IN2, OUTPUT); pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT); pinMode(RIGHT_IN2, OUTPUT); pinMode(RIGHT_PWM, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int err = readLineError();
  int corr = (int)(Kp * err);
  int left = constrain(baseLeft - corr, 0, 255);
  int right = constrain(baseRight + corr, 0, 255);

  const char* action = "FORWARD";
  if (err < 0) action = "SHIFT_LEFT";
  else if (err > 0) action = "SHIFT_RIGHT";

  setMotorSpeeds(left, right);
  printControlDebug(err, corr, left, right, action);
  delay(30);
}
```

---

## Checkpoint 04 — Повний заїзд з перехрестями

```cpp
const int LEFT_PWM = 5, LEFT_IN1 = 7, LEFT_IN2 = 8;
const int RIGHT_PWM = 6, RIGHT_IN1 = 9, RIGHT_IN2 = 10;
const int sensorPins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int threshold = 500;
int baseLeft = 130, baseRight = 130;
float Kp = 7.0;

void setMotorRaw(int pwm, int in1, int in2, int speed) {
  int s = constrain(speed, -255, 255);
  if (s >= 0) { digitalWrite(in1, HIGH); digitalWrite(in2, LOW); analogWrite(pwm, s); }
  else { digitalWrite(in1, LOW); digitalWrite(in2, HIGH); analogWrite(pwm, -s); }
}
void setMotorSpeeds(int left, int right) {
  setMotorRaw(LEFT_PWM, LEFT_IN1, LEFT_IN2, left);
  setMotorRaw(RIGHT_PWM, RIGHT_IN1, RIGHT_IN2, right);
}
bool sensor(int n) { return analogRead(sensorPins[n - 1]) < threshold; }

int readLineError() {
  const int w[8] = {-7, -5, -3, -1, 1, 3, 5, 7};
  long sumW = 0, active = 0;
  for (int i = 0; i < 8; i++) {
    bool onLine = analogRead(sensorPins[i]) < threshold;
    if (onLine) { sumW += w[i]; active++; }
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
  Serial.print(s3); Serial.print(" "); Serial.print(s4); Serial.print(" ");
  Serial.print(s5); Serial.print(" "); Serial.print(s6);
  Serial.print(" | CROSS: "); Serial.print(cross);
  Serial.print(" | ACTION: "); Serial.println(action);
}

void followLineWithP() {
  int err = readLineError();
  int corr = (int)(Kp * err);
  int left = constrain(baseLeft - corr, 0, 255);
  int right = constrain(baseRight + corr, 0, 255);
  const char* action = (err < 0) ? "SHIFT_LEFT" : (err > 0 ? "SHIFT_RIGHT" : "FORWARD");
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
    setMotorSpeeds(130, 130);
    delay(180);
  } else {
    printCrossDebug(s3, s4, s5, s6, false, "FOLLOW_LINE");
    followLineWithP();
  }
  delay(20);
}
```

---

Фінальна робоча версія для курсу знаходиться у:

- `project/line_follower_project.ino`
