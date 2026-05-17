# Модуль 01 — Старт і базовий рух

## Аналогія
Робот як велосипед: спочатку вчимося просто їхати рівно, ще без складних маневрів.

## Архітектурна суть
- Знайомство з цілями MakerTime.
- Поділ на команди та ролі.
- Визначення пінів підключення моторів і сенсорного блоку.
- Базові команди руху: вперед, стоп, плавний поворот.

## Практична перевірка
- Кожна команда запускає робота на короткій прямій.
- Робот має виконати: старт → 2 секунди руху → стоп.
- Чек-лист: живлення, напрям обертання моторів, реакція на команду стоп.

### Arduino-приклад: старт → рух → стоп + базовий debug

```cpp
void printMotionDebug(const char* phase, int leftSpeed, int rightSpeed) {
  Serial.print("PHASE: "); Serial.print(phase);
  Serial.print(" | L: "); Serial.print(leftSpeed);
  Serial.print(" | R: "); Serial.println(rightSpeed);
}

void runStartDemo() {
  const int leftSpeed = 130;
  const int rightSpeed = 130;

  printMotionDebug("START", 0, 0);
  delay(500);

  setMotorSpeeds(leftSpeed, rightSpeed);
  printMotionDebug("FORWARD_2S", leftSpeed, rightSpeed);
  delay(2000);

  setMotorSpeeds(0, 0);
  printMotionDebug("STOP", 0, 0);
}
```

Очікуваний формат логу:

```text
PHASE: START | L: 0 | R: 0
PHASE: FORWARD_2S | L: 130 | R: 130
PHASE: STOP | L: 0 | R: 0
```

## Важливий момент
На першому занятті не гонимося за швидкістю — головне стабільний і передбачуваний рух.
