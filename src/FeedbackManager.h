// FeedbackManager.h
#ifndef FEEDBACK_MANAGER_H
#define FEEDBACK_MANAGER_H

#include <Buzzer.h>
#include <Note.h>

enum class MelodyType {
  Start,
  TestMode,
  Obstacle,
  Left,
  Right,
  Error,
  Bluetooth,
  Autonomous,
  Success,
  BTConnected,
};

static Note startMelody[] = { { 1000, 100 }, { 1200, 100 } };
static Note testModeMelody[] = { { 1000, 100 }, { 800, 100 }, { 1000, 100 }, { 1200, 150 }, { 1000, 100 } };
static Note obstacleMelody[] = { { 800, 100 }, { 800, 100 }, { 800, 100 } };
static Note leftMelody[] = { { 600, 150 }, { 1000, 150 } };
static Note rightMelody[] = { { 1000, 150 }, { 600, 150 } };
static Note errorMelody[] = { { 400, 300 }, { 400, 300 } };
static Note bluetoothMelody[] = { { 1000, 150 }, { 800, 150 }, { 600, 200 } };
static Note autonomousMelody[] = { { 600, 150 }, { 800, 150 }, { 1000, 200 } };
static Note successMelody[] = { { 100, 100 }, { 100, 100 } };
static Note btConnectedMelody[] = {{ 600, 100 }, { 800, 100 }, { 1000, 150 }, { 1200, 200 } };

class FeedbackManager {
private:
  Buzzer* buzzer;
  MelodyType lastPlayed;
  int volume = 10;  // Range: 0 (mute) to 10 (max)

public:
  FeedbackManager(Buzzer* buzzer);

  void play(MelodyType type);
  void repeatLastMelody();
  void setVolume(int level);  // 0 to 10
};

#endif
