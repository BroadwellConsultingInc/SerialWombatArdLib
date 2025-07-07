#include <WiFi.h>
#include <Wire.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "SerialWombat.h"
#include "myWiFi.h"


const char* ssid = MY_WIFI_SSID;
const char* password = MY_WIFI_PASSWORD;
SerialWombatChip SWC;
uint8_t i2cAddress = 0;


WiFiServer wifiServer(14000);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Wire.begin();

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
  if (!MDNS.begin("SWBridge")) {   // Set the hostname to "SWBridge"
    Serial.println("Error setting up MDNS responder!");
    while(1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  i2cAddress = SWC.find(true);
  SWC.begin(Wire, i2cAddress);
}

uint8_t tx[8], rx[8], count;
void loop() {
  // put your main code here, to run repeatedly:

  WiFiClient client = wifiServer.available();

  uint8_t tcpToI2cBuffer[8];
  uint8_t tcpToI2cBufferCount = 0;
  uint8_t i2cToTcpBuffer[8];
  uint8_t i2cToTcpBufferCount = 0;
  if (client) {

    while (client.connected()) {

      while (client.available() > 0) {
        uint8_t x = client.read();
        Serial.write(x);
        if (count > 0)
        {
          tx[count] = x;
          ++count;
          if (count >= 8)
          {
            SWC.sendPacket(tx, rx);
            count = 0;
            Serial.write(rx, 8);
            client.write(rx, 8);
          }
        }
        else
        {
          if (x != 0x55 && x != 'x' && x != ' ')
          {
            tx[count] = x;
            ++count;
          }
        }


      }
      delay(0);

    }

    client.stop();
    Serial.println("Client disconnected");

  }
  ArduinoOTA.handle();
}
