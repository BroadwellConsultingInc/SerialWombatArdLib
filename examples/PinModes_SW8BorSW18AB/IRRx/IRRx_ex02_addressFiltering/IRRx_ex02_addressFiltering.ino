#include <SerialWombat.h>

SerialWombatChip sw;
SerialWombatIRRx irrx(sw);

#define IRRX_PIN 7
void setup() {
#ifdef ARDUINO_ESP8266_GENERIC
  Wire.begin(2, 0); // ESP-01 - SDA GPIO2, SCL GPIO0
#else
  Wire.begin();
#endif

  Serial.begin(115200);

  delay(500);

  sw.begin(Wire, sw.find(true));
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}


  irrx.begin(IRRX_PIN); // Default parameters:  NEC Mode, queue repeats, active low, 1000ms public data timeout, 0xFFFF public data default, no address filtering

}

void loop() {
  int receivedData = irrx.read();

  if (receivedData >=0)
  {
    char s[10];
    sprintf(s,"0x%X",irrx.readAddress());  // Read the 16 bit address of the transmitter, and convert it to a hex string
    Serial.println(s);
  }
  
}
