const byte AUDIO_ON_PIN = 2;
const byte BUTTON_PIN = 6;

const byte MOTOR_IN1_PIN = 8;
const byte MOTOR_IN2_PIN = 9;
const byte MOTOR_IN3_PIN = 10;
const byte MOTOR_IN4_PIN = 11;

const unsigned long DEBOUNCE_MS = 40;
const unsigned long AUDIO_ON_PULSE_MS = 250;
const unsigned long SOUND_PLAY_TIME_MS = 10000; // Change this to match your MP3 length.

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
  pinMode(AUDIO_ON_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  pinMode(MOTOR_IN3_PIN, OUTPUT);
  pinMode(MOTOR_IN4_PIN, OUTPUT);

  digitalWrite(AUDIO_ON_PIN, LOW);
  releaseMotor();

  Serial.begin(9600);
  Serial.println(F("nong healjai doll ready."));
}

void loop() {
  if (buttonWasPressed() && !actionRunning) {
    playAudioAndMoveMotorUntilFinished();
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

void playAudioAndMoveMotorUntilFinished() {
  actionRunning = true;

  triggerAudioOn();

  unsigned long startedAt = millis();
  int direction = 1;

  while (millis() - startedAt < SOUND_PLAY_TIME_MS) {
    moveHalfTurnUntilTimeEnds(direction, startedAt);
    direction = -direction;
  }

  releaseMotor();
  actionRunning = false;
}

void triggerAudioOn() {
  digitalWrite(AUDIO_ON_PIN, HIGH);
  delay(AUDIO_ON_PULSE_MS);
  digitalWrite(AUDIO_ON_PIN, LOW);
}

void moveHalfTurnUntilTimeEnds(int direction, unsigned long startedAt) {
  for (int i = 0; i < HALF_TURN_STEPS; i++) {
    if (millis() - startedAt >= SOUND_PLAY_TIME_MS) {
      return;
    }

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
