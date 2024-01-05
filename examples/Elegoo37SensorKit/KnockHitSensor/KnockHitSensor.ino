/* Serial Wombat 18AB chip connection to Elegoo Knock/Hit sensor from 37 sensor kit
   SCL,SDA,Ground,3.3V from Arduino to SerialWombat chip
   Ground,3.3V to Knock Switch
   Knock Switch Signal to SerialWombat chip pin 10
 */
 
 #include <SerialWombat.h>

 SerialWombatChip swc;

 SerialWombatDebouncedInput sensor(swc);
 void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  swc.begin(Wire,0x6b);

  sensor.begin(10,//Pin 10
                1, //1ms Debounce
                false, //Don't invert
                false);//Don't use pullup
  Serial.begin(115200);
  sensor.readTransitionsState(true);//Clear any transitions
}

void loop() {
  // put your main code here, to run repeatedly:
  sensor.readTransitionsState(false); 
  Serial.println(sensor.transitions);

}
