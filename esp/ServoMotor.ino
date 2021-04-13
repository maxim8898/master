#include <Servo.h>

Servo myservo;
Servo motor;

int angle = 0;

void setup() {
  Serial.begin(115200);
  myservo.attach(0);  // attaches the servo on GIO2 to the servo object
  motor.attach(2, 544, 2400);
  motor.writeMicroseconds(1500);
  delay(3000);
  angle = 0;
}

void loop() {
  Serial.println(angle);
  int serialInt = Serial.parseInt();
  if (serialInt == -1) {
    motor.writeMicroseconds(1570);
  }
  if (serialInt != 0) {
    angle = serialInt;
    // 70, 110 // 1388, 1611
    if (angle >= 75 && angle <= 105) {
      myservo.write(angle);
    }
  }
}
