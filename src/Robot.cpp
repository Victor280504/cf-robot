#include "Robot.h"
#include "FeedbackHelper.h"

ROBOT_MODE Robot::robotMode = BLUETOOTH;
bool Robot::testModeIsActive = false;
FeedbackManager* FeedbackHelper::feedback = nullptr;

static unsigned long lastInterruptTime = 0;
volatile bool testModeChanged = false;

void onTestModeInterrupt() {
  unsigned long currentTime = millis();
  if (currentTime - lastInterruptTime > 200) {
    testModeChanged = true;
    lastInterruptTime = currentTime;
  }
}

Robot::Robot(
  uint8_t leftMotorPins[3],
  uint8_t rightMotorPins[3],
  uint8_t servoPin,
  uint8_t sonarPins[2],
  uint8_t buzzerPin,
  uint8_t sliderPin,
  uint8_t buttonPin)
  : leftFront(leftMotorPins[0], leftMotorPins[1], leftMotorPins[2]),
    rightFront(rightMotorPins[0], rightMotorPins[1], rightMotorPins[2]),
    servo(servoPin),
    sonar(sonarPins[0], sonarPins[1]),
    buzzer(buzzerPin),
    sliderMode(sliderPin),
    changeTestModeButton(buttonPin),
    robotTesting(&mc, &buzzer, &servo, &sonar, 5000),
    feedback(&buzzer){};

static void Robot::changeRobotMode() {
  testModeIsActive = !testModeIsActive;
  if (robotMode == AUTONOMOUS && testModeIsActive) {
    Serial.println(F("AUTONOMOUS_TEST"));
    FeedbackHelper::playTestModeMelody();
  } else if (robotMode == BLUETOOTH && testModeIsActive) {
    Serial.println(F("BLUETOOTH_TEST"));
    FeedbackHelper::playTestModeMelody();
    RobotTest::informationControlledTests();
  } else if (robotMode == BLUETOOTH && !testModeIsActive) {
    Serial.println(F("BLUETOOTH"));
    FeedbackHelper::playBluetoothMelody();
  } else if (robotMode == AUTONOMOUS && !testModeIsActive) {
    Serial.println(F("AUTONOMOUS"));
    FeedbackHelper::playAutonomousMelody();
  }
}

void Robot::defineCallbacks() {
  sliderMode.setEventCallback(setAutonomousMode, 0);
  sliderMode.setEventCallback(setBluetoothMode, 1);
  sliderMode.setFeedbackCallback(FeedbackHelper::playAutonomousMelody, 0);
  sliderMode.setFeedbackCallback(FeedbackHelper::playBluetoothMelody, 1);

  // changeTestModeButton.setEventCallback(changeRobotMode);
}

static void Robot::setBluetoothMode() {
  robotMode = BLUETOOTH;
}

static void Robot::setAutonomousMode() {
  robotMode = AUTONOMOUS;
};

void Robot::initialize() {
  FeedbackHelper::feedback = &feedback;
  robotMode = BLUETOOTH;
  testModeIsActive = false;
  changeTestModeButton.initializePinIN();
  sliderMode.initializePinIN();

  mc.addMotor(FRONT_LEFT, &leftFront);
  mc.addMotor(FRONT_RIGHT, &rightFront);

  defineCallbacks();
  attachInterrupt(digitalPinToInterrupt(changeTestModeButton.getPin()), onTestModeInterrupt, FALLING);

  servo.attach();
  servo.moveTo(90);

  delay(300);
  Serial.println("Sistema iniciado.");
  if (testModeIsActive) {
    robotTesting.informationControlledTests();
  }
  FeedbackHelper::playStartMelody();
}

void Robot::controller() {
  switch (robotMode) {
    case BLUETOOTH:
      if (testModeIsActive) {
        robotTesting.runControlledTests();
      } else {
        runBluetoothMode();
      }
      break;
    case AUTONOMOUS:
      if (testModeIsActive) {
        robotTesting.runRoutineTests();
      } else {
        runAutonomousMode();
      }
    default:
      runBluetoothMode();
      break;
  }

  sliderMode.getStateFeedback();
  sliderMode.updateAlways();

  if (testModeChanged) {
    testModeChanged = false;
    changeRobotMode();
  }
  //changeTestModeButton.readButton();
  checkBluetoothConnection();
  delay(200);
}

void Robot::checkBluetoothConnection() {
  if (!bluetoothConnected && Serial.available()) {
    bluetoothConnected = true;
    Serial.println(F("[BLUETOOTH] Conectado"));
    FeedbackHelper::playBluetoothConnectedMelody();
  }
}

float Robot::measureDistanceAtAngle(int angle, int delayMs = 800) {
  servo.moveTo(angle);
  delay(delayMs);
  float distance = sonar.getDistanceCM();
  Serial.print("Distância em ");
  Serial.print(angle);
  Serial.print("°: ");
  Serial.println(distance);
  return distance;
}

