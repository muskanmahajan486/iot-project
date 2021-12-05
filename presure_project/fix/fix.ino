/*
 *  author : Danu Andrean  
 *  create : 2020
 *  update : 2021
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS_1 D1
#define ONE_WIRE_BUS_2 D2
OneWire oneWire_1(ONE_WIRE_BUS_1);
OneWire oneWire_2(ONE_WIRE_BUS_2);

DallasTemperature sensors_1(&oneWire_1);
DallasTemperature sensors_2(&oneWire_2);

//======================temp=================
float Celcius_1=0;
float Celcius_2=0;
float Fahrenheit_1=0;
float Fahrenheit_2=0;



//======================presure===============
int x;
float v;
float kpa;

//==================wifi=====================
String WIFI_SSID = "dann";
String WIFI_PASS = "danu12345";

String host = "192.168.43.60";
String url = "/api/v1/data?";
String url_respon = "http://192.168.43.60:8000/api/respon";

String postData;


//======================flow====================
#define PULSE_PIN D8  //gpio4
#define LED_PIN D7    //gpio13

volatile long pulseCount=0;
float calibrationFactor = 4.5;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
float totalLitres;

unsigned long oldTime;
int detik;
int menit;
int jam;
void ICACHE_RAM_ATTR pulseCounter()
{
  pulseCount++;
}



void setup() {
  Serial.begin(115200);
  
  wifiConnecting();
  
  //temp
  sensors_1.begin(); 
  sensors_2.begin(); 
  
  //flow
  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0; 
  
  pinMode(PULSE_PIN, INPUT);
//  pinMode(PULSE_PIN, INPUT_PULLUP);
//  digitalWrite(PULSE_PIN, HIGH);   
  attachInterrupt(PULSE_PIN, pulseCounter, FALLING);

}

void loop() {
//  //======================flow========================
    if((millis() - oldTime) > 1000)    // Only process counters once per second
    {
  
//      detachInterrupt(PULSE_PIN);
//      flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
      oldTime = millis();
//      flowMilliLitres = (flowRate / 60) * 1000;
//      totalMilliLitres += flowMilliLitres;     
//      totalLitres = totalMilliLitres * 0.001;
//      unsigned int frac;   
//  //    Serial.print("flowrate: ");
//  //    Serial.print(int(flowRate));  // Print the integer part of the variable
//  //    Serial.print(".");             // Print the decimal point
//      frac = (flowRate - int(flowRate)) * 10;
//  //    Serial.print(frac, DEC) ;      // Print the fractional part of the variable
//  //    Serial.print("L/min");
//  //    Serial.print("  Current Liquid Flowing: ");             // Output separator
//  //    Serial.print(flowMilliLitres);
//  //    Serial.print("mL/Sec");
//  //    Serial.print("  Output Liquid Quantity: ");             // Output separator
//  //    Serial.print(totalLitres);
//  //    Serial.println("L");
//      //Serial.print(totalMilliLitres);
//      //Serial.println("mL");
//  
//      pulseCount = 0;
//  
//      attachInterrupt(PULSE_PIN, pulseCounter, FALLING);
//    } 
//   
//  // ===================presure===============
//    x = analogRead(A0);
//    v = x*(5.0/1023.0);
//    kpa = ((v/5.0)-0.04)/0.0012858;
//    
//  //======================temp================
//    sensors_1.requestTemperatures(); 
//    Celcius_1=sensors_1.getTempCByIndex(0);
//    Fahrenheit_1=sensors_1.toFahrenheit(Celcius_1);
//  
//    sensors_2.requestTemperatures(); 
//    Celcius_2=sensors_2.getTempCByIndex(0);
//    Fahrenheit_2=sensors_2.toFahrenheit(Celcius_2);
//    
//    
//    Serial.print("C1 : ");
//    Serial.print(Celcius_1);
//    Serial.print("  F1 : ");
//    Serial.print(Fahrenheit_1);
//  
//    Serial.print("\t C2 : ");
//    Serial.print(Celcius_2);
//    Serial.print("  F2 : ");
//    Serial.println(Fahrenheit_2);
//    
//    Serial.print("kPa= ");
//    Serial.println(kpa);
//         
//  
//    Serial.print("Output Liquid Quantity: ");             // Output separator
//    Serial.print(totalLitres);
//    Serial.println("L");
//  
//    //send_data(kpa, flowMilliLitres, Celcius_1, Celcius_2 );
//    
    detik ++;
  }
  if(menit == 1){
    send_data(kpa, totalLitres, Celcius_1, Celcius_2 );
    menit =0;
  }

  if(detik ==60){
    menit ++;
    detik = 0;
  }

  Serial.println(detik);
  Serial.println(menit);
  delay(1000);

  receive_data();
   
  
}


//void pulseCounter()
//{
//  pulseCount++;
//}


void wifiConnecting(){
  //wifi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void receive_data(){
  if (WiFi.status() == WL_CONNECTED) { 
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(1000);
    http.begin(client, url_respon);
    int httpCode = http.GET();
    
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
  }
  else{
    Serial.println("NodeMCU tidak terhubung ke Access Point");
    wifiConnecting();
    
  }
}

void send_data(float pres, float flow, float temp1, float temp2){
  WiFiClient client;
  
  while(1)
  {
    if (!client.connect(host, 8000)) {
      Serial.println("connection failed");
    }
    else{
      break;
    }
    
  }
  String urls = url;
  urls += "data1=";
  urls += pres;
  urls += "&data2=";
  urls += flow;
  urls += "&data3=";
  urls += temp1;
  urls += "&data4=";
  urls += temp2;
  
  Serial.print("Requesting URL: ");
  Serial.println(urls);
 
  // This will send the request to the server
  client.print(String("GET ") + urls + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
 
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
}
