#include <SerialWombat.h>
/*

THIS EXAMPLE REQUIRES SW18AB Firmware Version 2.1.0 or Higher!

This example shows how to create a PulseOnChange pin configured to pulse when any of a set of
configured pins or data sources changes.

It also shows how the Pulse On Change mode can drive an output by providing a square wave
of a specified frequency and duty cycle when active.  This is useful for driving audio outputs.

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
Wire.begin();

sw.begin(0x6B);  //<<<<<<<<<<<<<<<<<   PUT YOUR I2C Address in here

Serial.begin(115200);

dbi19.begin(19);
dbi18.begin(18);
dbi17.begin(17);


poc.begin(8, // Output on pin 8
            SW_HIGH, 
            SW_LOW, 
            50, // 50mS pulse on change
            50, // Wait 50 ms after end of pulse before being able to trigger again
            1, // Or entries together, 
            1000, // 1000 uS PWM period
            0x8000); // 50% duty cycle
            
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
