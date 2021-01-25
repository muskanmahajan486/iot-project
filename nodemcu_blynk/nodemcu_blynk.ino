#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


// You should get Auth Token in the Blynk App.

char auth[] = "U593hrI-ELBdSJBygFvAwdmLzQEzEAb-";
char ssid[] = "Redmi Note 9 Pro";
char pass[] = "enterprise";

BlynkTimer timer;
WidgetLCD lcd_widget(V0);
void setup()
{
  // Debug console

  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  lcd_widget.clear();
  lcd_widget.print(0, 0, "TOPAN gile");

  // Setup a function to be called every second
 
}

void loop()
{
  Blynk.run();
}
