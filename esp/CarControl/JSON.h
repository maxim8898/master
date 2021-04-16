#include <ArduinoJson.h>

class JSON{
  public:
  
  String serialize(int left, int left_center, int right_center, int right, int speed, int temperature, int angle){
    String json = "";
    StaticJsonDocument<200> doc;
    doc["left"] = left;
    doc["left_center"] = left_center;
    doc["right_center"] = right_center;
    doc["right"] = right;
    doc["speed"] = speed;
    doc["temperature"] = temperature;
    doc["angle"] = angle;
    
    serializeJson(doc, json);
    return json;
  }
};
