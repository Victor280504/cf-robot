#include "RobotTest.h"
#include "FeedbackHelper.h"

RobotTest::RobotTest(MotorControl* mc, Buzzer* buzzer, ServoWrapper* servo, SensorUltrasonic* ultrasonic) {
  this->mc = mc;
  this->buzzer = buzzer;
  this->servo = servo;
  this->ultrasonic = ultrasonic;
};

RobotTest::RobotTest(MotorControl* mc, Buzzer* buzzer, ServoWrapper* servo, SensorUltrasonic* ultrasonic, int pauseDuration) {
  this->mc = mc;
  this->pauseBetweenTests = pauseDuration;
  this->buzzer = buzzer;
  this->servo = servo;
  this->ultrasonic = ultrasonic;
};

void RobotTest::runRoutineTests() {
  Serial.println(F("\n== Iniciando testes PWM simultâneos =="));
  buzzer->success();
  delay(1000);

  testForwardBoth();
  delay(1500);
  testBackwardBoth();
  delay(1500);

  testTurnLeftPWM();
  delay(1500);

  testTurnRightPWM();
  delay(1500);

  testTurnLeftInvert();
  delay(1500);

  testTurnRightInvert();
  delay(1500);

  testSoftTurnAroundRight();
  delay(1500);

  testSoftTurnAroundLeft();
  delay(1500);

  testUltrasonicDistance();
  delay(1500);

  testServoSweep();
  delay(1500);

  Serial.println(F("== Testes completos =="));
}

static void RobotTest::informationControlledTests() {
  Serial.println(F("=== Sistema de Testes Interativos via Bluetooth ==="));
  Serial.println(F("Comandos:"));
  Serial.println(F("1: Teste Frente"));
  Serial.println(F("2: Teste Ré"));
  Serial.println(F("3: Curva Esquerda"));
  Serial.println(F("4: Curva Direita"));
  Serial.println(F("5: Giro no Eixo Esquerda"));
  Serial.println(F("6: Giro no Eixo Direita"));
  Serial.println(F("7: Curva Ré Esquerda"));
  Serial.println(F("8: Curva Ré Direita"));
  Serial.println(F("9: Teste Motor M1"));

  Serial.println(F("A: Frente (Ambos PWM)"));
  Serial.println(F("B: Ré (Ambos PWM)"));
  Serial.println(F("C: Curva Esquerda PWM"));
  Serial.println(F("D: Curva Direita PWM"));
  Serial.println(F("E: Curva Invertida Esquerda"));
  Serial.println(F("F: Curva Invertida Direita"));
  Serial.println(F("G: Giro Suave Direita"));
  Serial.println(F("H: Giro Suave Esquerda"));
  Serial.println(F("I: Teste Servo Sweep"));
  Serial.println(F("J: Teste Sensor Ultrassônico"));
  Serial.println(F("K: Teste de Calibração linha reta"));
  Serial.println(F("L: Teste de Calibração M1"));
  Serial.println(F("M: Teste de Frente com tempo"));
  Serial.println(F("N: Teste de Roda Esquerda"));
  Serial.println(F("O: Teste de Roda Direita"));

  FeedbackHelper::playSuccessMelody();
}

void RobotTest::runControlledTests() {
  if (Serial.available()) {
    char cmd = Serial.read();  // ← sem ponteiro!
    Serial.print(F("Comando recebido: "));
    Serial.println(cmd);

    switch (cmd) {
      case '1': testForward(); break;
      case '2': testBackward(); break;
      case '3': testTurnLeft(); break;
      case '4': testTurnRight(); break;
      case '5': testRotateLeft(); break;
      case '6': testRotateRight(); break;
      case '7': testCurveLeft(); break;
      case '8': testCurveRight(); break;
      case '9': testM1(); break;

      case 'A': testForwardBoth(); break;
      case 'B': testBackwardBoth(); break;
      case 'C': testTurnLeftPWM(); break;
      case 'D': testTurnRightPWM(); break;
      case 'E': testTurnLeftInvert(); break;
      case 'F': testTurnRightInvert(); break;
      case 'G': testSoftTurnAroundRight(); break;
      case 'H': testSoftTurnAroundLeft(); break;
      case 'I': testServoSweep(); break;
      case 'J': testUltrasonicDistance(); break;
      case 'K': testForwardCalibrate(); break;
      case 'L': testForwardCalibrateM1(); break;
      case 'M': testForwardTiming(); break;
      case 'N': testLeftMotorPWM(); break;
      case 'O': testRightMotorPWM(); break;

      default:
        Serial.println(F("Comando inválido."));
        buzzer->error();
        break;
    }
  }
}

