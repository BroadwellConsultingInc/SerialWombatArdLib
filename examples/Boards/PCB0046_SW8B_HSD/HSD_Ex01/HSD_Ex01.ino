/* This example shows how to turn a channel on and off on the Serial Wombat PCB0046 HSD Board
 *  and monitor the current feedback and diagnostic lines.
 */

#include "SerialWombat.h"
#define PCB0046_I2C_BASE_ADDRESS 0x60  // << COMMENT IN THIS LINE AND SET THE BASE (Even) ADDRESS FOR YOUR BOARD!  It uses this plus this + 1.
#ifndef PCB0046_I2C_BASE_ADDRESS
#error "Comment in and set the Address define below the main comment.  The default address is 0x60"
#endif

PCB0046_HSD_PWM hsd;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  delay(3000);
  int16_t beginResult = hsd.begin(PCB0046_I2C_BASE_ADDRESS);

  //Optional Error Checking Code starts here
  if (beginResult < 0)
  {
    Serial.println("Something went wrong initializing the board.  This Example requires a PCB0046 HSD board");
    Serial.print("Error number: ");Serial.println(beginResult);
    while (1) {
      delay(100);
    }
  }

  // Set up an error handler.  Need to do it for both Serial Wombat chips on the PCB0046
  hsd.sw0.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  hsd.sw1.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  hsd.selectCurrentFeedbackChannel(6);
}

void loop() {

  hsd.output6.writePublicData(0xFFFF); // Turn on the channel
   delay(10000);  //Wait 10 seconds


   //Read the current for the channel and get the diagnostics
  Serial.println(hsd.readCurrentFeedbackAverage_mA());
  if (hsd.readChip0to3IsFaulted())
  {
    Serial.print(" FAULT 0to3");
  }
  if (hsd.readChip4to7IsFaulted())
  {
    Serial.print (" FAULT 4to7");
  }
  Serial.println();


 
 
   
   hsd.output6.writePublicData(0);  // Turn off and wait 10 seconds
    delay(10000); 
     Serial.println(hsd.readCurrentFeedbackAverage_mA());
       Serial.println(hsd.readVin_mV());
 
}
