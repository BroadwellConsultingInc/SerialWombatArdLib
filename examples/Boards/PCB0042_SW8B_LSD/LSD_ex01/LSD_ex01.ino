/*  This example shows how to use the Serial Wombat PCB0042 LSD to drive outputs 0 and 2 through 7
 *  And read the voltage feedback on output 1 (assumes the solder jumper is enabled for pin 1)
 *  
 *  It cycles through each output setting it to 25, 50, 75, and 100% duty cycle then turning it back off

     Video on PCB0042 LSD:

     TODO coming soon

     SerialWombatPWM_18AB pin mode documentation:

     TODO
     SerialWombatAnalogInput_18AB pin mode documentation:

     TODO
*/

#include <SerialWombat.h>

PCB0042_LSD_PWM swBoard;

//#define PCB0042_I2C_ADDRESS 0x60  // << COMMENT IN THIS LINE AND SET THE ADDRESS FOR YOUR BOARD!

#ifndef PCB0042_I2C_ADDRESS
#error "Comment in and set the Address define on line 22.  The default address is 0x60"
#endif

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("PCB0042 PWM output Example. "); 


  
  swBoard.begin(PCB0042_I2C_ADDRESS, true);  //  Initialize board and configure pin 1 to be an analog feedback.

   //Optional Error Checking Code starts here
  if (!swBoard.isSW08())
  {
    Serial.println("This Example requires a PCB0042 Remcon board");
    while (1) {
      delay(100);
    }
  }
  if (!swBoard.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  if ( !(swBoard.isPinModeSupported(PIN_MODE_PWM) &&  swBoard.isPinModeSupported(PIN_MODE_ANALOGINPUT)) )
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  swBoard.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  delay(1000);
}


uint8_t pin = 0;
void loop() {
	Serial.print("Input voltage is ");
	Serial.println(swBoard.readVin_mV());

	swBoard.outputArray[pin]->writePublicData(0x4000); // 25% duty cycle
	delay(500);
	swBoard.outputArray[pin]->writePublicData(0x8000); // 50% duty cycle
	delay(500);
	swBoard.outputArray[pin]->writePublicData(0xC000); // 75% duty cycle
	delay(500);
	swBoard.outputArray[pin]->writePublicData(0xFFFF); // 100% duty cycle
	delay(500);
	swBoard.outputArray[pin]->writePublicData(0); // 0% duty cycle

	++pin;
	if (pin == 1) {++pin;} // Using pin 1 for Vin analog input
	if (pin == 8) {pin = 0;} 

}
