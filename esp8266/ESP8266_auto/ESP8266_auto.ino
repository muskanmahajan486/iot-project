#include <SoftwareSerial.h>

#define DEBUG true
SoftwareSerial softSerial(2,3);



#define SSID "danu"     // "SSID-WiFiname" 
#define PASS "danu12345"       // "password"

//pengiriman ke web thingspeak
#define IP "184.106.153.149"// thingspeak.com ip
String msg = "GET /update?key=9YS21NU0HY5YS1IKU"; //change it with your api key like "GET /update?key=Your Api Key"
 
void setup() 
{
  uint32_t baud =115200;
  Serial.begin(baud);
  softSerial.begin(baud);
  
  
  Serial.println("AT");
  softSerial.println("AT");
  delay(1000);
        if(softSerial.find("OK")){
          Serial.println("ok");
          connectWiFi();
        }
       else{
        Serial.println("gagal");
       }
} 
void loop(){
  
}
boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  softSerial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  softSerial.println(cmd);
  delay(5000);
  if(softSerial.find("OK")){
    Serial.println("OK");
    return true;    
  }else{
    return false;
  }
}
