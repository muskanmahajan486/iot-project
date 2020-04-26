#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

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

//======================flow====================
byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;

float calibrationFactor = 4.5;
volatile byte pulseCount;

unsigned int frac;
float flowRate;
unsigned int flowMilliLitres;
float totalMilliLitres;
unsigned long oldTime;

//==================wifi=====================
String WIFI_SSID = "danC0k";
String WIFI_PASS = "danu12345";

String host = "http://192.168.43.60"; //IP PC
String url = "/api/v1/data?";
String postData;


void setup() {
  Serial.begin(115200);
  
  //wifi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  //temp
  sensors_1.begin(); 
  sensors_2.begin(); 
  
  //flow
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

}

void loop() {
  
//======================flow========================
//  if((millis() - oldTime) > 1000) 
//  {
//
//    detachInterrupt(sensorInterrupt);
//    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
//    oldTime = millis();
//    flowMilliLitres = (flowRate / 60) * 1000;
//    totalMilliLitres += flowMilliLitres;
//
//    
////    unsigned int frac;
////   
////    Serial.print("Flow rate: ");
////    Serial.print(int(flowRate));  /
////    Serial.print(".");            
////    frac = (flowRate - int(flowRate)) * 10;
////    Serial.print(frac, DEC) ;     
////    Serial.print("L/min");
////    Serial.print("  Current Liquid Flowing: ");   
////    Serial.print(flowMilliLitres);
////    Serial.print("mL/Sec");
////
////    Serial.print("  Output Liquid Quantity: ");          
////    Serial.print(totalMilliLitres);
////    Serial.println("mL");
//
//    pulseCount = 0;
//   
//    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
//  }
 
// ===================presure===============
  x = analogRead(A0);
  v = x*(5.0/1023.0);
  kpa = ((v/5.0)-0.04)/0.0012858;
  
//======================temp================
  sensors_1.requestTemperatures(); 
  Celcius_1=sensors_1.getTempCByIndex(0);
  Fahrenheit_1=sensors_1.toFahrenheit(Celcius_1);

  sensors_2.requestTemperatures(); 
  Celcius_2=sensors_2.getTempCByIndex(0);
  Fahrenheit_2=sensors_2.toFahrenheit(Celcius_2);
  
  
  Serial.print("C1 : ");
  Serial.print(Celcius_1);
  Serial.print("F1 : ");
  Serial.print(Fahrenheit_1);

  Serial.print("\t C2 : ");
  Serial.print(Celcius_2);
  Serial.print("F2 : ");
  Serial.println(Fahrenheit_2);
  
  Serial.print("kPa= ");
  Serial.println(kpa);
       
//
//  Serial.print("ml/s= ");
//  Serial.println(flowMilliLitres);

  //send_data(kpa, flowMilliLitres, Celcius_1, Celcius_2 );
  delay(200);
  
}


void pulseCounter()
{
  pulseCount++;
}

void send_data(float pres, float flow, float temp1, float temp2){
  WiFiClient client;
  if (!client.connect(host, 5000)) {
    Serial.println("connection failed");
    return;
  }
  
  url += "data1=";
  url += "1";
  url += "&data2=";
  url += "2";
  url += "&data3=";
  url += "3";
  url += "&data4=";
  url += "4";
  Serial.print("Requesting URL: ");
  Serial.println(url);
 
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
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
