#include <SerialWombat.h>
/*
This example shows how create an output that pulses high when the Serial Wombat 18AB Chip
receives a communication packet.  This allows easy creation of a communication LED.

This example assumes an LED with appropriate current limiting resistor is attached to pin 8 
of the Serial Wombat 18AB chip.  Pin 8 is chosen because this example requires minimal hardware
support (Analog, enhanced digital capability, etc.) and pin 8 has none of these things.

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

void setup() {
  // put your setup code here, to run once:
Wire.begin();

sw.begin(0x6B);  //<<<<<<<<<<<<<<<<<   PUT YOUR I2C Address in here

Serial.begin(115200);

poc.begin  ( 8);
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
