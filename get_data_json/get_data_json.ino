#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "dann";
const char* password = "danu12345";

void wifiConnecting(){
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Menghubungkan ke Access Point");
    for(int c=0;c<3;c++){
      Serial.print(" .");
      delay(1000);  
    }
    Serial.println();
  }
}

void setup () {
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Setting NodeMCU");
  delay(1000);
  wifiConnecting();
}
 
void loop() {
  Serial.print(" stay");
  String url = "http://192.168.43.60/parsedata/";
  if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;  
    http.begin(url);  
    int httpCode = http.GET();                                                       
    if (httpCode > 0) { 
      String payload = http.getString();
      Serial.println("HTTP Response Code : ");
      Serial.println(httpCode);
      Serial.println("HTTP Response Payload : ");
      Serial.println(payload);
      
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  const char* sensor = doc["id"];
      Serial.println(sensor);
    http.end();   
  }else
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("NodeMCU tidak terhubung ke Access Point");
    wifiConnecting();
  }
  delay(5000);
}
}
