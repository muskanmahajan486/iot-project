/*
 * main.cpp

 *
 *  Created on: 2017年11月3日
 *      Author: I am xxx
 */
#include <arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "PDCN_yalong";
const char* password =  "lala123456789";
const char* mqttServer = "192.168.100.217";
const int mqttPort = 1883;
const char* mqttUser = "yourMQTTuser";
const char* mqttPassword = "yourMQTTpassword";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  Serial.begin(115200);
  WiFi.setHostname("node1");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client" )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }

  client.publish("esp/test", "Hello from ESP32");

}

void loop() {

  client.publish("/home/lala", "3.33");
  client.loop();
  delay(5000);
  //Serial.available()
}

