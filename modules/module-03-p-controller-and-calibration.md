# Модуль 03 — P-контролер і калібрування двигунів

## Аналогія
Коли ми йдемо по лінії на підлозі, то що сильніше відхилилися — то сильніше виправляємо крок. Це і є логіка P-контролера.

## Архітектурна суть
- Введення помилки (error) відносно центру лінії.
- Використання P-контролера: сила корекції пропорційна помилці.
- Налаштування коефіцієнта `Kp`.
- Калібрування базових швидкостей моторів:
  - підібрати значення лівого/правого двигуна так, щоб робот їхав рівно на прямій.

## Практична перевірка
- Серія тестів з різними `Kp`.
- Вимір часу кола до і після калібрування.
- Фіксація мінімального стабільного `Kp`, при якому робот не «розгойдується».

### Debug-вивід: помилка, корекція і команди моторам

```cpp
void printControlDebug(int error, int correction, int leftSpeed, int rightSpeed, const char* action) {
  Serial.print("ERR: "); Serial.print(error);
  Serial.print(" | CORR: "); Serial.print(correction);
  Serial.print(" | L: "); Serial.print(leftSpeed);
  Serial.print(" | R: "); Serial.print(rightSpeed);
  Serial.print(" | ACTION: "); Serial.println(action);
}

void applyPControl(int error, int baseSpeed, float Kp) {
  int correction = (int)(Kp * error);
  int leftSpeed = constrain(baseSpeed - correction, 0, 255);
  int rightSpeed = constrain(baseSpeed + correction, 0, 255);

  const char* action = "FORWARD";
  if (error < 0) action = "SHIFT_LEFT";
  if (error > 0) action = "SHIFT_RIGHT";

  setMotorSpeeds(leftSpeed, rightSpeed);
  printControlDebug(error, correction, leftSpeed, rightSpeed, action);
}
```

Очікуваний формат логу:

```text
ERR: -2 | CORR: -18 | L: 158 | R: 122 | ACTION: SHIFT_LEFT
ERR: 0 | CORR: 0 | L: 140 | R: 140 | ACTION: FORWARD
ERR: 3 | CORR: 27 | L: 113 | R: 167 | ACTION: SHIFT_RIGHT
```

## Важливий момент
Калібрування моторів робиться перед фінальним тюнінгом `Kp`, інакше налаштування контролера буде неточним.
