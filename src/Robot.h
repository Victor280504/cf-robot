#ifndef ROBOT_H
#define ROBOT_H

#include <Motor.h>
#include <MotorControl.h>
#include <SensorUltrasonic.h>
#include <ServoWrapper.h>
#include <SliderButton.h>
#include <PushButton.h>
#include <Buzzer.h>
#include <Note.h>
#include <Servo.h>
#include "RobotTest.h"
#include "FeedbackManager.h"
#include "FeedbackHelper.h"

enum ROBOT_MODE {
  BLUETOOTH,
  AUTONOMOUS,
};

// enum ROBOT_ACTIONS {
//   LEFT,
//   RIGHT,
//   FORWARD,
//   BACKWARD,
  
// };

class Robot {
private:
  // === Motors ===
  Motor leftFront;
  Motor rightFront;
  ServoWrapper servo;
  MotorControl mc;

  // === Sensors ===
  SensorUltrasonic sonar;
  Buzzer buzzer;

  // === Buttons ===
  SliderButton sliderMode;
  PushButton changeTestModeButton;

  // === Settings ===
  RobotTest robotTesting;
  FeedbackManager feedback;

  const float OBSTACLE_DISTANCE_CM = 25;
  const int MAX_ATTEMPTS = 3;

  int noActionCount = 0;
  bool bluetoothConnected = false;

  static ROBOT_MODE robotMode;
  static bool testModeIsActive;

  static void setBluetoothMode();
  static void setAutonomousMode();

  void defineCallbacks();
  void handleNoEscape();
  int decideEscapeDirection();
  float measureDistanceAtAngle(int angle, int delayMs = 800);

public:
  static void changeRobotMode();
  Robot(
    uint8_t leftMotorPins[3],
    uint8_t rightMotorPins[3],
    uint8_t servoPin,
    uint8_t sonarPins[2],
    uint8_t buzzerPin,
    uint8_t sliderPin,
    uint8_t buttonPin);
  void initialize();
  void controller();
  void runAutonomousMode();
  void runBluetoothMode();
  void checkBluetoothConnection();
};

#endif