#include <PS4Controller.h>
#include <Arduino.h>
#include <driver/ledc.h>

//Motor 1 (Dianteira Esquerda)
const int pinPWM_D_E_F = 13;
const int pinPWM_D_E_R = 12;

//Motor 2 (Dianteira Direita)
const int pinPWM_D_D_F = 14;
const int pinPWM_D_D_R = 27;

//Motor 3 (Traseira Esquerda)
const int pinPWM_T_E_F = 16;
const int pinPWM_T_E_R = 17;

//Motor 4 (Traseira Direita)
const int pinPWM_T_D_F = 25;
const int pinPWM_T_D_R = 26;

//Variaveis dos comandos do Controle PS4
unsigned long lastTimeStamp = 0;
int AnalogLY = 0;
bool nullAnalogLY = false;
int AnalogRX = 0;
bool nullAnalogRX = false;
int L2;
int R2;
int R1;

//NotificaçãO dos comandos do Controle PS4 no serial Monitor
void notify() {
  char messageString[200];
  sprintf(messageString, "%4d,%4d,%4d,%4d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d",
  PS4.LStickX(),
  PS4.LStickY(),
  PS4.RStickX(),
  PS4.RStickY(),
  PS4.Left(),
  PS4.Down(),
  PS4.Right(),
  PS4.Up(),
  PS4.Square(),
  PS4.Cross(),
  PS4.Circle(),
  PS4.Triangle(),
  PS4.L1(),
  PS4.R1(),
  PS4.L2(),
  PS4.R2(),  
  PS4.Share(),
  PS4.Options(),
  PS4.PSButton(),
  PS4.Touchpad(),
  PS4.Charging(),
  PS4.Audio(),
  PS4.Mic(),
  PS4.Battery());

  if (millis() - lastTimeStamp > 50) {
    lastTimeStamp = millis();
    
    Serial.print("LY: ");
    Serial.println(AnalogLY);

    Serial.print("RX: ");
    Serial.println(AnalogRX);

    Serial.print("L2: ");
    Serial.println(L2);

    Serial.print("R2: ");
    Serial.println(R2);

    Serial.print("R1: ");
    Serial.println(R1);
  }
}

//Status de conexão do Controle PS4
void onConnect() {
  Serial.println("Connected!.");
}

void onDisConnect() {
  Serial.println("Disconnected!.");
}

//Função de inicialização de canais PWM
void setupPWM(uint8_t pinPWM, uint8_t canalPWM) {
  ledcSetup(canalPWM, 5000, 8);
  ledcAttachPin(pinPWM, canalPWM);
}

//Função de controle PWM
void setPWM(uint8_t canalPWM, int setpoint, int inputMin, int inputMax, int outputMin, int outputMax) {
  int pwmValue = map(setpoint, inputMin, inputMax, outputMin, outputMax);
  pwmValue = constrain(pwmValue, outputMin, outputMax);
  ledcWrite(canalPWM, pwmValue);
}

void setup() {
  
  //Inicializa serial
  Serial.begin(115200);

  //Inicializa Controle PS4
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();
  PS4.setLed(255, 0, 0);
  Serial.println("Ready.");
  

  //Inicializa canais PWM
    //Motor 1
    setupPWM(pinPWM_D_E_F, 0);
    setupPWM(pinPWM_D_E_R, 1);

    //Motor 2
    setupPWM(pinPWM_D_D_F, 2);
    setupPWM(pinPWM_D_D_R, 3);

    //Motor 3
    setupPWM(pinPWM_T_E_F, 4);
    setupPWM(pinPWM_T_E_R, 5);

    //Motor 4
    setupPWM(pinPWM_T_D_F, 6);
    setupPWM(pinPWM_T_D_R, 7);
  //  
}

