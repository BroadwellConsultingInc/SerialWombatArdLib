#include <SerialWombat.h>
/*
  This example shows how create an output that pulses high when the Serial Wombat 8B or 18AB Chip
  receives a communication packet.  This allows easy creation of a communication LED.

  This example assumes an LED with appropriate current limiting resistor is attached to a pin of the Serial Wombat
  chip.

  The pulse on change pin mode looks at other pins or public 16-bit data to determine when to pulse.

  Data providers 0-63 correspond to physical pins on the Serial Wombat chip (only 0-19 are implemented on
  the SW18AB).  64 and up correspond to system values.

  Avaialble public data is enumerated in the firmware (not all values suppored by all chips):
  https://github.com/BroadwellConsultingInc/SerialWombat/blob/main/SerialWombatCommon/serialWombat.h

  which is also available as an enumeration in the Arduino Library SerialWombatDataSource enum:
  https://broadwellconsultinginc.github.io/SerialWombatArdLib/_serial_wombat_8h.html#aa93a00ab6275924ab4d521604780f6fa


  In this case we will use SW_DATA_SOURCE_PACKETS_RECEIVED (enum value 71) which changes each time a packet
  addressed to this Serial Wombat chip is received.

  We will pulse the LED high for 50mS with a delay of 50mS for low time (The default values).

  Each Pulse on Change pin has up to 4 state machine entries that can cause the pin to become active.  This is
  useful if you want to have more than one source capable of driving a pulse (such as multiple buttons leading to a
  feedback led or piezo).

  For this example we will only use one which will be configured to pulse on change of SW_DATA_SOURCE_PACKETS_RECEIVED.



  Documentation for the SerialWombatPulseOnChange Arduino class is available at:
  https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_pulse_on_change.html#details

*/

SerialWombatChip sw;
SerialWombatPulseOnChange poc(sw);

#define PULSE_ON_CHANGE_PIN 1

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Pulse on Change Example");


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
  if (sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_PULSE_ON_CHANGE))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  poc.begin  ( PULSE_ON_CHANGE_PIN);// Implied values active high, inactive low, 50mS pulse time, 50mS pulse off, OR entries, No PWM
  poc.setEntryOnChange(0, // first entry (valid values are 0-7)
                       (uint8_t)SerialWombatDataSource::SW_DATA_SOURCE_PACKETS_RECEIVED);  // The data source we're monitoring for change

}


void loop() {

  //Cause 3 packets to be sent
  for (int i = 0; i < 3; ++i)
  {
    uint16_t x = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_PACKETS_RECEIVED);
    Serial.println(x);
  }
  // then delay
  delay(1000);
}
