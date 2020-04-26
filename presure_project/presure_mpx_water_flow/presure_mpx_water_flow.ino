#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//======================temp=================
float Celcius=0;
float Fahrenheit=0;

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



void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
 
  sensors.begin(); 
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
  

  if((millis() - oldTime) > 1000) 
  {

    detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;

    
//    unsigned int frac;
//   
//    Serial.print("Flow rate: ");
//    Serial.print(int(flowRate));  /
//    Serial.print(".");            
//    frac = (flowRate - int(flowRate)) * 10;
//    Serial.print(frac, DEC) ;     
//    Serial.print("L/min");
//    Serial.print("  Current Liquid Flowing: ");   
//    Serial.print(flowMilliLitres);
//    Serial.print("mL/Sec");
//
//    Serial.print("  Output Liquid Quantity: ");          
//    Serial.print(totalMilliLitres);
//    Serial.println("mL");

    pulseCount = 0;
   
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
 
// ===================presure===============
  x = analogRead(A0);
  v = x*(5.0/1023.0);
  kpa = ((v/5.0)-0.04)/0.0012858;
  
//======================temp================
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
  
  Serial.print("kPa= ");
  Serial.println(kpa);
       

  Serial.print("ml/s= ");
  Serial.println(flowMilliLitres);
 
  delay(200);
 
}


void pulseCounter()
{
  pulseCount++;
}
