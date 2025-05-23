#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip

SerialWombatProtectedOutput swpo(sw);
SerialWombatAnalogInput Feedback(sw);

// This example is explained in a video tutorial at: https://youtu.be/p8CO04C1q_Y

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial

  swpo.begin(3,1);  // Controlling pin 3.   Feedback from pin 1.
  Feedback.begin(1);  // Begin analog reading on pin 1
}


int i;
void loop() {
      if(swpo.isInSafeState())
     {
      Serial.println("Protected Output Fault Detected, Output set to Safe State!");
     }
     if (i & 0x01)
     {
        swpo.configure(PO_FAULT_IF_FEEDBACK_GREATER_THAN_EXPECTED,8000,10,SW_HIGH,SW_LOW);
        Serial.println("On");
     }
     else
     {
        swpo.digitalWrite(LOW);
        Serial.println("Off");
     }

     delay(100);
     Serial.print ("counts at drain: ");
     Serial.println(Feedback.readCounts());
     Serial.print(Feedback.readVoltage_mV());
     Serial.println(" mV");
     Serial.println();
     
     delay(3000);
     ++i;
}
