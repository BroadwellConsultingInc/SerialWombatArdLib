#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip
SerialWombatServo ContinuousServo(sw);  // Declare a Servo on pin 2 of Serial Wombat sw
SerialWombatServo StandardServo(sw);   // Declare a Servo on pin 3 of Serial Wombat sw

// A video tutorial is available which explains this example in detail at: https://youtu.be/WiciAtS1ng0
void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Basic Servo Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  if (sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_SERVO))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  ContinuousServo.attach(2, 500, 2500, true); // Initialize a servo on pin 2, 500uS minimum pulse, 2500 us Maximum pulse, reversed
  StandardServo.attach(3);  // Initialize a servo on pin 3 using Arduino equivalent default values.  Use a different pin on SW18AB, since 3 is SDA

}

void loop() {

  // put your main code here, to run repeatedly:

  ContinuousServo.write(30);       // Takes a number from 0 to 180
  StandardServo.write16bit(5500);  // Takes a number from 0 to 65535:  Higher resolution
  delay(5000);
  ContinuousServo.write(140);
  StandardServo.write16bit(50000);
  delay(5000);

}
