
#include "ESP8266WiFi.h"
#include <Wire.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#define I2CADDRESS 0x6B
 
const char* ssid = "gileadn2_4ghz";
const char* password = "413926413926413926413926CA";
 
WiFiServer wifiServer(4000);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(2,0);
  delay(1000);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
 
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());
 
  wifiServer.begin();

  OTA_init();
}
  

void loop() {
  // put your main code here, to run repeatedly:
 
  WiFiClient client = wifiServer.available();

  uint8_t tcpToI2cBuffer[8];
  uint8_t tcpToI2cBufferCount = 0;
  uint8_t i2cToTcpBuffer[8];
  uint8_t i2cToTcpBufferCount = 0;
  if (client) {
    
    while (client.connected()) {
      
      while (client.available()>0) {

        
       
        
        uint8_t c = client.read();
        Serial.write(c);
        
        if (tcpToI2cBufferCount == 0 && (c == ' ' || c == 'U'))
        {
          continue;
        }
        tcpToI2cBuffer[tcpToI2cBufferCount] = c;
        ++tcpToI2cBufferCount;

        if (tcpToI2cBufferCount >= 8)
        {
          Wire.beginTransmission(I2CADDRESS);
          Wire.write(tcpToI2cBuffer,8);
          Wire.endTransmission();
          tcpToI2cBufferCount = 0;
          delay(2);
          Wire.beginTransmission(I2CADDRESS);
          Wire.requestFrom(I2CADDRESS,8); // Request the transmitted two bytes from the two registers
          Wire.readBytes(i2cToTcpBuffer,8); // Reads the data from the register
          Wire.endTransmission();
          client.write(i2cToTcpBuffer,8);
          Serial.write(i2cToTcpBuffer,8);
        }
        
      }
      delay(10);
    }
 
    client.stop();
    Serial.println("Client disconnected");
 
  }
    ArduinoOTA.handle();
}
