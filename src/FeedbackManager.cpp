#include "FeedbackManager.h"
#include <Arduino.h>

FeedbackManager::FeedbackManager(Buzzer* buzzer)
  : buzzer(buzzer) {}

void FeedbackManager::play(MelodyType type) {
  const Note* melody = nullptr;
  int length = 0;

  switch (type) {
    case MelodyType::Start:
      melody = startMelody;
      length = 2;
      break;
    case MelodyType::TestMode:
      melody = testModeMelody;
      length = 5;
      break;
    case MelodyType::Obstacle:
      melody = obstacleMelody;
      length = 3;
      break;
    case MelodyType::Left:
      melody = leftMelody;
      length = 2;
      break;
    case MelodyType::Right:
      melody = rightMelody;
      length = 2;
      break;
    case MelodyType::Error:
      melody = errorMelody;
      length = 2;
      break;
    case MelodyType::Bluetooth:
      melody = bluetoothMelody;
      length = 3;
      Serial.println(F("BLUETOOTH"));
      break;
    case MelodyType::Autonomous:
      melody = autonomousMelody;
      length = 3;
      Serial.println(F("AUTONOMOUS"));
      break;
    case MelodyType::Success:
      melody = successMelody;
      length = 2;
      break;
    case MelodyType::BTConnected:
      melody = btConnectedMelody;
      length = 4;
      break;
  }

  if (melody && length > 0) {
    lastPlayed = type;
    buzzer->playMelody(melody, length);
  }
}

void FeedbackManager::repeatLastMelody() {
  play(lastPlayed);
}

void FeedbackManager::setVolume(int level) {
  volume = constrain(level, 0, 10);
}

