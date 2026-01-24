#include <SerialWombat.h>

SerialWombatChip sw;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(3000);

  sw.begin(Wire, sw.find(true));
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
}

void loop() {
  
}
