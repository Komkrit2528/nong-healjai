/*
  nong healjai doll

  Board: Arduino Nano
  Audio: HW-188 style MP3 player board with Play/Pause button
  Motor: 28BYJ-48 5V stepper motor with ULN2003 driver board

  Behavior:
  - Press the pull-up button once.
  - Arduino simulates one Play/Pause button press on the MP3 board.
  - The 28BYJ-48 turns forward 180 degrees, then returns 180 degrees.

  MP3 trigger wiring:
  Use a small NPN transistor or optocoupler across the MP3 board Play/Pause
  button contacts. Arduino D7 drives the transistor/opto input.
*/

#include <Stepper.h>

const byte BUTTON_PIN = 2;
const byte MP3_PLAY_TRIGGER_PIN = 7;

const byte MOTOR_IN1_PIN = 8;
const byte MOTOR_IN2_PIN = 9;
const byte MOTOR_IN3_PIN = 10;
const byte MOTOR_IN4_PIN = 11;

const unsigned long DEBOUNCE_MS = 40;
const unsigned long MP3_TRIGGER_MS = 250;
const unsigned long START_RETURN_DELAY_MS = 300;

const int STEPS_PER_REVOLUTION = 2048; // 28BYJ-48 common full output-shaft revolution.
const int HALF_TURN_STEPS = STEPS_PER_REVOLUTION / 2;
const int MOTOR_RPM = 10;

Stepper dollMotor(
  STEPS_PER_REVOLUTION,
  MOTOR_IN1_PIN,
  MOTOR_IN3_PIN,
  MOTOR_IN2_PIN,
  MOTOR_IN4_PIN
);

bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;
unsigned long lastDebounceAt = 0;
bool actionRunning = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(MP3_PLAY_TRIGGER_PIN, OUTPUT);
  digitalWrite(MP3_PLAY_TRIGGER_PIN, LOW);

  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  pinMode(MOTOR_IN3_PIN, OUTPUT);
  pinMode(MOTOR_IN4_PIN, OUTPUT);
  releaseMotor();

  dollMotor.setSpeed(MOTOR_RPM);

  Serial.begin(9600);
  Serial.println(F("nong healjai doll ready."));
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

  triggerMp3PlayButton();

  dollMotor.step(HALF_TURN_STEPS);
  delay(START_RETURN_DELAY_MS);
  dollMotor.step(-HALF_TURN_STEPS);

  releaseMotor();
  actionRunning = false;
}

void triggerMp3PlayButton() {
  digitalWrite(MP3_PLAY_TRIGGER_PIN, HIGH);
  delay(MP3_TRIGGER_MS);
  digitalWrite(MP3_PLAY_TRIGGER_PIN, LOW);
}

void releaseMotor() {
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  digitalWrite(MOTOR_IN3_PIN, LOW);
  digitalWrite(MOTOR_IN4_PIN, LOW);
}
