#include <SerialWombat.h>

/*
This example shows reading the analog signal from a Linear Hall Sensor module that's part of the
Elegoo 37 sensor kit.

This code was created from the SerialWombatTemplate_I2C sketch.  All of the remaining code except
for this comment block was created using Automatic code generation from the WombatPanel application.

A video demonstrating this is avaialble at:
TODO

Connections:
Arduino providing voltage and I2C control to Serial Wombat 18AB chip  (Serial Wombat Address pin floating)
3.3V to Sensor
Sensor Analog output to Serial Wombat 18AB pin 19
Servo powered by USB battery, common ground with Serial Wombat Chip
Serial Wombat pin 15 to Servo Signal line

The Serial Wombat 18AB chip is configured to read an analog input on pin 19.

Pin 15 is configured to Servo Control.   The Scaled Output unit of pin 15 is configured
to get input from pin 19, and to use 2d Linear Lookup Table to convert the input to 3 servo
output positions.

Documentation for the  relevant classes is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_analog_input__18_a_b.html
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_servo__18_a_b.html
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_abstract_scaled_output.html

*/
SerialWombatChip sw;
//Put this line before setup()
SerialWombatAnalogInput_18AB Pin19_AnalogInput(sw); // Your serial wombat chip may be named something else than sw
//Put this line before setup()
SerialWombatServo_18AB Pin15Servo(sw); // Your serial wombat chip may be named something else than sw

void setup() {
#ifdef ARDUINO_ESP8266_GENERIC
  Wire.begin(2, 0); // ESP-01 - SDA GPIO2, SCL GPIO0
#else
  Wire.begin();
#endif

  Serial.begin(115200);

  delay(100);

  sw.begin(Wire, sw.find(true));



  //Add this line to  setup():
  Pin19_AnalogInput.begin(19, //Pin Number
                          64,  // Samples per Average
                          65408,//Filter Constant
                          AnalogInputPublicDataOutput::AnalogInputPublicDataOutput_Raw); //Public data output



  //Add this line to  setup():
  Pin15Servo.attach(15,//Pin
                    500, //Minimum Pulse Time
                    2500, //MaximumPulse time
                    false);  //Reverse//Put the interpolation table in user RAM.
  {
    uint16_t xytable[] = {
      0, 0,
      27000, 0,
      27001, 32768,
      37000, 32768,
      37001, 65535,
      65535, 65535,
    };

    sw.writeUserBuffer(0, (uint8_t*)xytable, 24);
  }

  Pin15Servo.Enable2DLookupOutputScaling(0);

  //put this line in setup.  Make this the last line after other
  // Output Scaling configurations for this pin
  Pin15Servo.writeScalingEnabled(true, //Enabled
                                 19); //DataSource


}

void loop() {

}
