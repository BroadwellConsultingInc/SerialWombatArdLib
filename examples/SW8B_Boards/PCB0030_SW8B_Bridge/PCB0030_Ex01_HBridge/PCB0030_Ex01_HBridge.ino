#include <SerialWombat.h>

PCB0030_Bridge bridgeBoard;


void setup() {
#ifdef ARDUINO_ESP8266_GENERIC
  Wire.begin(2, 0); // ESP-01 - SDA GPIO2, SCL GPIO0
#else
  Wire.begin();
#endif

  Serial.begin(115200);

  delay(500);

  bridgeBoard.begin(bridgeBoard.find(true));
  bridgeBoard.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if(!bridgeBoard.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
}

void loop() {
  bridgeBoard.hBridge45.writePublicData(0);
  bridgeBoard.hBridge67.writePublicData(32768);
  delay(2000);
  bridgeBoard.hBridge45.writePublicData(32768);
  bridgeBoard.hBridge67.writePublicData(32768);
  delay(1000);
  bridgeBoard.hBridge45.writePublicData(65535);
  bridgeBoard.hBridge67.writePublicData(32768);
  delay(2000);
  bridgeBoard.hBridge45.writePublicData(32768);
  bridgeBoard.hBridge67.writePublicData(32768);
  delay(1000);
  bridgeBoard.hBridge45.writePublicData(32768);
  bridgeBoard.hBridge67.writePublicData(0);
  delay(2000);
  bridgeBoard.hBridge45.writePublicData(32768);
  bridgeBoard.hBridge67.writePublicData(32768);
  delay(1000);
   bridgeBoard.hBridge45.writePublicData(32768);
  bridgeBoard.hBridge67.writePublicData(65535);
  delay(2000);
  bridgeBoard.hBridge45.writePublicData(32768);
  bridgeBoard.hBridge67.writePublicData(32768);
  delay(1000);
  
}