void RobotTest::testForwardBoth() {
  Serial.println(F("\n[1] Ambos para FRENTE:"));

  for (int pwm = 0; pwm <= 255; pwm += 25) {
    int currentPWM = (pwm == 250 ? 255 : pwm);
    Serial.print(F("PWM "));
    Serial.println(currentPWM);

    buzzer->beep(60);
    mc->move(FRONT_LEFT, FORWARD, currentPWM);
    mc->move(FRONT_RIGHT, FORWARD, currentPWM);
    delay(1000);
    mc->stopAll();
    delay(500);
  }
}

void RobotTest::testForwardCalibrate() {
  Serial.println(F("\n[1] Ambos para FRENTE != PWM :"));

  for (int pwm = 0; pwm <= 255; pwm += 25) {
    int currentPWM = (pwm == 250 ? 255 : pwm);
    Serial.print(F("PWM "));
    Serial.println(currentPWM);

    buzzer->beep(60);
    mc->move(FRONT_LEFT, FORWARD, currentPWM + 25);
    mc->move(FRONT_RIGHT, FORWARD, currentPWM);
    delay(1000);
    mc->stopAll();
    delay(500);
  }
}

void RobotTest::testLeftMotorPWM() {
  Serial.println(F("\n[1] LEFT PWM :"));

  for (int pwm = 0; pwm <= 255; pwm++) {
    int currentPWM = pwm;
    Serial.print(F("PWM "));
    Serial.println(currentPWM);
    buzzer->beep(60);
    mc->move(FRONT_LEFT, FORWARD, currentPWM);
    delay(1000);
    mc->stopAll();
    delay(500);
  }
}
void RobotTest::testRightMotorPWM() {
  Serial.println(F("\n[1] RIGHT PWM :"));

  for (int pwm = 0; pwm <= 255; pwm++) {
    int currentPWM = pwm;
    Serial.print(F("PWM "));
    Serial.println(currentPWM);
    buzzer->beep(60);
    mc->move(FRONT_RIGHT, FORWARD, currentPWM);
    delay(1000);
    mc->stopAll();
    delay(500);
  }
}

void RobotTest::testForwardCalibrateM1() {
  Serial.println(F("\n--- Testando: M1 Calibrate ---"));
  mc->move(FRONT_LEFT, FORWARD, 125);
  mc->move(FRONT_RIGHT, FORWARD, 100);
  delay(1560);
  mc->stopAll();
  delay(pauseBetweenTests);
}

void RobotTest::testForwardTiming() {
  Serial.println(F("\n[Teste: Comando M - Teste para frente com Tempo"));
  unsigned long start = millis();
  buzzer->beep(140);

  // Serial.print("[TESTE FRENTE PWM] PWM Esq: " + String(125 + (25 * i)) + " PWM Dir:" + String(100 + (25 * i)));
  buzzer->beep(180);
  delay(100);

  // 7000
  while (millis() - start < 2000) {
    mc->move(FRONT_LEFT, FORWARD, 105);
    mc->move(FRONT_RIGHT, FORWARD, 105);
    delay(100);
  }
  buzzer->beep(100);
  buzzer->beep(100);
  buzzer->beep(100);

  mc->stopAll();
  Serial.println(F("Fim do teste Timing iterativo."));
}

void RobotTest::testBackwardBoth() {
  Serial.println(F("\n[2] Ambos para TRÁS:"));

  for (int pwm = 0; pwm <= 255; pwm += 25) {
    int currentPWM = (pwm == 250 ? 255 : pwm);
    Serial.print(F("PWM "));
    Serial.println(currentPWM);

    buzzer->beep(80);
    mc->move(FRONT_LEFT, BACKWARD, currentPWM);
    mc->move(FRONT_RIGHT, BACKWARD, currentPWM);
    delay(1000);
    mc->stopAll();
    delay(500);
  }
}

void RobotTest::testTurnLeftPWM() {
  Serial.println(F("\n[3] Giro para ESQUERDA (A):"));

  for (int pwm = 0; pwm <= 255; pwm += 25) {
    int currentPWM = (pwm == 250 ? 255 : pwm);
    Serial.print(F("PWM "));
    Serial.println(currentPWM);

    buzzer->beep(100);
    mc->move(FRONT_LEFT, BACKWARD, currentPWM);
    mc->move(FRONT_RIGHT, FORWARD, currentPWM);
    delay(1000);
    mc->stopAll();
    delay(500);
  }
}

void RobotTest::testTurnRightPWM() {
  Serial.println(F("\n[4] Giro para DIREITA (D):"));

  for (int pwm = 0; pwm <= 255; pwm += 25) {
    int currentPWM = (pwm == 250 ? 255 : pwm);
    Serial.print(F("PWM "));
    Serial.println(currentPWM);

    buzzer->beep(120);
    mc->move(FRONT_LEFT, FORWARD, currentPWM);
    mc->move(FRONT_RIGHT, BACKWARD, currentPWM);
    delay(1000);
    mc->stopAll();
    delay(500);
  }
}

