/*
 *  author : Danu Andrean  
 *  create : 2020
 *  update : 2021
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>


int httpCode;
const char* ssid = "dann";
const char* password = "danu12345";
String url = "http://192.168.43.60:8000/api/respon";

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
  
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Setting NodeMCU");
  delay(1000);
  wifiConnecting();
}
 
void loop() {
  
  if (WiFi.status() == WL_CONNECTED) { 
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(1000);
    http.begin(client, url);
    httpCode = http.GET();
    
    if (httpCode > 0) { 
      String payload = http.getString();
      Serial.print("HTTP Response Code : ");
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
      const char* sensor = doc["btn1"];
      Serial.println(sensor);
         
    }
    http.end();
    delay(1000);
  }
  else{
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("NodeMCU tidak terhubung ke Access Point");
      wifiConnecting();
    }
  }
}
