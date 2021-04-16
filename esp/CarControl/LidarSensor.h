#include <Wire.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure;

#define TCAADDR 0x70

bool isLeftSensorInitialized = true;
bool isLeftCenterSensorInitialized = true;
bool isRightCenterSensorInitialized = true;
bool isRightSensorInitialized = true;

class LidarSensor{
  public:
  /*
    VCC - 3v3
    GND - GND
    SCL - D1 (GPIO5)
    SDA - D2 (GPIO4)
 */
  void InitializeSensors(){
    Serial.println("---Start Initialize Sensors---");

    delay(1000);
    tcaSelect(0);
    if (!lox.begin()) {
        Serial.println(F("test"));
    }

    tcaSelect(7);
    if (!lox.begin()) {
        Serial.println(F("Failed to boot Left Pin 7 sensor"));
        isLeftSensorInitialized = false;
    }

    tcaSelect(6);
    if (!lox.begin()) {
        Serial.println(F("Failed to boot Left Center Pin 6 sensor"));
        isLeftCenterSensorInitialized = false;
    }

    tcaSelect(5);
    if (!lox.begin()) {
        Serial.println(F("Failed to boot Right Center Pin 5 sensor"));
        isRightCenterSensorInitialized = false;
    }

    tcaSelect(4);
    if (!lox.begin()) {
        Serial.println(F("Failed to boot Right Pin 4 sensor"));
        isRightSensorInitialized = false;
    }

    Serial.println("---End Initialize---");
  }

  void ReadSensors(int *sensors) {
    tcaSelect(7);
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4 && isLeftSensorInitialized) {  // phase failures have incorrect data
      sensors[0] = measure.RangeMilliMeter;
    }

    tcaSelect(6);
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4 && isLeftCenterSensorInitialized) {  // phase failures have incorrect data
      sensors[1] = measure.RangeMilliMeter;
    }

    tcaSelect(5);
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4 && isRightCenterSensorInitialized) {  // phase failures have incorrect data
      sensors[2] = measure.RangeMilliMeter;
    }

    tcaSelect(4);
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4 && isRightSensorInitialized) {  // phase failures have incorrect data
      sensors[3] = measure.RangeMilliMeter;
    }
  }

  void tcaSelect(uint8_t i) {
      if (i > 7) return;

      Wire.beginTransmission(TCAADDR);
      Wire.write(1 << i);
      Wire.endTransmission();
  }
};
