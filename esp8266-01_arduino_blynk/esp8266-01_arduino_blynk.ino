#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#define ESP8266_BAUD 115200

char auth[] = "U593hrI-ELBdSJBygFvAwdmLzQEzEAb-";
char ssid[] = "Redmi Note 9 Pro"; //You can replace the wifi name to your wifi 

char pass[] = "enterprise";  //Type password of your wifi.

SoftwareSerial EspSerial(2, 3); // RX, TX
WidgetLCD lcd(V0);

ESP8266 wifi(&EspSerial);

void setup()
{
  
  Serial.begin(ESP8266_BAUD);
  EspSerial.begin(ESP8266_BAUD);
  Serial.println("AT");
  EspSerial.println("AT");
  delay(1000);
  if(EspSerial.find("OK")){
    Serial.println("okay");
  }
  
  Blynk.begin(auth, wifi, ssid, pass);
  lcd.clear();
  lcd.print(0, 0, "IoT");
}

void loop()
{
  Blynk.run();
}
