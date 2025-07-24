#ifndef ROBOT_TEST_H
#define ROBOT_TEST_H

#include <Motor.h>
#include <MotorControl.h>
#include <Buzzer.h>
#include <ServoWrapper.h>
#include <SensorUltrasonic.h>

const uint16_t speeds[] = { 100, 150, 200, 255 };
const uint16_t durations[] = { 500, 1000, 1500 };

class RobotTest {

private:
  MotorControl* mc;
  Buzzer* buzzer;
  ServoWrapper* servo;
  SensorUltrasonic* ultrasonic;
  int pauseBetweenTests = 10000;

  // Calibration
  void testForward();
  void testBackward();
  void testTurnLeft();
  void testTurnRight();
  void testRotateLeft();
  void testRotateRight();
  void testCurveLeft();
  void testCurveRight();
  void testM1();
  void testForwardCalibrate();
  void testForwardCalibrateM1();

  void testDiagonalLeftCalibrate(); 
  void testDiagonalRightCalibrate(); 
  void testLeftCalibrate(); 
  void testRightCalibrate(); 
  void testDiagonalLeftBackCalibrate();
  void testDiagonalRightBackCalibrate();

  // PWM
  void testForwardBoth();
  void testBackwardBoth();
  void testTurnLeftPWM();
  void testTurnRightPWM();
  void testTurnLeftInvert();
  void testTurnRightInvert();
  void testSoftTurnAroundRight();
  void testSoftTurnAroundLeft();
  void testForwardTiming();
  void testLeftMotorPWM();
  void testRightMotorPWM();

  // Modules
  void testServoSweep();
  void testUltrasonicDistance();

public:
  RobotTest(MotorControl* mc, Buzzer* buzzer, ServoWrapper* servo, SensorUltrasonic* ultrasonic);
  RobotTest(MotorControl* mc, Buzzer* buzzer, ServoWrapper* servo, SensorUltrasonic* ultrasonic, int pauseDuration);
  static void informationControlledTests();
  void runControlledTests();
  void runRoutineTests();
};

#endif
