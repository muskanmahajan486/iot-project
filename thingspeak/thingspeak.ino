#include <WiFi.h>
#include "ThingSpeak.h"
int value;
int ChannelID = 490525;    
int jantung = 4 ;

const char* host    = "192.168.43.21" ; 
const char* ssid     = "dancuk"; // Your SSID (Name of your WiFi)
const char* password = "danuandrean"; //Your Wifi password


// setting API ThingSpeak
const char* server = "api.thingspeak.com";
 String WriteAPIKey = " IBE0RS26SSPFC5ER" ;                    
 String ReadApiKey = " NSIDW8R640WV7C4B " ;

WiFiClient client;
void setup()
{
   WiFi.begin("dancuk","danuandrean");
   Serial.begin(115200);              //inisialiasi port serial utk debugging
   WiFi.mode(WIFI_STA);
   pinMode  (jantung,INPUT) ;
   ThingSpeak.begin(client);

        while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
         }
}

void loop(){
  
   int value= analogRead(jantung);
   value = value;
   Serial.println(value);
             if (client.connect(server,80)){
                Serial.println("connected");
                delay(1000);
            
                String  postStr = WriteAPIKey;
                postStr += "&field1=";
                postStr += String(value);
                postStr +="&field2=";
                postStr += String(value);
                postStr += "\r\n\r\n";
                
                client.print("POST /update HTTP/1.1\n");
                client.print("Host: api.thingspeak.com\n");
                client.print("Connection: close\n");
                client.print("X-THINGSPEAKAPIKEY: " +WriteAPIKey+ "\n");
                client.print("Content-Type: application/x-www-form-urlencoded\n");
                client.print("Content-Length: ");
                client.print( postStr.length());
                client.print("\n\n");
                client.print( postStr);
        }
   client.stop();
 
    if (value<=100){ 
      Serial.println ("WONGE SEHAT");}  
    else{
      Serial.println ("penyakiten");}
 delay (5000);
}

