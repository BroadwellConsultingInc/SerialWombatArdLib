/*
Serial Wombat Chip connection to RGB LED from Elegoo 37 Sensor kit
 */
#include <SerialWombat.h>



SerialWombatChip SWC;
SerialWombatPWM red(SWC),green(SWC),blue(SWC);
void setup() {
  Wire.begin();
  SWC.begin(Wire,0x6B);
  red.begin(0);
  green.begin(1);
  blue.begin(2);
}

uint16_t redSetting = 0;
uint16_t blueSetting = 65535;
uint16_t greenSetting = 0;
void loop() {

  redSetting += 70;
  greenSetting += 50;
  //blueSetting =+ 500;

  //if (redSetting > 32768) redSetting = 0;  
  //if (greenSetting >32768) greenSetting = 0;
  //if (blueSetting > 32768) blueSetting = 0;
  //red.writePublicData(redSetting);
  //green.writePublicData(greenSetting);
  blue.writePublicData(blueSetting);
}
