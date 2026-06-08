# nong healjai doll Arduino code

โค้ดนี้ใช้ Arduino Nano ควบคุม DFPlayer Mini ให้เล่นไฟล์เสียงเมื่อกดสวิตช์แบบ pull-up 1 ครั้ง และสั่งสเต็ปปิ้งมอเตอร์หมุนไป 180 องศา แล้วหมุนกลับ 180 องศา

## Hardware

- Arduino Nano
- DFPlayer Mini MP3 module
- MicroSD card
- Stepper motor
- A4988 หรือ DRV8825 stepper driver
- Push button
- External motor power supply

## Wiring

| Arduino Nano | Module |
| --- | --- |
| D2 | Push button to GND, using `INPUT_PULLUP` |
| D3 | STEP on A4988/DRV8825 |
| D4 | DIR on A4988/DRV8825 |
| D5 | ENABLE on A4988/DRV8825 |
| D10 | DFPlayer TX |
| D11 | DFPlayer RX through 1k resistor |
| 5V | DFPlayer VCC |
| GND | Common GND for Arduino, DFPlayer, and motor driver |

## MP3 file

Put the sound file on the microSD card as:

```text
/mp3/0001.mp3
```

## Arduino libraries

Install these libraries in Arduino IDE:

- `DFRobotDFPlayerMini`
- `AccelStepper`

## Motor setup

The sketch assumes:

- 200 steps per revolution stepper motor
- 16x microstepping
- 180 degrees = 1600 step pulses

If your driver uses a different microstep setting, change:

```cpp
const int MICROSTEPS = 16;
```
