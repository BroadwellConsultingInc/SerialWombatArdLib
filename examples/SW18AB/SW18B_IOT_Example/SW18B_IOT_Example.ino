#include "SerialWombat.h"
#include <myWiFi.h>  // Create your own file in one of the library or toolset directories that defines MY_WIFI_NETWORK and MY_WIFI_PASSWORD 
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>



SerialWombatChip swc;


const char* ssid = MY_WIFI_NETWORK;
const char* password = MY_WIFI_PASSWORD;
uint16_t temperature = 2500;
ESP8266WebServer server(80);
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128,64);

void setup() {
  Wire.begin(2,0);
  delay(250);
    
  swc.begin(Wire,0x6B);

  // put your setup code here, to run once:

   displayInit();


   ota_setup();
   MDNS.begin("SerialWombatTempSensor");
   server.on("/", handleRoot);
   server.begin();

}

uint8_t counter = 0;
void loop() {
  // put your main code here, to run repeatedly:
  ArduinoOTA.handle();
  server.handleClient();
  MDNS.update();
  
  delay(100);
 
  
  display.clearDisplay();

  display.setTextSize(1);
  display.println(WiFi.localIP());
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.setTextSize(4);
  uint32_t sum = 0;
  for (int i = 0; i < 100; ++i)
  {
    sum += swc.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_TEMPERATURE);
  }
  temperature = sum/100; //Average
  display.print(temperature/100); //Convert from 100's of deg C to deg c
  display.print(".");
  display.println(temperature%100);
  display.display();
}

void displayInit()
{
  // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the Screen
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("IOT Example");
  display.println(WiFi.localIP());
  display.display();
}

void handleRoot() {
  char tempString[80];
  sprintf(tempString,"%d.%d deg C\r\n",temperature/100,temperature%100);
  server.send(200, "text/plain", tempString);
}
