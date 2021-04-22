
                             ////////////    ////////////     ////////////      ///////      ////      //
                            ////////////    ////       //    ////           ////     ///    ////      //
                           ////            ////       //    ////           ////            ////      //
                          ///////         /////////////    ////////          ///////      ////////////
                         ///////         ///////          ////////                ///    ////////////
                        ////            ////  ////       ////           ////     ///    ////      //
                       ////            ////     ///     ////////////      ///////      ////      //
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ADC.h"
#include "setup.h"
#include "IRremote.h"
#include "config.h"
#include "motor.h"
#include "PWMServo.h"
#include "sensors.h"
#include "uart.h"
#include "GP2Y0A60SZLF.h"
#include "timeManagement.h"

void setup() {
  ADC_setup();
  Motor::init();
  Messaging::init();
  pinMode(BUTTON, INPUT); //button init
  digitalWrite(BUTTON, HIGH); //set pul-up resistor
}

Observer updSensorsObs = Observer(Sensors::updateSensors);
Observer logicObs = Observer(logic);
Observer motorObs = Observer(Motor::drive);
Observer messageParseObs = Observer(Messaging::parseIncomingMessage);
Observer messageSetObs = Observer(Messaging::setMessage);
Observer messageSendObs = Observer(Messaging::transmitMessage);
Observer ledObs = Observer(Motor::toggleLed);

unsigned long timer_current_millis = 0;
void loop() {

  Motor::speedR = 10;
  while (1) {
    timer_current_millis = millis();
    updSensorsObs.doCode(   10, timer_current_millis);
    logicObs.doCode(        10, timer_current_millis);
    motorObs.doCode(        30, timer_current_millis);
    messageParseObs.doCode( 30, timer_current_millis);
    messageSetObs.doCode(   30, timer_current_millis);
    messageSendObs.doCode(  30, timer_current_millis);
    ledObs.doCode(500, timer_current_millis);
    delay(1);
  }
}
////////////////////////////////

const int lengthListInter = 5;
boolean isBack = false;
boolean isTurn = false;

int tenth = 10; int stopCounter = 0;
int speedAr[20] = {0};
int Integral[lengthListInter][2];
int stopAr[2][3] = {0};
int l40; int r40; int l0; int r0; int c0;
int summSpeed = 0; int speedIndex = 0;
////////////////////////////////
void logic() {
  l40 = Sensors::left90;
  r40 = Sensors::right90;
  l0 = Sensors::left45;
  r0 = Sensors::right45;
  c0 = Sensors::center0;
  
  double inter = addInter(l40, r40);
  inter--;

  if(isBack || stopCounter > 9){
    stopCounter++;
    if(stopCounter < 10){
      Motor::speedR = 0;
    } else {
      if(stopCounter < 70){
        Motor::speedR = -2;
      } else {
        stopCounter = 0;
      }
    }
    
  }else{
    if(c0 < 100){
      Motor::writeServo(45*sign(inter));
      Motor::speedR = 2;

    }else{
      Motor::writeServo(0);
      if(r0 < 20 || r40 < 20) Motor::writeServo(-40);
      if(l0 < 20 || l40 < 20) Motor::writeServo(40);
      Motor::speedR = 2;      
    }
  }
  addSpeed(Motor::speedR);
  Motor::speedR += constrain((Motor::speedR - summSpeed/20), -15, 0);
  
  if(tenth == 70){
    isBack = isStop();
    tenth = 0;
  }
  tenth++;  
}

double addInter(int left, int right){
  for(int i = lengthListInter - 1; i > 0; i--){
    Integral[i][0] = Integral[i-1][0];
    Integral[i][1] = Integral[i-1][1];
  }
  Integral[0][0] = left;
  Integral[0][1] = right;
  int lInter = 0;
  int rInter = 0;
  for(int i = 0; i < lengthListInter; i++){
    lInter += Integral[i][0];
    rInter += Integral[i][1];
  }
  return rInter/lInter;
}

int sign(double value){
  if(value >= 0) return 1;
  else return -1;
}

void addSpeed(int speedCurrent){
  summSpeed -= speedAr[speedIndex];
  speedAr[speedIndex] = speedCurrent;
  summSpeed += speedCurrent;
  speedIndex++;
  if(speedIndex == 20)
    speedIndex = 0;
}

boolean isStop(){
  stopAr[1][0] = stopAr[0][0];
  stopAr[1][1] = stopAr[0][1];
  stopAr[1][2] = stopAr[0][2];
  stopAr[0][0] = l40;
  stopAr[0][1] = c0;
  stopAr[0][2] = r40;
  if((abs(stopAr[1][0] - stopAr[0][0]) == 0 && abs(stopAr[1][2] - stopAr[0][2]) == 0) ) return true;
    else return false;
}
