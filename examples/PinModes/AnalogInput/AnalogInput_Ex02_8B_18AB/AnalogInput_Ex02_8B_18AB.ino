#include <SerialWombat.h>


SerialWombatChip sw;
SerialWombatAnalogInput swAn0(sw);
SerialWombatServo swServo(sw);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Analog Input Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
  if(sw.isSW04())
  {
    Serial.println("This Example is not supported on the Serial Wombat 4B chip.  An 8B or 18AB chip is required.");
    while(1){delay(100);}
  }
  if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
  if(sw.isSW08() && !(sw.isPinModeSupported(PIN_MODE_ANALOGINPUT)&& sw.isPinModeSupported(PIN_MODE_SERVO)))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while(1){delay(100);}
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
                      //Optional Error handling code end 


  swAn0.begin(0);
  swServo.attach(1,1000,15000);

}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t pot = swAn0.readCounts();
  Serial.println(pot);
  swServo.write16bit(pot);
  delay(100);
}