void loop() {

  //Atribui comandos do controle para as variaveis
  AnalogLY = PS4.LStickY();
  AnalogRX = PS4.RStickX();
  L2 = PS4.L2();
  R2 = PS4.R2();
  R1 = PS4.R1();

  //Identifica posições nulas nos Analógicos do controle (centro)
  if (AnalogLY > -6 && AnalogLY < 6){
    nullAnalogLY = true;
  } else {nullAnalogLY = false;}

  if (AnalogRX > -6 && AnalogRX < 6){
    nullAnalogRX = true;
  } else {nullAnalogRX = false;}

  //Controles de direção e aceleração
  
    //4 Rodas para Frente (Analógico L para frente)
    if (AnalogLY > 6 && nullAnalogRX == true && R2 == 0 && L2 == 0 && R1 == 0) {
      
      //Motor 1
      setPWM(0, AnalogLY, 6, 127, 0, 255);
      setPWM(1, 0, 0, 0, 0, 0);

      //Motor 2
      setPWM(2, AnalogLY, 6, 127, 0, 255);
      setPWM(3, 0, 0, 0, 0, 0);

      //Motor 3
      setPWM(4, AnalogLY, 6, 127, 0, 255);
      setPWM(5, 0, 0, 0, 0, 0);

      //Motor 4
      setPWM(6, AnalogLY, 6, 127, 0, 255);
      setPWM(7, 0, 0, 0, 0, 0);
    }

    //4 Rodas para trás (Analógico L para trás)
    if (AnalogLY < -6 && nullAnalogRX == true && R2 == 0 && L2 == 0 && R1 == 0){

      //Motor 1
      setPWM(0, 0, 0, 0, 0, 0);
      setPWM(1, AnalogLY, -6, -127, 0, 255);

      //Motor 2
      setPWM(2, 0, 0, 0, 0, 0);
      setPWM(3, AnalogLY, -6, -127, 0, 255);

      //Motor 3
      setPWM(4, 0, 0, 0, 0, 0);
      setPWM(5, AnalogLY, -6, -127, 0, 255);

      //Motor 4
      setPWM(6, 0, 0, 0, 0, 0);
      setPWM(7, AnalogLY, -6, -127, 0, 255);
    }

    //Vira pra Direita (Analógico R para Direita)
    if (AnalogRX > 6 && nullAnalogLY == true && R2 == 0 && L2 == 0 && R1 == 0){
    
      //Motor 1
      setPWM(0, AnalogRX, 6, 127, 0, 255);
      setPWM(1, 0, 0, 0, 0, 0);

      //Motor 2
      setPWM(2, AnalogRX, 6, 127, 0, 150);
      setPWM(3, 0, 0, 0, 0, 0);

      //Motor 3
      setPWM(4, AnalogRX, 6, 127, 0, 255);
      setPWM(5, 0, 0, 0, 0, 0);

      //Motor 4
      setPWM(6, AnalogRX, 6, 127, 0, 150);
      setPWM(7, 0, 0, 0, 0, 0);
    }

    //Vira pra Esquerda (Analógico R para esquerda)
    if (AnalogRX < -6 && nullAnalogLY == true && R2 == 0 && L2 == 0 && R1 == 0){
      
      //Motor 1
      setPWM(0, AnalogRX, -6, -127, 0, 150);
      setPWM(1, 0, 0, 0, 0, 0);

      //Motor 2
      setPWM(2, AnalogRX, -6, -127, 0, 255);
      setPWM(3, 0, 0, 0, 0, 0);

      //Motor 3
      setPWM(4, AnalogRX, -6, -127, 0, 150);
      setPWM(5, 0, 0, 0, 0, 0);

      //Motor 4
      setPWM(6, AnalogRX, -6, -127, 0, 255);
      setPWM(7, 0, 0, 0, 0, 0);
    }

    //Giro 360 para direita (R2 + Analógico R para Direita)
    if (AnalogRX > 6 && nullAnalogLY == true && R2 == 1 && L2 == 0 && R1 == 0) {
      
      //Motor 1
      setPWM(0, AnalogRX, 6, 127, 0, 255);
      setPWM(1, 0, 0, 0, 0, 0);

      //Motor 2
      setPWM(2, 0, 0, 0, 0, 0);
      setPWM(3, AnalogRX, 6, 127, 0, 255);

      //Motor 3
      setPWM(4, AnalogRX, 6, 127, 0, 255);
      setPWM(5, 0, 0, 0, 0, 0);

      //Motor 4
      setPWM(6, 0, 0, 0, 0, 0);
      setPWM(7, AnalogRX, 6, 127, 0, 255);
    }

    //Giro 360 para esquerda (L2 + Analógico R para esquerda)
    if (AnalogRX < 6 && nullAnalogLY == true && R2 == 0 && L2 == 1 && R1 == 0) {
      
      //Motor 1
      setPWM(0, 0, 0, 0, 0, 0);
      setPWM(1, AnalogRX, -6, -127, 0, 255);

      //Motor 2
      setPWM(2, AnalogRX, -6, -127, 0, 255);
      setPWM(3, 0, 0, 0, 0, 0);

      //Motor 3
      setPWM(4, 0, 0, 0, 0, 0);
      setPWM(5, AnalogRX, -6, -127, 0, 255);

      //Motor 4
      setPWM(6, AnalogRX, -6, -127, 0, 255);
      setPWM(7, 0, 0, 0, 0, 0);
    }

    //Freio
    if (R1 == 1){
      setPWM(0, 0, 0, 0, 0, 0);
      setPWM(1, 0, 0, 0, 0, 0);
      setPWM(2, 0, 0, 0, 0, 0);
      setPWM(3, 0, 0, 0, 0, 0);
      setPWM(4, 0, 0, 0, 0, 0);
      setPWM(5, 0, 0, 0, 0, 0);
      setPWM(6, 0, 0, 0, 0, 0);
      setPWM(7, 0, 0, 0, 0, 0);
    }

  //
}
