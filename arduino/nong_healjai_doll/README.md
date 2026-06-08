# nong healjai doll Arduino code

โค้ดนี้ใช้ Arduino Nano ควบคุมบอร์ด MP3 แบบสั่งจ่ายไฟด้วยขา Arduino และมอเตอร์ 28BYJ-48 ผ่านบอร์ด ULN2003

กติกาการทำงาน:

- กดสวิตช์ที่ขา `D6` แบบ `INPUT_PULLUP`
- Arduino ส่งสัญญาณ HIGH 5V ที่ขา `D2` เพื่อสั่งวงจรจ่ายไฟให้บอร์ดเสียง
- มอเตอร์หมุนไป 180 องศา แล้วหมุนกลับ 180 องศาซ้ำ ๆ พร้อมกับเสียง
- บอร์ดเสียงและมอเตอร์ทำงาน 10 นาที แล้ว Arduino ปิดขา `D2`

## Wiring

| Arduino Nano | Module |
| --- | --- |
| D2 | Audio power control signal |
| D6 | Push button to GND, using `INPUT_PULLUP` |
| D8 | ULN2003 IN1 |
| D9 | ULN2003 IN2 |
| D10 | ULN2003 IN3 |
| D11 | ULN2003 IN4 |
| 5V | ULN2003 VCC and MP3 board 5V input |
| GND | Common GND for Arduino, ULN2003, MP3 board, and power supply |

## Audio Power Duration

ตั้งเวลาจ่ายไฟให้บอร์ดเสียงที่บรรทัดนี้:

```cpp
const unsigned long AUDIO_POWER_ON_TIME_MS = 600000;
```

ค่า `600000` คือ 10 นาที

## Important Power Note

ห้ามใช้ขา `D2` จ่ายไฟเลี้ยงบอร์ดเสียงโดยตรง เพราะขา Arduino จ่ายกระแสได้น้อย ให้ใช้ `D2` เป็นสัญญาณควบคุม transistor, MOSFET, relay module หรือ power switch module เพื่อเปิดไฟ 5V ให้บอร์ดเสียงแทน

## Motor Setup

โค้ดตั้งค่า 28BYJ-48 เป็น:

- 180 degrees = 1024 steps
- Step delay = 3 ms

ถ้ามอเตอร์หมุนเร็วหรือช้าเกินไป ให้ปรับ:

```cpp
const unsigned int STEP_DELAY_MS = 3;
```

ถ้ามอเตอร์หมุนสั่นหรือไม่หมุน ให้ลองสลับลำดับสาย IN1-IN4 หรือปรับลำดับใน `STEP_SEQUENCE`

## Arduino Libraries

โค้ดเวอร์ชันนี้ไม่ต้องติดตั้งไลบรารีเพิ่ม และไม่ใช้ `Stepper.h`
