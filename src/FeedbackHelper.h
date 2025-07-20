#ifndef FEEDBACK_HELPER_H
#define FEEDBACK_HELPER_H

#include "FeedbackManager.h"

class FeedbackHelper {
public:
  static FeedbackManager* feedback;

  static void playAutonomousMelody() {
    if (feedback) {
      feedback->play(MelodyType::Autonomous);
    }
  }

  static void playBluetoothMelody() {
    if (feedback) {
      feedback->play(MelodyType::Bluetooth);
    }
  }
  
  static void playBluetoothConnectedMelody() {
    if (feedback) {
      feedback->play(MelodyType::BTConnected);
    }
  }

  static void playTestModeMelody() {
    if (feedback) {
      feedback->play(MelodyType::TestMode);
    }
  }

  static void playStartMelody() {
    if (feedback) {
      feedback->play(MelodyType::Start);
    }
  }

  static void playObstacleMelody() {
    if (feedback) {
      feedback->play(MelodyType::Obstacle);
    }
  }

  static void playLeftMelody() {
    if (feedback) {
      feedback->play(MelodyType::Left);
    }
  }

  static void playRightMelody() {
    if (feedback) {
      feedback->play(MelodyType::Right);
    }
  }

  static void playErrorMelody() {
    if (feedback) {
      feedback->play(MelodyType::Error);
    }
  }

  static void playSuccessMelody() {
    if (feedback) {
      feedback->play(MelodyType::Success);
    }
  }
};

#endif