void RobotTest::testTurnLeftInvert() {
  Serial.println(F("\n[5] Giro invertido para ESQUERDA:"));

  for (int pwm = 0; pwm <= 255; pwm += 25) {
    int rightPWM = (pwm == 250 ? 255 : pwm);
    int leftPWM = 255 - rightPWM;

    Serial.print(F("PWM L:"));
    Serial.print(leftPWM);
    Serial.print(F(" | R:"));
    Serial.println(rightPWM);

    buzzer->beep(140);
    mc->move(FRONT_LEFT, BACKWARD, leftPWM);
    mc->move(FRONT_RIGHT, FORWARD, rightPWM);
    delay(1000);
    mc->stopAll();
    delay(500);
  }
}

void RobotTest::testTurnRightInvert() {
  Serial.println(F("\n[6] Giro invertido para DIREITA:"));

  for (int pwm = 0; pwm <= 255; pwm += 25) {
    int leftPWM = (pwm == 250 ? 255 : pwm);
    int rightPWM = 255 - leftPWM;

    Serial.print(F("PWM L:"));
    Serial.print(leftPWM);
    Serial.print(F(" | R:"));
    Serial.println(rightPWM);

    buzzer->beep(200);
    mc->move(FRONT_LEFT, FORWARD, leftPWM);
    mc->move(FRONT_RIGHT, BACKWARD, rightPWM);
    delay(1000);
    mc->stopAll();
    delay(500);
  }
}

void RobotTest::testSoftTurnAroundRight() {
  Serial.println(F("\n[Teste: Comando V - Curva suave para DIREITA]"));
  unsigned long start = millis();
  buzzer->beep(200);

  while (millis() - start < 10000) {
    mc->move(FRONT_LEFT, FORWARD, 255);
    mc->move(FRONT_RIGHT, BACKWARD, 100);
    delay(100);
  }

  mc->stopAll();
  Serial.println(F("Fim do teste V."));
}

void RobotTest::testSoftTurnAroundLeft() {
  Serial.println(F("\n[Teste: Comando J - Curva suave para ESQUERDA]"));
  unsigned long start = millis();
  buzzer->beep(140);

  while (millis() - start < 10000) {
    mc->move(FRONT_LEFT, BACKWARD, 100);
    mc->move(FRONT_RIGHT, FORWARD, 255);
    delay(100);
  }

  mc->stopAll();
  Serial.println(F("Fim do teste J."));
}

void RobotTest::testForward() {
  Serial.println(F("\n--- Testando: Frente ---"));
  for (int s = 0; s < 4; s++) {
    for (int d = 0; d < 3; d++) {
      Serial.print(F("[Frente] PWM: "));
      Serial.print(speeds[s]);
      Serial.print(F(" | Tempo: "));
      Serial.print(durations[d]);
      Serial.println(F("ms"));

      buzzer->beep(60);
      mc->move(FRONT_LEFT, FORWARD, speeds[s]);
      mc->move(FRONT_RIGHT, FORWARD, speeds[s]);
      delay(durations[d]);
      mc->stopAll();

      delay(pauseBetweenTests);
    }
  }
}

void RobotTest::testM1() {
  Serial.println(F("\n--- Testando: M1 ---"));
  mc->move(FRONT_LEFT, FORWARD, 255);
  mc->move(FRONT_RIGHT, FORWARD, 255);
  delay(1560);
  mc->stopAll();
  delay(pauseBetweenTests);
}

void RobotTest::testBackward() {
  Serial.println(F("\n--- Testando: Ré ---"));
  for (int s = 0; s < 4; s++) {
    for (int d = 0; d < 3; d++) {
      Serial.print(F("[Ré] PWM: "));
      Serial.print(speeds[s]);
      Serial.print(F(" | Tempo: "));
      Serial.print(durations[d]);
      Serial.println(F("ms"));

      buzzer->beep(80);
      mc->move(FRONT_LEFT, BACKWARD, speeds[s]);
      mc->move(FRONT_RIGHT, BACKWARD, speeds[s]);
      delay(durations[d]);
      mc->stopAll();

      delay(pauseBetweenTests);
    }
  }
}

