#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);  //--> Server on port 80

//==========wifi================
#include <Wire.h>
#include "tampilanweb.h" //Isi halaman web HTML dengan javascript
//SSID dan Kata Sandi router WiFi
const char* ssid = "Odading Mang Oleh";
const char* password = "magelangan";

//==========serial==============
#include <SoftwareSerial.h>
SoftwareSerial ser(D1,D2);

int starts = 0;
int checksum = 0;
int bypass = 0;
String data_decode ="";


//==========sensor==============
String ppm_sensor;
String ph_sensor;
String ppm_sensor_send;
String ph_sensor_send;

bool lock = false;

//========== function website=========
void penampilData() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}


// ==========masukan data di sini..ganti nilai yg ad d dalam String ================= //
void handleRoot() {
  server.send(200, "text/html", MAIN_page); //--> Send web page
}
void penganbilData() {
                  // Arus                    Tegangan              Power
 String DataSensor = ph_sensor_send+";"+ppm_sensor_send;  // Sensor 1
 server.send(200, "text/plane", DataSensor); //Send ADC value only to client ajax request
}


void setup() {
  ser.begin(57600);

  //===============wifi setup===========
  Serial.begin(115200);
  delay(500);
  delay(500);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1);
    Serial.print(".");
  }
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  server.on("/", handleRoot);
  server.on("/", penampilData);      //Which routine to handle at root location. This is display page
  server.on("/readADC", penganbilData); //This page is called by java Script AJAX

  server.begin();                  //Start server
  Serial.println("HTTP server started");
  
}
 
void loop() {
  serial_get();

}

void serial_get(){

  String content = "";
  String content_check = "";
  
  if (ser.available()) {
    while (ser.available()) {
      
      char data = (char)ser.read() ;
//      Serial.print(starts);//
      if(data == '|'){
        checksum = 1;
      }
      //start
      if(data=='*')
        starts =1;
       //end
      else if(data =='#'){
        
//        /Serial.println("check");
        if(content != content_check){
          content = "";
          content_check = "";
        }
        data_decode = content;
        
        starts =0;
        checksum = 0;
        bypass =0;
        break;
      }
        
      else if(starts == 1){
        if(checksum == 0 ){
          content += data;
        }
        else{
          if( bypass == 0)
             bypass =1;
          else
            content_check += data; 
          
        }
      }
    }
  }
  Serial.println(data_decode);
  
  for (int i=0; i<data_decode.length(); i++){
      if(data_decode[i] == ','){
        lock= true;
      }
      else{
        if(lock==false){
          ppm_sensor +=data_decode[i];
        }
        else{
          ph_sensor += data_decode[i];
        }
      }
  }

  ppm_sensor_send = ppm_sensor;
  ph_sensor_send = ph_sensor;
  Serial.println(ppm_sensor);
  Serial.println("/");
  Serial.println(ph_sensor);
  
 
 delay(500);
  server.handleClient();
  //reset
  ppm_sensor = ""; 
  ph_sensor= "";
  content = "";
  content_check = "";
  lock= false;
}
