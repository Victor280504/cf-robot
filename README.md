# 🤖 CFRobot - Robô Explorador Híbrido

| **Versão** | **Autor** | **Status** | **Licença** |
| :---: | :---: | :---: | :---: |
| `1.0.0` | Victor Emanuel | `Em Desenvolvimento` | `MIT` |

### Código fonte e classes de controle para o robô proposto como trabalho final da disciplina de Computação Física.

## 📜 Sobre o Projeto

Este repositório contém a biblioteca `CFRobot`, desenvolvida como parte do projeto final da disciplina de Computação Física. O objetivo foi criar um robô móvel com funcionalidades duplas: **navegação autônoma com desvio de obstáculos** e um **sistema de controle remoto híbrido** via Bluetooth.

A biblioteca encapsula toda a lógica de baixo nível para o gerenciamento de sensores e atuadores, fornecendo uma interface de alto nível simples e modular para controlar o robô.

## ✨ Funcionalidades (Requisitos)

| ID | Requisito | Status |
| :--- | :--- | :--- |
| ✅ | **Movimentação Essencial** | Completo |
| ✅ | **Controle Remoto (Bluetooth)** | Completo |
| ✅ | **Detecção de Obstáculos** | Completo |
| ✅ | **Manobra de Desvio Autônoma** | Completo |
| ✅ | **Seleção de Modos de Operação** | Completo |
| 🔄 | **Qualidade e Modularidade do Código** | Em Andamento |
| 🔄 | **Controle Remoto Híbrido Avançado** | Em Andamento |
| 🔄 | **Odometria Básica** | Em Andamento |
| 🔄 | **App de Controle Avançado** | Em Andamento |
| 📝 | **Design Físico do Robô** | A Fazer |

## 🛠️ Materiais

* **Microcontrolador:** Arduino Nano
* **Chassi:** Chassi 2WD
* **Atuadores:**
  * 2x Motores DC com roda
  * 1x Servo Motor (SG90 ou similar)
* **Driver:** 1x Driver de Motor L298N
* **Sensores:**
  * 1x Sensor Ultrassônico HC-SR04
* **Comunicação:**
  * 1x Módulo Bluetooth HC-05
* **Alimentação:**
  * 1x Bateria (ex: 2x 18650 Li-ion ou Power Bank)
* **Componentes de Interface:**
  * 1x Botão Deslizante (para seleção de modo)
  * 1x Botão de Ação (Push-button, para o modo de teste)
  * 1x Interruptor Geral
  * 4x Resistores (para os botões, e módulo bluetooth)

## 🔗 Dependências

Este projeto requer outras duas bibliotecas desenvolvidas durante a disciplina. Certifique-se de instalá-las também:

