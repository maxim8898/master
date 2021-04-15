#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include "BlynkControl.h"
#include "LidarSensor.h"
#include "HttpSensorClient.h"
#include "JSON.h"

const int motorSpeedDefault = 1500;
const int angleDefault = 90;
const String serverName = "http://192.168.31.161:3002/snapshots";

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
LidarSensor lidarSensor;
HttpSensorClient httpSensorClient;
JSON json;

AsyncWebServer server(80);
AsyncWebSocket ws("/sensors");
HTTPClient http;

int motorSpeed = 0;
int angle = 0;
String request;
int sensors[2];

void setup() {
  myservo.attach(0);
  myservo.write(angleDefault);
  
  motor.attach(2, 544, 2400);
  motor.writeMicroseconds(motorSpeedDefault);
  delay(3000);

  lidarSensor.InitializeSensors();
  Blynk.begin(auth, ssid, pass);

  server.addHandler(&ws);
  // Start server
  server.begin();
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  Blynk.run();

  int motorSpeed = motorSpeedDefault + BlControl.GetAcceleration() * BlControl.GetPowerPercentage();
  int angle = angleDefault + BlControl.GetAngle();
  
  motor.writeMicroseconds(motorSpeed);
  if (angle >= 75 && angle <= 105) {
    myservo.write(angle);
  }
  
  lidarSensor.ReadSensors(sensors);
  request = json.serialize("2021-04-14T19:00:39.812Z", sensors[0], 3, 4, sensors[1], motorSpeed, 25, angle);
  httpSensorClient.SendPostRequest(serverName, request);
  ws.textAll(String(request));
  Serial.println("Motor Speed= " + String(motorSpeed));
  Serial.println("Angle= " + String(angle));
  Serial.println("Sensor1= " + String(sensors[0]));
  Serial.println("Sensor2= " + String(sensors[1]));
  delay(100);
}
