#include "SerialWombat.h"
/*
  This example shows how to generate varying frequencies with the Serial Wombat 18AB's frequency generator pin mode.  We will play "Twinkle Twinkle Little Star"


  A video demonstrating the use of the Frequency Generator Pin Mode is available at:
  TODO

  Documentation for the SerialWombatHighSpeedClock class is available at:
  https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_h_s_clock.htmll
*/

SerialWombatChip sw;
SerialWombatFrequencyOutput freqOutput(sw);
SerialWombat18ABOscillatorTuner oscTun(sw);

#define FREQUENCY_PIN 7  //Must be an enhanced digital capability pin.


#define NOTE_QUARTER 200
#define NOTE_HALF (NOTE_QUARTER * 2)
#define NOTE_SEPARATION 30

#define NOTE_C 523
#define NOTE_D 587
#define NOTE_E 659
#define NOTE_F 698
#define NOTE_G 784
#define NOTE_A 880


const uint16_t song[] =
{
  NOTE_C,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_C,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_G,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_G,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_A,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_A,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_G,  NOTE_HALF, 0, NOTE_SEPARATION,

  NOTE_F,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_F,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_E,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_E,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_D,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_D,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_C,  NOTE_HALF, 0, NOTE_SEPARATION,

  NOTE_G,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_G,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_F,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_F,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_E,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_E,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_D,  NOTE_HALF, 0, NOTE_SEPARATION,

  NOTE_G,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_G,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_F,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_F,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_E,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_E,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_D,  NOTE_HALF, 0, NOTE_SEPARATION,

  NOTE_C,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_C,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_G,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_G,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_A,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_A,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_G,  NOTE_HALF, 0, NOTE_SEPARATION,

  NOTE_F,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_F,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_E,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_E,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_D,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_D,  NOTE_QUARTER, 0, NOTE_SEPARATION,
  NOTE_C,  NOTE_HALF, 0, NOTE_SEPARATION,
  0, 10000, // Delay between replay
};

#define NUMBER_OF_NOTES (sizeof(song)/4)


void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("1Hz Blink Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
  if (sw.isSW04())
  {
    Serial.println("This Example is not supported on the Serial Wombat 4B chip.  An 8B or 18AB chip is required.");
    while (1) {
      delay(100);
    }
  }
  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  if (sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_FREQUENCY_OUTPUT))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  freqOutput.begin(FREQUENCY_PIN,
                   1000 // The maximum frequency we need to be able to generate.  Providing this parameter helps the Serial Wombat Timing Resource Manager make optimal decicisions for allocating hardware modules
                  ); //
}


int i = 0;
int nextNote_millis = 0;
void loop() {
  oscTun.update(); // Call this periodically to improve oscillator frequency accuracy.  See https://youtu.be/T2uBQM3s_JM


  if (millis() > nextNote_millis)
  {
    freqOutput.writePublicData(song[i * 2]);
    nextNote_millis = millis() + song[i * 2 + 1];
    ++ i;
    if (i >= NUMBER_OF_NOTES)
    {
      delay(2000);
      i = 0;
    }
  }

}
