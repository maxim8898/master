#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include "BlynkControl.h"

const int motorSpeedDefault = 1500;
const int angleDefault = 90;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

Servo myservo;
Servo motor;
BlynkControl BlControl;

int motorSpeed = 0;
int angle = 0;

void setup() {
  Serial.begin(9600);
  myservo.attach(0);
  myservo.write(angleDefault);
  
  motor.attach(2, 544, 2400);
  motor.writeMicroseconds(motorSpeedDefault);
  delay(3000);
  
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();

  int motorSpeed = motorSpeedDefault + BlControl.GetAcceleration() * BlControl.GetPowerPercentage();
  int angle = angleDefault + BlControl.GetAngle();
  
  motor.writeMicroseconds(motorSpeed);
  if (angle >= 75 && angle <= 105) {
    myservo.write(angle);
  }
}
