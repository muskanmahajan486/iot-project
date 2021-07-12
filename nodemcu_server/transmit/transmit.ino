/* 
 author : danu Andrean
 note : transmiter

*/

#include <SoftwareSerial.h>
SoftwareSerial ser(2,3);

/*sensor initial*/
#include <SPI.h> 

float ph_calibration = 15.50;
const int ph_sensor = A1;
const int turbi_sensor = A0; 
int phsensorValue = 0; 
int turbisensorValue = 0;
unsigned long int phavgValue;
unsigned long int turbiavgValue; 
int phbuf[10],phtemp;
int turbibuf[10],turbitemp;
int phValue,ppm;

void setup() {
  ser.begin(57600);
  Serial.begin(9600);
}
 
void loop() {
  ph();
  turbi();
  
  serial_send(ppm,phValue);
}
void serial_send( int sensor1, int sensor2){
  
  String data_string = (String)sensor1+","+(String)sensor2;
  const char *data=data_string.c_str();

  ser.write("*");
  ser.write(data);/////
  ser.write("|");
  ser.write(data);
  ser.write("#");
  delay(1000);
}
void turbi(){
  for(int i=0;i<10;i++) 
  { 
    turbibuf[i]=analogRead(turbi_sensor);
    delay(30);
  }
  for(int i=0;i<9;i++)
  {
    for(int j=i+1;j<10;j++)
    {
      if(turbibuf[i]>turbibuf[j])
      {
        turbitemp=turbibuf[i];
        turbibuf[i]=turbibuf[j];
        turbibuf[j]=turbitemp;
      }
    }
  }
  turbiavgValue=0;
  for(int i=2;i<8;i++)
  turbiavgValue+=turbibuf[i];
  float turbiADC=(float)turbiavgValue/6;
  ppm = (turbiADC - 686.94)/-0.4189;
  Serial.print("PPM = ");
  Serial.println(ppm);
  delay(500);
}
void ph(){
  for(int i=0;i<10;i++) 
  { 
    phbuf[i]=analogRead(ph_sensor);
    delay(30);
  }
  for(int i=0;i<9;i++)
  {
    for(int j=i+1;j<10;j++)
    {
      if(phbuf[i]>phbuf[j])
      {
        phtemp=phbuf[i];
        phbuf[i]=phbuf[j];
        phbuf[j]=phtemp;
      }
    }
  }
  phavgValue=0;
  for(int i=2;i<8;i++)
    phavgValue+=phbuf[i];
    
  float pHVol=(float)phavgValue*5.0/1024/6;
  phValue = -5.70 * pHVol + ph_calibration;
  Serial.print("pH = ");
  Serial.print(phValue);
  Serial.print("  ");
}