void RobotTest::testTurnLeft() {
  Serial.println(F("\n--- Testando: Curva à Esquerda ---"));
  for (int d = 0; d < 3; d++) {
    Serial.print(F("[Curva Esquerda] PWM Esq: 100 | PWM Dir: 200 | Tempo: "));
    Serial.println(durations[d]);

    buzzer->beep(100);
    mc->move(FRONT_LEFT, FORWARD, 100);
    mc->move(FRONT_RIGHT, FORWARD, 200);
    delay(durations[d]);
    mc->stopAll();

    delay(pauseBetweenTests);
  }
}

void RobotTest::testTurnRight() {
  Serial.println(F("\n--- Testando: Curva à Direita ---"));
  for (int d = 0; d < 3; d++) {
    Serial.print(F("[Curva Direita] PWM Esq: 200 | PWM Dir: 100 | Tempo: "));
    Serial.print(durations[d]);
    Serial.println(F("ms"));

    buzzer->beep(120);
    mc->move(FRONT_LEFT, FORWARD, 200);
    mc->move(FRONT_RIGHT, FORWARD, 100);
    delay(durations[d]);
    mc->stopAll();
    delay(pauseBetweenTests);
  }
}

void RobotTest::testRotateLeft() {
  Serial.println(F("\n--- Testando: Giro no Eixo para Esquerda ---"));
  for (int s = 0; s < 4; s++) {
    Serial.print(F("[Giro Esquerda] PWM: "));
    Serial.print(speeds[s]);
    Serial.println(F(" | Tempo: 800ms"));

    buzzer->beep(140);
    mc->move(FRONT_LEFT, BACKWARD, speeds[s]);
    mc->move(FRONT_RIGHT, FORWARD, speeds[s]);
    delay(800);
    mc->stopAll();
    delay(pauseBetweenTests);
  }
}

void RobotTest::testRotateRight() {
  Serial.println(F("\n--- Testando: Giro no Eixo para Direita ---"));
  for (int s = 0; s < 4; s++) {
    Serial.print(F("[Giro Direita] PWM: "));
    Serial.print(speeds[s]);
    Serial.println(F(" | Tempo: 800ms"));

    buzzer->beep(160);
    mc->move(FRONT_LEFT, FORWARD, speeds[s]);
    mc->move(FRONT_RIGHT, BACKWARD, speeds[s]);
    delay(800);
    mc->stopAll();
    delay(pauseBetweenTests);
  }
}

void RobotTest::testCurveLeft() {
  Serial.println(F("\n--- Testando: Curva Ré à Esquerda ---"));
  for (int d = 0; d < 3; d++) {
    Serial.print(F("[Curva Ré Esquerda] PWM Esq: 200 | PWM Dir: 100 | Tempo: "));
    Serial.print(durations[d]);
    Serial.println(F("ms"));

    buzzer->beep(180);
    mc->move(FRONT_LEFT, BACKWARD, 200);
    mc->move(FRONT_RIGHT, BACKWARD, 100);
    delay(durations[d]);
    mc->stopAll();
    delay(pauseBetweenTests);
  }
}

void RobotTest::testCurveRight() {
  Serial.println(F("\n--- Testando: Curva Ré à Direita ---"));
  for (int d = 0; d < 3; d++) {
    Serial.print(F("[Curva Ré Direita] PWM Esq: 100 | PWM Dir: 200 | Tempo: "));
    Serial.print(durations[d]);
    Serial.println(F("ms"));

    buzzer->beep(200);
    mc->move(FRONT_LEFT, BACKWARD, 100);
    mc->move(FRONT_RIGHT, BACKWARD, 200);
    delay(durations[d]);
    mc->stopAll();
    delay(pauseBetweenTests);
  }
}

void RobotTest::testUltrasonicDistance() {
  Serial.println(F("\n--- Testando: Sensor Ultrassônico ---"));

  for (int i = 0; i < 5; i++) {
    float distance = ultrasonic->getDistanceCM();  // Supondo SensorUltrasonic* ultrasonic
    Serial.print(F("Leitura "));
    Serial.print(i + 1);
    Serial.print(F(": "));
    Serial.print(distance);
    Serial.println(F(" cm"));
    buzzer->beep(60);
    delay(1000);
  }

  Serial.println(F("Fim do teste do sensor."));
}

void RobotTest::testServoSweep() {
  Serial.println(F("\n--- Testando: Servo Sweep ---"));

  for (int angle = 0; angle <= 180; angle += 15) {
    Serial.print(F("Ângulo: "));
    Serial.println(angle);
    buzzer->beep(50);
    servo->moveTo(angle);  // Supondo que você passe o ServoWrapper por ponteiro na classe
    delay(500);
  }

  for (int angle = 180; angle >= 0; angle -= 15) {
    Serial.print(F("Ângulo: "));
    Serial.println(angle);
    buzzer->beep(50);
    servo->moveTo(angle);
    delay(500);
  }

  Serial.println(F("Fim do teste de servo."));
}