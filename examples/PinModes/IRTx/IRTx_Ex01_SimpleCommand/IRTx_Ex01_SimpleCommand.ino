#include <SerialWombat.h>

SerialWombatChip sw;
SerialWombatIRTx irTx(sw);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(3000);

  sw.begin(Wire, sw.find(true));
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}

  irTx.begin(7,0xC7EA);  //Set up pin 7 as IRTx
  if (sw.isSW08())
  {
    irTx.enableSW8b38KHzWP6();  // Set up Pin 6 on SW08 as carrier
  }
  else if (sw.isSW18())
  {
      //Set up pin 9 on SW18AB as carrier
    SerialWombatFrequencyOutput_18AB fo(sw);
    fo.begin(9);
    fo.writePublicData(38000);
  }
}

void loop() {
  delay(5000);
  irTx.sendMessage(0x10,0xC7EA);  // Every 5000 ms send 0x10 to device 0xC7EA.  This is volume down on Jon's TCL TV
  
}
