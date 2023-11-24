#include <PS4Controller.h>
#include <Arduino.h>
#include <driver/ledc.h>

const int pinoPWM_frente = 27;
const int pinoPWM_tras = 16;


unsigned long lastTimeStamp = 0;
int AnalogLY = 0;
int AnalogLYcorrigido=0;

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
    Serial.println(AnalogLY);
  }
}

void onConnect() {
  Serial.println("Connected!.");
}

void onDisConnect() {
  Serial.println("Disconnected!.");
}

void setupPWM1() {
  ledcSetup(0, 5000, 8);
  ledcAttachPin(27, 0);
}

void setupPWM2() {
  ledcSetup(1, 5000, 8);
  ledcAttachPin(16, 1);
}

void setPWM1(int setpoint) {
  int pwmValue = map(setpoint, 6, 127, 0, 255);
  pwmValue = constrain(pwmValue, 0, 255);
  ledcWrite(0, pwmValue);
}

void setPWM2(int setpoint) {
  int pwmValue = map(setpoint, -6, -127, 0, 255);
  pwmValue = constrain(pwmValue, 0, 255);
  ledcWrite(1, pwmValue);
}

void setup() {
  Serial.begin(115200);
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();
  Serial.println("Ready.");
  setupPWM1();
  setupPWM2();
}

void loop() {
  AnalogLY = PS4.LStickY();
  if (AnalogLY > 0){
    setPWM1(AnalogLY);
    setPWM2(0);
  }

  if (AnalogLY < 0){
    setPWM2(AnalogLY);
    setPWM1(0);
  }
  
}
