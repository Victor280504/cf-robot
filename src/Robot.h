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
  CONTROLED_ACTIONS,
};

enum ROBOT_ACTIONS {
  R_LEFT,
  R_RIGHT,
  R_FORWARD,
  R_BACKWARD,
  R_STOP,
  R_DIAGONAL_LEFT,
  R_DIAGONAL_RIGHT,
  R_BACK_DIAGONAL_LEFT,
  R_BACK_DIAGONAL_RIGHT,
  R_TURN_AROUND_LEFT,
  R_TURN_AROUND_RIGHT,
  R_TURN_PI_LEFT,
  R_TURN_PI_RIGHT,
};

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
  Event controledFunction;

  const float OBSTACLE_DISTANCE_CM = 25;
  const int MAX_ATTEMPTS = 3;

  int noActionCount = 0;
  unsigned long forwardStartTime = 0;
  bool isMovingForward = false;
  bool bluetoothConnected = false;
  bool controlMode = false;

  static ROBOT_MODE robotMode;
  static bool testModeIsActive;

  static void setBluetoothMode();
  static void setAutonomousMode();

  void defineCallbacks();
  void handleNoEscape();
  
  void backward();
  void forward();
  void left();
  void right();
  void stop();
  void diagonalLeft();
  void diagonalRight();
  void backDiagonalLeft();
  void backDiagonalRight();
  void turnAroundLeft();
  void turnAroundRight();
  void turnPILeft();
  void turnPIRight();
  
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
  void setControlMode(bool mode) {
    controlMode = mode;
  }
  void setControlledFunction(Event::Callback cb){
    controledFunction.setCallback(cb);
  }
  void setMove(ROBOT_ACTIONS action) {
    switch (action) {
      case R_FORWARD: forward(); break;
      case R_BACKWARD: backward(); break;
      case R_LEFT: left(); break;
      case R_RIGHT: right(); break;
      case R_DIAGONAL_LEFT: diagonalLeft(); break;
      case R_DIAGONAL_RIGHT: diagonalRight(); break;
      case R_BACK_DIAGONAL_LEFT: backDiagonalLeft(); break;
      case R_BACK_DIAGONAL_RIGHT: backDiagonalRight(); break;
      case R_TURN_AROUND_LEFT: turnAroundLeft(); break;
      case R_TURN_AROUND_RIGHT: turnAroundRight(); break;
      case R_STOP: stop(); break;
      case R_TURN_PI_LEFT: turnPILeft(); break;
      case R_TURN_PI_RIGHT: turnPIRight(); break;
      default: mc.stopAll(); break;
    }
  }
  ROBOT_ACTIONS decideEscapeDirection();
  float measureDistanceAtAngle(int angle, int delayMs = 800);
};

#endif