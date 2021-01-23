#include <SoftwareSerial.h>
#define DEBUG true

SoftwareSerial softSerial(8,9);
#include "ThingSpeak.h"
int ChannelID = 490525;

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

//  Variables

const int PulseWire = A5;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value.




#define SSID "dancuk"           // "SSID-WiFiname" 
#define PASS "danu12345"       // "password"


 
// setting API ThingSpeak
#define ip "184.106.153.149" 
const char* server = "api.thingspeak.com";
 String WriteAPIKey = "GET /update?key=IBE0RS26SSPFC5ER" ;                    
 volatile int myBPM;   


void setup() 
{
  uint32_t baud =115200;
  Serial.begin(baud);
  softSerial.begin(baud);
  
  
  Serial.println("AT");
  softSerial.println("AT");
  delay(5000);
        if(softSerial.find("OK")){
          connectWiFi();
        }
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold);             
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //print sebelum mendapatkan data pulse sensor  
  }
} 
void loop(){
 myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 
if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
 Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);                        // Print the value inside of myBPM. 
}
 delay(700);
            
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += ip;
  cmd += "\",80";
  softSerial.println(cmd);
  delay(2000);
  if(softSerial.find("Error")){
    return;
  }
  else{
    cmd =WriteAPIKey;
    cmd += "&field1=";   
    cmd += myBPM;
    cmd += "\r\n";
    Serial.print("AT+CIPSEND=");
    softSerial.print("AT+CIPSEND=");        //pengiriman data mengirimkan logika ">"
    Serial.println(cmd.length());
    softSerial.println(cmd.length());
  }
  if(softSerial.find(">")){
    Serial.print(cmd);
    softSerial.print(cmd);
  }
  else{
   Serial.println("AT+CIPCLOSE");
   softSerial.println("AT+CIPCLOSE");

  }
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
  }
}