void Robot::handleNoEscape() {
  FeedbackHelper::playErrorMelody();

  Serial.println(F("Recuando..."));
  mc.backward();
  delay(500);
  mc.stopAll();
  delay(300);

  Serial.println(F("Girando 180 graus..."));
  mc.move(FRONT_LEFT, FORWARD, 100);
  mc.move(FRONT_RIGHT, BACKWARD, 100);
  delay(500);
  mc.stopAll();
  delay(300);

  float newDistance = sonar.getDistanceCM();
  Serial.print(F("Nova distância à frente: "));
  Serial.println(newDistance);

  if (newDistance > OBSTACLE_DISTANCE_CM) {
    Serial.println(F("Caminho livre após giro!"));
    FeedbackHelper::playStartMelody();
    mc.forward();
    delay(500);
  } else {
    Serial.println(F("Ainda bloqueado. Parando."));
    FeedbackHelper::playErrorMelody();
    mc.stopAll();
  }
}

void Robot::runAutonomousMode() {
  float distance = sonar.getDistanceCM();
  Serial.print(F("Distância detectada: "));
  Serial.println(distance);

  if (distance <= OBSTACLE_DISTANCE_CM) {
    mc.stopAll();
    delay(100);
    Serial.println(F("Indo para trás!"));
    mc.backward();
    delay(300);
    mc.stopAll();
    FeedbackHelper::playObstacleMelody();

    int direction = decideEscapeDirection();
    delay(500);

    switch (direction) {
      case -1:
        Serial.println(F("Desviando para esquerda."));
        FeedbackHelper::playLeftMelody();
        mc.move(FRONT_LEFT, FORWARD, 100);
        mc.move(FRONT_RIGHT, FORWARD, 200);
        delay(500);
        noActionCount = 0;
        break;
      case 1:
        Serial.println(F("Desviando para direita."));
        FeedbackHelper::playRightMelody();
        mc.move(FRONT_RIGHT, FORWARD, 100);
        mc.move(FRONT_LEFT, FORWARD, 200);
        delay(500);
        noActionCount = 0;
        break;
      case 2:
        Serial.println(F("Indo para frente."));
        FeedbackHelper::playStartMelody();
        mc.moveAll(FORWARD, 160);
        delay(400);
        noActionCount = 0;
        break;
      default:
        noActionCount++;
        Serial.print(F("Tentativas sem sucesso: "));
        Serial.println(noActionCount);

        if (noActionCount >= MAX_ATTEMPTS) {
          Serial.println(F("Muitas tentativas. Executando plano de escape..."));
          handleNoEscape();
          noActionCount = 0;
        } else {
          Serial.println(F("Sem rota livre. Aguardando nova tentativa..."));
          FeedbackHelper::playErrorMelody();
        }
        break;
    }
    mc.stopAll();
    delay(300);
  } else {
    mc.forward();
  }
}

int Robot::decideEscapeDirection() {
  float leftDist = measureDistanceAtAngle(180);
  float frontDist = measureDistanceAtAngle(90);
  float rightDist = measureDistanceAtAngle(0);

  servo.moveTo(90);
  delay(100);

  int decistion = 0;

  if (frontDist > OBSTACLE_DISTANCE_CM) return 2;
  if (frontDist > OBSTACLE_DISTANCE_CM && (frontDist > leftDist || frontDist > rightDist)) return 2;
  if (leftDist > OBSTACLE_DISTANCE_CM && leftDist > rightDist) return -1;
  if (rightDist > OBSTACLE_DISTANCE_CM && rightDist > leftDist) return 1;

  return 0;
}

void Robot::runBluetoothMode() {
  if (Serial.available()) {
    char command = Serial.read();
    Serial.print("Comando recebido: ");
    Serial.println(command);

    switch (command) {
      case 'F': mc.forward(); break;
      case 'B': mc.backward(); break;

      case 'L':
        mc.move(FRONT_LEFT, STOP);
        mc.move(FRONT_RIGHT, FORWARD);
        delay(250);
        mc.stopAll();
        break;

      case 'R':
        mc.move(FRONT_LEFT, FORWARD);
        mc.move(FRONT_RIGHT, STOP);
        delay(250);
        mc.stopAll();
        break;

      case 'A':
        mc.move(FRONT_LEFT, BACKWARD);
        mc.move(FRONT_RIGHT, FORWARD);
        break;

      case 'D':
        mc.move(FRONT_LEFT, FORWARD);
        mc.move(FRONT_RIGHT, BACKWARD);
        break;
      case 'V':
        mc.move(FRONT_LEFT, BACKWARD, 100);  // roda esquerda mais lenta
        mc.move(FRONT_RIGHT, FORWARD, 200);  // roda direita normal
        delay(100);

        break;
      case 'J':
        mc.move(FRONT_LEFT, FORWARD, 200);    // roda esquerda normal
        mc.move(FRONT_RIGHT, BACKWARD, 100);  // roda direita mais lenta
        delay(100);
        break;

      case 'S':
        mc.stopAll();
        break;

      default:
        Serial.println("Comando inválido.");
        break;
    }
  }
}