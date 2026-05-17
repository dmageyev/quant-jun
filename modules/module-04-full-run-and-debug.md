# Модуль 04 — Повний заїзд, перехрестя і дебаг

## Аналогія
Це як генеральна репетиція перед виступом: запускаємо повний сценарій, ловимо помилки, покращуємо результат.

## Архітектурна суть
- Реалізація логіки проходження перехресть.
- Узгодження правил поведінки на перехресті (наприклад, пріоритет прямого руху).
- Проведення першого повного заїзду з фіксацією часу.
- Аналіз багів: зліт з лінії, нестабільні повороти, помилки вибору траєкторії.

## Практична перевірка
- Мінімум 3 повних кола на команду.
- Ведення таблиці: час кола, кількість сходів, поведінка на перехрестях.
- Після кожної ітерації — одна зміна в налаштуваннях і повторний заїзд.

### Debug-вивід: перехрестя і рішення алгоритму

```cpp
void printCrossDebug(bool s3, bool s4, bool s5, bool s6, bool isCross, const char* action) {
  Serial.print("S3-6: ");
  Serial.print(s3); Serial.print(" ");
  Serial.print(s4); Serial.print(" ");
  Serial.print(s5); Serial.print(" ");
  Serial.print(s6);
  Serial.print(" | CROSS: "); Serial.print(isCross);
  Serial.print(" | ACTION: "); Serial.println(action);
}

void handleIntersection() {
  bool s3 = sensor(3), s4 = sensor(4), s5 = sensor(5), s6 = sensor(6);
  bool isCross = s3 && s4 && s5 && s6;

  const char* action = "FOLLOW_LINE";

  if (isCross) {
    action = "GO_STRAIGHT_THROUGH_CROSS";
    setMotorSpeeds(135, 135);
    delay(180);
  } else {
    followLineWithP();
  }

  printCrossDebug(s3, s4, s5, s6, isCross, action);
}
```

Очікуваний формат логу:

```text
S3-6: 1 1 1 1 | CROSS: 1 | ACTION: GO_STRAIGHT_THROUGH_CROSS
S3-6: 0 1 1 0 | CROSS: 0 | ACTION: FOLLOW_LINE
```

## Важливий момент
На етапі дебагу змінюємо тільки один параметр за раз, щоб розуміти причину покращення або погіршення.
