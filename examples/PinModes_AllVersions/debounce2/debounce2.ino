#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat

SerialWombatDebouncedInput blueButton(sw);

SerialWombatDebouncedInput redButton(sw);
SerialWombatButtonCounter redCounter(redButton);

SerialWombatDebouncedInput greenButton(sw);
SerialWombatButtonCounter greenCounter(greenButton);

SerialWombatServo servo(sw);

#define SERVO_PIN 2
#define RED_BUTTON_PIN 0
#define GREEN_BUTTON_PIN 1
#define BLUE_BUTTON_PIN 3  // Set this to some other value on SW18AB, as 3 is an I2C pin

// This example is explained in a video tutorial at: https://youtu.be/R1KM0J2Ug-M

long int servoPosition = 90;
void setup() {
  // put your setup code here, to run once:

    Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port, This SerialWombat's address is 6C.

	// Check for proper Serial Wombat version and pin mode, set error handler.  Not necessary, but useful for debugging problems
	errorChecking();  

  
   redButton.begin(RED_BUTTON_PIN);
   greenButton.begin(GREEN_BUTTON_PIN);
   blueButton.begin(BLUE_BUTTON_PIN);

   redCounter.begin(&servoPosition, 1,500,2000,5,250,5000,10,100);
   redCounter.lowLimit = 0;
   redCounter.highLimit = 180;

   greenCounter.begin(&servoPosition, -1, 500,2000,-5,250,5000,-10,100);
   greenCounter.lowLimit = 0;
   greenCounter.highLimit = 180;

   servo.attach(SERVO_PIN,true);  //Servo on serial wombat pin 2, reverse direction
}

void loop() {
  
     redCounter.update();
     greenCounter.update();
    if (blueButton.digitalRead())
    {
      servoPosition = 90;
    }
     servo.write(servoPosition);
    Serial.println(servoPosition);

    delay(50);
 
}

int errorChecking()
{
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial

	if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}

	if (!sw.isPinModeSupported(SerialWombatPinMode_t::PIN_MODE_ANALOGINPUT))
	{
		Serial.println("The Analog Pin mode didn't respond as expected.  Verify proper operation with Wombat Panel.");
		if (sw.isSW08())
		{
			Serial.println("You may need to load a different firmware image to your SW8B board.");
		}
		while (1)
		{
			delay(100);
		}

	}

	return(0);
}

