/*
  nong healjai doll

  Board: Arduino Nano
  Audio: DFPlayer Mini MP3 module
  Stepper driver: A4988/DRV8825 style STEP/DIR driver

  Behavior:
  - Press the pull-up button once.
  - DFPlayer plays /mp3/0001.mp3 from the microSD card.
  - The stepper motor turns forward 180 degrees, then returns 180 degrees.
*/

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <AccelStepper.h>

const byte BUTTON_PIN = 2;
const byte DFPLAYER_RX_PIN = 10; // Arduino RX, connect to DFPlayer TX.
const byte DFPLAYER_TX_PIN = 11; // Arduino TX, connect to DFPlayer RX through 1k resistor.
const byte STEPPER_STEP_PIN = 3;
const byte STEPPER_DIR_PIN = 4;
const byte STEPPER_ENABLE_PIN = 5;

const unsigned long DEBOUNCE_MS = 40;
const unsigned long START_RETURN_DELAY_MS = 300;

const int MP3_TRACK_NUMBER = 1;
const int MP3_VOLUME = 25; // 0 to 30.

const long MOTOR_STEPS_PER_REV = 200; // 1.8 degree stepper motor.
const int MICROSTEPS = 16;            // Match MS1/MS2/MS3 wiring on the driver.
const long HALF_TURN_STEPS = (MOTOR_STEPS_PER_REV * MICROSTEPS) / 2;

SoftwareSerial mp3Serial(DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);
DFRobotDFPlayerMini mp3;
AccelStepper stepper(AccelStepper::DRIVER, STEPPER_STEP_PIN, STEPPER_DIR_PIN);

bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;
unsigned long lastDebounceAt = 0;
bool actionRunning = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(STEPPER_ENABLE_PIN, OUTPUT);
  digitalWrite(STEPPER_ENABLE_PIN, HIGH); // Disable driver when idle. LOW enables most drivers.

  Serial.begin(9600);
  mp3Serial.begin(9600);

  stepper.setMaxSpeed(900);
  stepper.setAcceleration(600);
  stepper.setCurrentPosition(0);

  if (!mp3.begin(mp3Serial)) {
    Serial.println(F("DFPlayer Mini not found. Check wiring and SD card."));
  } else {
    mp3.volume(MP3_VOLUME);
    Serial.println(F("DFPlayer Mini ready."));
  }
}

void loop() {
  if (buttonWasPressed() && !actionRunning) {
    playVoiceAndMoveMotor();
  }
}

bool buttonWasPressed() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceAt = millis();
  }

  if ((millis() - lastDebounceAt) > DEBOUNCE_MS && reading != stableButtonState) {
    stableButtonState = reading;

    if (stableButtonState == LOW) {
      lastButtonReading = reading;
      return true;
    }
  }

  lastButtonReading = reading;
  return false;
}

void playVoiceAndMoveMotor() {
  actionRunning = true;
  digitalWrite(STEPPER_ENABLE_PIN, LOW);

  mp3.playMp3Folder(MP3_TRACK_NUMBER);

  stepper.moveTo(HALF_TURN_STEPS);
  stepper.runToPosition();

  delay(START_RETURN_DELAY_MS);

  stepper.moveTo(0);
  stepper.runToPosition();

  digitalWrite(STEPPER_ENABLE_PIN, HIGH);
  actionRunning = false;
}
