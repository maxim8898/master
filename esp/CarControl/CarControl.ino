#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFiClient.h>
#include <Servo.h>
#include "LidarSensor.h"
#include "JSON.h"
#include "variables.h"

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

int motorSpeed = MOTOR_SPEED_DEFAULT;
int angle = SERVO_ANGLE_DEFAULT;
String request;
int sensors[4];

void setup() {
  myservo.attach(SERVO_PIN);
  myservo.write(SERVO_ANGLE_DEFAULT);
  
  motor.attach(MOTOR_PIN, 544, 2400);
  motor.writeMicroseconds(MOTOR_SPEED_DEFAULT);
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

  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.begin();
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, String((char*)data));
    const String action = doc["action"];

    if (action == "ArrowUp") {
      motorSpeed = MOTOR_SPEED_FORWARD;
    } else if (action == "ArrowDown") {
      motorSpeed = MOTOR_SPEED_BACK;
    } else if (action == "ArrowLeft") {
      angle = SERVO_ANGLE_LEFT;
    } else if (action == "ArrowRight") {
      angle = SERVO_ANGLE_RIGHT;
    } else if (action == "DefaultAngle") {
      angle = SERVO_ANGLE_DEFAULT;
    } else if (action == "DefaultSpeed") {
      motorSpeed = MOTOR_SPEED_DEFAULT;
    } else {
      angle = SERVO_ANGLE_DEFAULT;
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
 void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void loop() {
  motor.writeMicroseconds(motorSpeed);
  if (angle >= SERVO_ANGLE_RIGHT && angle <= SERVO_ANGLE_LEFT) {
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
