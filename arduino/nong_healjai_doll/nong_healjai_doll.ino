const byte BUTTON_PIN = 2;
const byte MP3_PLAY_TRIGGER_PIN = 7;

const byte MOTOR_IN1_PIN = 8;
const byte MOTOR_IN2_PIN = 9;
const byte MOTOR_IN3_PIN = 10;
const byte MOTOR_IN4_PIN = 11;

const unsigned long DEBOUNCE_MS = 40;
const unsigned long MP3_TRIGGER_MS = 250;
const unsigned long START_RETURN_DELAY_MS = 300;

const int HALF_TURN_STEPS = 1024; // 28BYJ-48: about 180 degrees.
const unsigned int STEP_DELAY_MS = 3;

bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;
unsigned long lastDebounceAt = 0;
bool actionRunning = false;

const byte STEP_SEQUENCE[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(MP3_PLAY_TRIGGER_PIN, OUTPUT);

  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  pinMode(MOTOR_IN3_PIN, OUTPUT);
  pinMode(MOTOR_IN4_PIN, OUTPUT);

  digitalWrite(MP3_PLAY_TRIGGER_PIN, LOW);
  releaseMotor();

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

  moveMotor(HALF_TURN_STEPS);
  delay(START_RETURN_DELAY_MS);
  moveMotor(-HALF_TURN_STEPS);

  releaseMotor();
  actionRunning = false;
}

void triggerMp3PlayButton() {
  digitalWrite(MP3_PLAY_TRIGGER_PIN, HIGH);
  delay(MP3_TRIGGER_MS);
  digitalWrite(MP3_PLAY_TRIGGER_PIN, LOW);
}

void moveMotor(int steps) {
  int direction = 1;

  if (steps < 0) {
    direction = -1;
    steps = -steps;
  }

  for (int i = 0; i < steps; i++) {
    int sequenceIndex = i % 8;

    if (direction < 0) {
      sequenceIndex = 7 - sequenceIndex;
    }

    setMotorStep(sequenceIndex);
    delay(STEP_DELAY_MS);
  }
}

void setMotorStep(int sequenceIndex) {
  digitalWrite(MOTOR_IN1_PIN, STEP_SEQUENCE[sequenceIndex][0]);
  digitalWrite(MOTOR_IN2_PIN, STEP_SEQUENCE[sequenceIndex][1]);
  digitalWrite(MOTOR_IN3_PIN, STEP_SEQUENCE[sequenceIndex][2]);
  digitalWrite(MOTOR_IN4_PIN, STEP_SEQUENCE[sequenceIndex][3]);
}

void releaseMotor() {
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  digitalWrite(MOTOR_IN3_PIN, LOW);
  digitalWrite(MOTOR_IN4_PIN, LOW);
}
