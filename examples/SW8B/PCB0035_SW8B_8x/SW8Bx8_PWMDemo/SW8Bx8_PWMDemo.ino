#include <SerialWombat.h>
/*
This example shows creating 8 instances of the Serial Wombat class and 64 instances of the SerialWombatPWM_18AB class
to drive 64 PWMs at different frequencies on a SW8B PCB0035 8x board.  This board has 8 Serial Wombat 8B chips on
addresses 0x60 to 0x67, resulting in 64 total outputs.

This example sets each pin (00,01..07, 10,11,12..17, ...   76,77) to a frequency of
1000 * the tens digit of the pin, and 100 * the ones digit HZ.  This can be measured to determine that all
pins are being independently manipulated.

The SerialWombatPWM_18AB class applies to both the SW8B and SW18AB firmware.

The Serial Wombat Arduino C++ library uses references instead of pointers for most operations as
these are easier for beginners.  However, it is not possible to create an array of references in
C++.  The SerialWombatPWM_18AB class requires a reference to a Serial Wombat Chip when it is created.

As such I see no way around explictly declaring all 8 Serial Wombat chips and all
64 pwm instances as shown in this example.
An array of pointers is then made to the explictly declared objects, and used iteratively for
the begin and frequency set calls.

Beginners to C++ should note that this results in a -> operator to get from the array of
Serial Wombat chips or PWMs instead of the typical '.' operator.

If anyone knows of a more elegant solution that doesn't require changing the underlying library,
please send me an example at help@serialwombat.com

A video demonstrating this example is available at:
TODO

Documentation for the SerialWombatPWM_18AB class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_p_w_m__18_a_b.html
*/

SerialWombatChip sw0,sw1,sw2,sw3,sw4,sw5,sw6,sw7;  // Arrays of references are not allowed in C++, and the PWM Declaration requires a reference.

                                                  

SerialWombatChip* sw[8] = {&sw0,&sw1,&sw2,&sw3,&sw4,&sw5,&sw6,&sw7};

SerialWombatPWM_18AB p00(sw0),p01(sw0),p02(sw0),p03(sw0),p04(sw0),p05(sw0),p06(sw0),p07(sw0);
SerialWombatPWM_18AB p10(sw1),p11(sw1),p12(sw1),p13(sw1),p14(sw1),p15(sw1),p16(sw1),p17(sw1);
SerialWombatPWM_18AB p20(sw2),p21(sw2),p22(sw2),p23(sw2),p24(sw2),p25(sw2),p26(sw2),p27(sw2);
SerialWombatPWM_18AB p30(sw3),p31(sw3),p32(sw3),p33(sw3),p34(sw3),p35(sw3),p36(sw3),p37(sw3);
SerialWombatPWM_18AB p40(sw4),p41(sw4),p42(sw4),p43(sw4),p44(sw4),p45(sw4),p46(sw4),p47(sw4);
SerialWombatPWM_18AB p50(sw5),p51(sw5),p52(sw5),p53(sw5),p54(sw5),p55(sw5),p56(sw5),p57(sw5);
SerialWombatPWM_18AB p60(sw6),p61(sw6),p62(sw6),p63(sw6),p64(sw6),p65(sw6),p66(sw6),p67(sw6);
SerialWombatPWM_18AB p70(sw7),p71(sw7),p72(sw7),p73(sw7),p74(sw7),p75(sw7),p76(sw7),p77(sw7);

SerialWombatPWM_18AB* sw8bPins[8][8]= 
{
  {&p00,&p01,&p02,&p03,&p04,&p05,&p06,&p07},
 {&p10,&p11,&p12,&p13,&p14,&p15,&p16,&p17},
  {&p20,&p21,&p22,&p23,&p24,&p25,&p26,&p27},
  {&p30,&p31,&p32,&p33,&p34,&p35,&p36,&p37},
  {&p40,&p41,&p42,&p43,&p44,&p45,&p46,&p47},
  {&p50,&p51,&p52,&p53,&p54,&p55,&p56,&p57},
  {&p60,&p61,&p62,&p63,&p64,&p65,&p66,&p67},
  {&p70,&p71,&p72,&p73,&p74,&p75,&p76,&p77},
};
  

void setup() {

  Wire.begin();

  Serial.begin(115200);

  delay(100);
  for (int chip = 0; chip < 8; ++chip)
  {
      sw[chip]->begin(Wire, 0x60 + chip);
      sw[chip]->queryVersion();
      for (int pinOnChip = 0; pinOnChip < 8; ++pinOnChip)
      {
          sw8bPins[chip][pinOnChip]->begin(pinOnChip,0x8000);
          sw8bPins[chip][pinOnChip]->writeFrequency_Hz(chip*1000 + 100*pinOnChip);
      }
  }
}

void loop() {
  
}
