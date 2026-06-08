# nong healjai doll Arduino code

โค้ดนี้ใช้ Arduino Nano ควบคุมบอร์ด MP3 แบบสั่ง ON/Play ด้วยขา Arduino และมอเตอร์ 28BYJ-48 ผ่านบอร์ด ULN2003

กติกาการทำงาน:

- กดสวิตช์ที่ขา `D6` แบบ `INPUT_PULLUP`
- Arduino สั่ง ON/Play เสียงที่ขา `D2`
- มอเตอร์หมุนไป 180 องศา แล้วหมุนกลับ 180 องศาซ้ำ ๆ พร้อมกับเสียง
- มอเตอร์หยุดเมื่อครบเวลาของไฟล์เสียงที่ตั้งไว้ใน `SOUND_PLAY_TIME_MS`

## Wiring

| Arduino Nano | Module |
| --- | --- |
| D2 | Audio ON/Play trigger |
| D6 | Push button to GND, using `INPUT_PULLUP` |
| D8 | ULN2003 IN1 |
| D9 | ULN2003 IN2 |
| D10 | ULN2003 IN3 |
| D11 | ULN2003 IN4 |
| 5V | ULN2003 VCC and MP3 board 5V input |
| GND | Common GND for Arduino, ULN2003, MP3 board, and power supply |

## Sound Duration

ตั้งเวลาความยาวไฟล์เสียงที่บรรทัดนี้:

```cpp
const unsigned long SOUND_PLAY_TIME_MS = 10000;
```

ตัวอย่าง:

- ไฟล์เสียง 5 วินาที ใช้ `5000`
- ไฟล์เสียง 10 วินาที ใช้ `10000`
- ไฟล์เสียง 30 วินาที ใช้ `30000`

บอร์ด MP3 แบบนี้ไม่ได้ส่งสัญญาณกลับมาว่าเสียงจบแล้ว โค้ดจึงใช้เวลาเป็นตัวกำหนดว่าเสียงจบเมื่อไร

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