- [TFCFBot](https://github.com/Victor280504/tfcfbot): Biblioteca de controle de baixo nível para motores e sensores.
- [CFUtils](https://github.com/Victor280504/cfutils): Funções utilitárias e helpers.

## ⚙️ Instalação da Biblioteca

1.  **Instale as Dependências:** Baixe e instale as bibliotecas `TFCFBot` e `CFUtils` seguindo as instruções de cada repositório.
2.  **Clone o Repositório `CFRobot`:**
    ```bash
    git clone https://github.com/Victor280504/CFRobot.git
    ```
3.  **Mova para a Pasta de Bibliotecas:**
    Copie a pasta `CFRobot` para o diretório de bibliotecas da sua IDE Arduino:
    * **Windows:** `C:\Users\SeuUsuario\Documents\Arduino\libraries`
    * **Linux/macOS:** `~/Documents/Arduino/libraries/`
4.  **Reinicie a IDE:**
    Feche e abra a IDE do Arduino para que ela reconheça todas as bibliotecas.

## 🔌 Conexões e Pinos

O exemplo principal (`AutobotExample.ino`) utiliza a seguinte configuração de pinos. Adapte conforme sua montagem.

| Componente | Conexão no Arduino |
| :--- | :--- |
| **Motor Esquerdo** (via L298N) | `IN1` -> `D8`, `IN2` -> `D9`, `ENA` -> `D5` |
| **Motor Direito** (via L298N) | `IN3` -> `D6`, `IN4` -> `D7`, `ENB` -> `D11` |
| **Servo Motor** | Sinal -> `D10` |
| **Sensor Ultrassônico** | `TRIG` -> `D3`, `ECHO` -> `D4` |
| **Buzzer** (Feedback sonoro) | `+` -> `D13` |
| **Botão Deslizante** (Seletor) | Sinal -> `D12` |
| **Botão de Ação** | Sinal -> `D2` |
| **Bluetooth HC-05** | `TX` -> `RX` do Arduino, `RX` -> `TX` do Arduino |

## 🚀 Exemplo Rápido (Getting Started)

A biblioteca foi projetada para ser simples. O código principal no seu arquivo `.ino` pode ser tão limpo quanto este:

```cpp
// Arquivo: AutobotExample.ino

#include "Robot.h"

// --- Definição dos Pinos ---
// Motores [IN1, IN2, ENA]
uint8_t leftPins[3]{ 8, 9, 5 };
uint8_t rightPins[3]{ 6, 7, 11};

// Servo
uint8_t servoPin = 10;

// Sensor Ultrassônico [TRIG, ECHO]
uint8_t sonarPins[2]{ 3, 4 };

// Componentes de Interface
uint8_t buzzerPin = 13;
uint8_t sliderPin = 12; // Botão deslizante para modo
uint8_t buttonPin = 2;   // Botão de ação para modo de teste

// --- Instanciação do Robô ---
// O objeto 'autobot' recebe todos os pinos e gerencia o hardware.
Robot autobot(
  leftPins,
  rightPins,
  servoPin,
  sonarPins,
  buzzerPin,
  sliderPin,
  buttonPin);

void setup() {
  Serial.begin(9600);
  while (!Serial); // Espera a porta serial conectar

  // O método initialize configura todos os pinos e calibra os sensores.
  autobot.initialize();
}

void loop() {
  // O método controller() é o coração do robô.
  // Ele lê os sensores, verifica o modo de operação (autônomo ou remoto)
  // e executa as ações necessárias.
  autobot.controller();
}
```

##  UML - Diagrama de Classes

*Esta seção está reservada para o diagrama de classes do projeto, mostrando a relação entre `Robot`, `FeedbackManager`, `RobotTest` e outras classes auxiliares.*

## 📁 Estrutura do Projeto

```
/CFRobot/
├── examples/
│   └── AutobotExample/
│       └── AutobotExample.ino
├── src/
│   ├── Robot.h
│   ├── Robot.cpp
│   ├── RobotTest.h
│   ├── RobotTest.cpp
│   ├── FeedbackManager.h
│   ├── FeedbackManager.cpp
│   └── FeedbackHelper.h
├── library.properties
├── LICENSE
└── README.md
```

## 📱 Aplicativo de Controle (Android)

O controle remoto do robô é realizado por um aplicativo Android que se comunica via Bluetooth com o módulo HC-05.

* **Repositório do App:** [davsilvam/bluetooth-control-hc-05](https://github.com/davsilvam/bluetooth-control-hc-05)

## 🚀 Atalho para Entrega (Download Pack)

Para facilitar a avaliação e replicação do projeto, foi criado um pacote `.zip` contendo todas as bibliotecas necessárias (`CFRobot`, `TFCFBot`, `CFUtils`) e o arquivo `.apk` do aplicativo de controle para instalação direta no Android.

* **Link do Pacote:** **[TFCF package](https://drive.google.com/drive/folders/11ltiSsJKyzZRNag5bbc_1_RpNF3kYIGa?usp=sharing)**

## 🤝 Equipe e Colaboradores

Este projeto foi realizado com a colaboração dos seguintes integrantes:

* **Victor Emanuel**, **Natan Fonseca** e **Gustavo Alex** - *Desenvolvimento da Biblioteca, Firmware e Montagem do Robô, Testes e apresentação*
* **Davi Silva** e **Guilherme Bessa** - *Desenvolvimento do Aplicativo de Controle Android*

---