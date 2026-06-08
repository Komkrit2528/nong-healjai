# nong healjai doll Arduino code

โค้ดนี้ใช้ Arduino Nano กับอุปกรณ์ตามรูป:

- บอร์ดเล่น MP3 แบบมีปุ่ม Play/Pause, Previous, Next และช่อง MicroSD/USB
- มอเตอร์ 28BYJ-48 5V พร้อมบอร์ดไดรเวอร์ ULN2003

เมื่อกดสวิตช์แบบ pull-up 1 ครั้ง Arduino จะสั่งให้บอร์ด MP3 เล่นเสียงโดยจำลองการกดปุ่ม Play/Pause แล้วสั่งให้มอเตอร์หมุนไป 180 องศาและหมุนกลับ 180 องศา

## Hardware

- Arduino Nano
- HW-188 style MP3 player board หรือบอร์ด MP3 แบบปุ่มกดตามรูป
- MicroSD card with MP3 file
- 28BYJ-48 5V stepper motor
- ULN2003 stepper driver board
- Push button
- 5V power supply
- NPN transistor เช่น 2N2222/S8050 หรือ optocoupler สำหรับกดปุ่ม Play/Pause แทนมือ

## Wiring

| Arduino Nano | Module |
| --- | --- |
| D2 | Push button to GND, using `INPUT_PULLUP` |
| D7 | MP3 Play/Pause trigger circuit |
| D8 | ULN2003 IN1 |
| D9 | ULN2003 IN2 |
| D10 | ULN2003 IN3 |
| D11 | ULN2003 IN4 |
| 5V | ULN2003 VCC and MP3 board 5V input |
| GND | Common GND for Arduino, ULN2003, MP3 board, and power supply |

## MP3 trigger circuit

บอร์ด MP3 ในรูปไม่ได้ใช้ serial command แบบ DFPlayer Mini จึงให้ Arduino สั่งเล่นเสียงด้วยการจำลองการกดปุ่ม Play/Pause

แนะนำให้ใช้ NPN transistor หรือ optocoupler คร่อมขาปุ่ม Play/Pause บนบอร์ด MP3:

```text
Arduino D7 -- 1k resistor -- NPN base
NPN emitter ---------------- GND
NPN collector -------------- one side of MP3 Play/Pause button
MP3 button other side ------ GND side of the Play/Pause button
```

ถ้าปุ่ม Play/Pause บนบอร์ดไม่ได้ต่อข้างหนึ่งลง GND ให้ใช้ optocoupler แทน transistor เพื่อแยกวงจรและทำตัวเป็นสวิตช์คร่อมปุ่ม

## MP3 file

ใส่ไฟล์เสียงลง MicroSD ตามรูปแบบที่บอร์ด MP3 ของคุณรองรับ เช่น:

```text
001.mp3
```

ก่อนต่อ Arduino ให้ลองกดปุ่ม Play/Pause บนบอร์ด MP3 ด้วยมือก่อนว่าเล่นไฟล์เสียงได้

## Arduino libraries

ใช้ไลบรารีมาตรฐานของ Arduino:

- `Stepper`

ไม่ต้องใช้ `DFRobotDFPlayerMini` แล้ว เพราะบอร์ด MP3 ตัวนี้ควบคุมแบบปุ่มกด

## Motor setup

โค้ดตั้งค่า 28BYJ-48 เป็น:

- 2048 steps per output-shaft revolution
- 180 degrees = 1024 steps
- Speed = 10 RPM

ถ้ามอเตอร์หมุนสั่นหรือไม่หมุน ให้ลองสลับลำดับสาย IN1-IN4 หรือเปลี่ยนลำดับพินใน constructor:

```cpp
Stepper dollMotor(STEPS_PER_REVOLUTION, MOTOR_IN1_PIN, MOTOR_IN3_PIN, MOTOR_IN2_PIN, MOTOR_IN4_PIN);
```
