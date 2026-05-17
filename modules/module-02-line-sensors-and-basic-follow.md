# Модуль 02 — Датчики лінії і базове слідування

## Аналогія
Датчики для робота як очі для людини: вони допомагають зрозуміти, де чорна лінія, а де біле поле.

## Архітектурна суть
- Повторення: що таке датчик лінії.
- Читання станів 8 аналогових датчиків через функцію `bool(sensorNumber)` для номерів 1..8.
- Побудова простого алгоритму:
  - якщо центр бачить лінію — їдемо прямо;
  - якщо лінія зліва — корекція вліво;
  - якщо лінія справа — корекція вправо.

## Практична перевірка
- Вивести на Serial Monitor стани датчиків під час руху.
- Провести 5 тестових заїздів по простій трасі.
- Зафіксувати, де робот найчастіше сходить з лінії.

### Debug-вивід: стан датчиків і реакція алгоритму

```cpp
void printLineDebug(bool s1, bool s2, bool s3, bool s4, bool s5, bool s6, bool s7, bool s8, const char* action) {
  Serial.print("SENSORS: ");
  Serial.print(s1); Serial.print(" ");
  Serial.print(s2); Serial.print(" ");
  Serial.print(s3); Serial.print(" ");
  Serial.print(s4); Serial.print(" ");
  Serial.print(s5); Serial.print(" ");
  Serial.print(s6); Serial.print(" ");
  Serial.print(s7); Serial.print(" ");
  Serial.print(s8);
  Serial.print(" | ACTION: ");
  Serial.println(action);
}

void loop() {
  bool s1 = sensor(1), s2 = sensor(2), s3 = sensor(3), s4 = sensor(4);
  bool s5 = sensor(5), s6 = sensor(6), s7 = sensor(7), s8 = sensor(8);

  const char* action = "STOP";

  if (s4 || s5) {
    action = "FORWARD";
    moveForward();
  } else if (s1 || s2 || s3) {
    action = "TURN_LEFT";
    turnLeftSoft();
  } else if (s6 || s7 || s8) {
    action = "TURN_RIGHT";
    turnRightSoft();
  } else {
    stopMotors();
  }

  printLineDebug(s1, s2, s3, s4, s5, s6, s7, s8, action);
  delay(50);
}
```

Очікуваний формат логу:

```text
SENSORS: 0 0 0 1 1 0 0 0 | ACTION: FORWARD
SENSORS: 1 1 0 0 0 0 0 0 | ACTION: TURN_LEFT
SENSORS: 0 0 0 0 0 1 1 0 | ACTION: TURN_RIGHT
```

## Важливий момент
Перед ускладненням алгоритму треба переконатися, що зчитування датчиків стабільне і зрозуміле для дітей.
