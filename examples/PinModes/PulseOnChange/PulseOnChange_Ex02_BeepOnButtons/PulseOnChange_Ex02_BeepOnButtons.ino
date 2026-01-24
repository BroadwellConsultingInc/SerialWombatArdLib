#include <SerialWombat.h>
/*

THIS EXAMPLE REQUIRES SW18AB Firmware Version 2.1.0 or Higher!

This example shows how to create a PulseOnChange pin configured to pulse when any of a set of
configured pins or data sources changes.

This example assumes a piezo is attached to Pin 8 (The kind of piezo without a built in driver)
through a resistor.  Pin 8 is chosen because this example requires minimal hardware
support (Analog, enhanced digital capability, etc.) and pin 8 has none of these things.

The pulse on change pin mode looks at pins 19,18, and 17 which are configured as debounced inputs.
When the value of any of those pins increases (the button is pressed) then a tone is generated.

Documentation for the SerialWombatPulseOnChange Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_pulse_on_change.html#details

*/

SerialWombatChip sw;
SerialWombatPulseOnChange poc(sw);
SerialWombatDebouncedInput dbi19(sw),dbi18(sw),dbi17(sw);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Pulse on Change multiple input Example");


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
  if (sw.isSW08() && !(sw.isPinModeSupported(PIN_MODE_PULSE_ON_CHANGE) && sw.isPinModeSupported(PIN_MODE_DEBOUNCE)))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

dbi19.begin(19);
dbi18.begin(18);
dbi17.begin(17);


poc.begin(8);  // Implied values active high, inactive low, 50mS pulse time, 50mS pulse off, OR entries, No PWM
            
poc.setEntryOnIncrease(0, // first entry (valid values are 0-7)
19);  // The data source we're monitoring for change
poc.setEntryOnIncrease(1, // first entry (valid values are 0-7)
18);  // The data source we're monitoring for change
poc.setEntryOnIncrease(2, // first entry (valid values are 0-7)
17);  // The data source we're monitoring for change

}


void loop() {

 //Nothing to do...  The Serial Wombat chip is handling this internally.
delay(1000);
}
