#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//Your Domain name with URL path or IP address with path
const char* serverName = "http://YourNetwork:3002/snapshots";

class HttpSensorClient{
  public:
  
  void SendPostRequest(String request){
    HTTPClient http;
      
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(request);
   
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
      
    // Free resources
    http.end();
  }
};