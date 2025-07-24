#include "Robot.h"
#include "FeedbackHelper.h"

ROBOT_MODE Robot::robotMode = BLUETOOTH;
bool Robot::testModeIsActive = false;
FeedbackManager* FeedbackHelper::feedback = nullptr;

static unsigned long lastInterruptTime = 0;
volatile bool testModeChanged = false;

void onTestModeInterrupt()
{
  unsigned long currentTime = millis();
  if (currentTime - lastInterruptTime > 200)
  {
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
      feedback(&buzzer) {};

static void Robot::changeRobotMode()
{
  testModeIsActive = !testModeIsActive;
  if (robotMode == AUTONOMOUS && testModeIsActive)
  {
    Serial.println(F("AUTONOMOUS_TEST"));
    FeedbackHelper::playTestModeMelody();
  }
  else if (robotMode == BLUETOOTH && testModeIsActive)
  {
    Serial.println(F("BLUETOOTH_TEST"));
    FeedbackHelper::playTestModeMelody();
    RobotTest::informationControlledTests();
  }
  else if (robotMode == BLUETOOTH && !testModeIsActive)
  {
    Serial.println(F("BLUETOOTH"));
    FeedbackHelper::playBluetoothMelody();
  }
  else if (robotMode == AUTONOMOUS && !testModeIsActive)
  {
    Serial.println(F("AUTONOMOUS"));
    FeedbackHelper::playAutonomousMelody();
  }
}

void Robot::defineCallbacks()
{
  sliderMode.setEventCallback(setAutonomousMode, 0);
  sliderMode.setEventCallback(setBluetoothMode, 1);
  sliderMode.setFeedbackCallback(FeedbackHelper::playAutonomousMelody, 0);
  sliderMode.setFeedbackCallback(FeedbackHelper::playBluetoothMelody, 1);

  // changeTestModeButton.setEventCallback(changeRobotMode);
}

static void Robot::setBluetoothMode()
{
  robotMode = BLUETOOTH;
}

static void Robot::setAutonomousMode()
{
  robotMode = AUTONOMOUS;
};

void Robot::initialize()
{
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
  if (testModeIsActive)
  {
    robotTesting.informationControlledTests();
  }
  FeedbackHelper::playStartMelody();
}

void Robot::controller()
{
  switch (robotMode)
  {
  case BLUETOOTH:
    if (testModeIsActive)
    {
      robotTesting.runControlledTests();
    }
    else
    {
      runBluetoothMode();
    }
    break;
  case AUTONOMOUS:
    if (testModeIsActive)
    {
      robotTesting.runRoutineTests();
    }
    else
    {
      runAutonomousMode();
    }
  case CONTROLED_ACTIONS:
    controledFunction.trigger();
  default:
    runBluetoothMode();
    break;
  }

  if (!controlMode)
  {
    sliderMode.getStateFeedback();
    sliderMode.updateAlways();

    if (testModeChanged)
    {
      testModeChanged = false;
      changeRobotMode();
    }
  }
  // changeTestModeButton.readButton();
  checkBluetoothConnection();
  delay(200);
}

void Robot::checkBluetoothConnection()
{
  if (!bluetoothConnected && Serial.available())
  {
    bluetoothConnected = true;
    Serial.println(F("[BLUETOOTH] Conectado"));
    FeedbackHelper::playBluetoothConnectedMelody();
  }
}

float Robot::measureDistanceAtAngle(int angle, int delayMs = 800)
{
  servo.moveTo(angle);
  delay(delayMs);
  float distance = sonar.getDistanceCM();
  Serial.print("Distância em ");
  Serial.print(angle);
  Serial.print("°: ");
  Serial.println(distance);
  return distance;
}

void Robot::handleNoEscape()
{
  FeedbackHelper::playErrorMelody();

  Serial.println(F("Recuando..."));
  backward();
  delay(500);
  stop();
  delay(300);

  Serial.println(F("Girando 180 graus..."));
  turnPILeft();
  delay(500);
  stop();
  delay(300);

  float newDistance = sonar.getDistanceCM();
  Serial.print(F("Nova distância à frente: "));
  Serial.println(newDistance);

  if (newDistance > OBSTACLE_DISTANCE_CM)
  {
    Serial.println(F("Caminho livre após giro!"));
    FeedbackHelper::playStartMelody();
    forward();
    delay(500);
  }
  else
  {
    Serial.println(F("Ainda bloqueado. Parando."));
    FeedbackHelper::playErrorMelody();
    turnAroundLeft();
    Serial.println(F("Rodando aleatóriamente..."));
    delay(2000);
    stop();
  }
}

void Robot::runAutonomousMode()
{
  float distance = sonar.getDistanceCM();
  Serial.print(F("Distância detectada: "));
  Serial.println(distance);

  if (distance <= OBSTACLE_DISTANCE_CM)
  {
    mc.stopAll();
    delay(100);
    Serial.println(F("Indo para trás!"));
    backward();
    delay(300);
    stop();
    FeedbackHelper::playObstacleMelody();

    ROBOT_ACTIONS direction = decideEscapeDirection();
    delay(500);
    servo.moveTo(90);
    switch (direction)
    {
    case R_LEFT:
      Serial.println(F("Desviando para esquerda."));
      FeedbackHelper::playLeftMelody();
      left();
      delay(500);
      noActionCount = 0;
      break;
    case R_RIGHT:
      Serial.println(F("Desviando para direita."));
      FeedbackHelper::playRightMelody();
      right();
      delay(500);
      noActionCount = 0;
      break;
    case R_FORWARD:
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

      if (noActionCount >= MAX_ATTEMPTS)
      {
        Serial.println(F("Muitas tentativas. Executando plano de escape..."));
        handleNoEscape();
        noActionCount = 0;
      }
      else
      {
        Serial.println(F("Sem rota livre. Aguardando nova tentativa..."));
        FeedbackHelper::playErrorMelody();
        Serial.println(F("Rodando aleatóriamente..."));
        turnAroundRight();
        delay(2000);
      }
      break;
    }
    stop();
    delay(300);
  }
  else
  {
    forward();
    noActionCount = 0;
  }
}

ROBOT_ACTIONS Robot::decideEscapeDirection()
{
  float leftDist = measureDistanceAtAngle(180);

  if (leftDist > OBSTACLE_DISTANCE_CM)
    return R_LEFT;

  float frontDist = measureDistanceAtAngle(90);
  if (frontDist > OBSTACLE_DISTANCE_CM)
    return R_FORWARD;

  float rightDist = measureDistanceAtAngle(0);
  if (rightDist > OBSTACLE_DISTANCE_CM)
    return R_RIGHT;

  servo.moveTo(90);
  delay(100);

  return;
}

void Robot::runBluetoothMode()
{
  if (Serial.available())
  {
    char command = Serial.read();
    Serial.print("Comando recebido: ");
    Serial.println(command);

    switch (command)
    {
    case 'F':
      forward();
      break;
    case 'B':
      backward();
      break;
    case 'L':
      left();
      break;
    case 'R':
      right();
      break;
    case 'A':
      turnAroundLeft();
      break;
    case 'D':
      turnAroundRight();
      break;
    case 'V': // Não sei se entendi esse, coloquei pra virar 180 graus pela esquerda
      turnPILeft();
      break;
    case 'J': // Não sei se entendi esse, coloquei pra virar 180 graus pela direita
      turnPIRight();
      break;
    case 'S':
      stop();
      break;
    case 'Q':
      diagonalLeft();
      break;
    case 'P':
      diagonalRight();
      break;
    case 'G':
      backDiagonalLeft();
      break;
    case 'H':
      backDiagonalRight();
      break;
    default:
      Serial.println("Comando inválido.");
      break;
    }
  }
}
void Robot::backward()
{
  mc.backward();
};
void Robot::forward()
{
  mc.move( FRONT_LEFT,FORWARD,  constrain(200, 105, 255));
  mc.move( FRONT_RIGHT,FORWARD, constrain(200, 81, 255));
  delay(300);
};
void Robot::left()
{
  mc.move(FRONT_LEFT, STOP);
  mc.move(FRONT_RIGHT, FORWARD);
  delay(335);
  mc.stopAll();
};
void Robot::right()
{
  mc.move(FRONT_LEFT, FORWARD);
  mc.move(FRONT_RIGHT, STOP);
  delay(325);
  mc.stopAll();
};
void Robot::stop()
{
  mc.stopAll();
};
void Robot::diagonalLeft()
{
  mc.move(FRONT_LEFT, FORWARD, 130);
  mc.move(FRONT_RIGHT, FORWARD, 255);
  delay(800);
};
void Robot::diagonalRight()
{
  mc.move(FRONT_LEFT, FORWARD, 255);
  mc.move(FRONT_RIGHT, FORWARD, 110);
  delay(800);
};
void Robot::backDiagonalLeft()
{
  mc.move(FRONT_LEFT, BACKWARD, 130);
  mc.move(FRONT_RIGHT, BACKWARD, 245);
  delay(800);
};
void Robot::backDiagonalRight()
{
  mc.move(FRONT_LEFT, BACKWARD, 255);
  mc.move(FRONT_RIGHT, BACKWARD, 105);
  delay(800);
};
void Robot::turnAroundLeft()
{
  mc.move(FRONT_LEFT, BACKWARD);
  mc.move(FRONT_RIGHT, FORWARD);
};
void Robot::turnAroundRight()
{
  mc.move(FRONT_LEFT, FORWARD);
  mc.move(FRONT_RIGHT, BACKWARD);
};
void Robot::turnPILeft()
{
  mc.move(FRONT_LEFT, BACKWARD, 150); // roda esquerda mais lenta
  mc.move(FRONT_RIGHT, FORWARD, 200); // roda direita normal
  delay(390);                         //
  mc.stopAll();
}
void Robot::turnPIRight()
{
  mc.move(FRONT_LEFT, FORWARD, 200);   // roda esquerda normal
  mc.move(FRONT_RIGHT, BACKWARD, 150); // roda direita mais lenta
  delay(380);
  mc.stopAll();
}