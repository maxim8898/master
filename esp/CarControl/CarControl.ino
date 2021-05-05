#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFiClient.h>
#include <Servo.h>
#include "LidarSensor.h"
#include "JSON.h"

const int motorSpeedDefault = 1500;
const int angleDefault = 90;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

Servo myservo;
Servo motor;
LidarSensor lidarSensor;
JSON json;

AsyncWebServer server(80);
AsyncWebSocket ws("/sensors");

int motorSpeed = 0;
int angle = 0;
String request;
int sensors[4];

void setup() {
  myservo.attach(0);
  myservo.write(angleDefault);
  
  motor.attach(2, 544, 2400);
  motor.writeMicroseconds(motorSpeedDefault);
  delay(3000);
  Serial.begin(115200);
  lidarSensor.InitializeSensors();
  WiFi.begin(ssid, pass);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nIP Address is: ");
  Serial.println(WiFi.localIP());

  server.addHandler(&ws);
  server.begin();
}

void loop() {
  int motorSpeed = motorSpeedDefault;
  int angle = angleDefault;
  
  motor.writeMicroseconds(motorSpeed);
  if (angle >= 75 && angle <= 105) {
    myservo.write(angle);
  }
  
  lidarSensor.ReadSensors(sensors);
  request = json.serialize(sensors[0], sensors[1], sensors[2], sensors[3], motorSpeed, 25, angle);
  ws.textAll(String(request));

  Serial.println("Motor Speed= " + String(motorSpeed));
  Serial.println("Angle= " + String(angle));
  Serial.println("Left Sensor= " + String(sensors[0]));
  Serial.println("Left Center Sensor= " + String(sensors[1]));
  Serial.println("Right Center Sensor= " + String(sensors[2]));
  Serial.println("Right Sensor= " + String(sensors[3]));
  delay(100);
}
