// Serial Wombat Cookbook Recipe 0001 (SWCB_0001) - Add 18 PWM outputs to an Arduino Board
#include <SerialWombat.h>

SerialWombatChip sw;

//First 6 Will be High Res
SerialWombatPWM_18AB pwm1(sw);
SerialWombatPWM_18AB pwm2(sw);
SerialWombatPWM_18AB pwm3(sw);
SerialWombatPWM_18AB pwm4(sw);
SerialWombatPWM_18AB pwm5(sw);
SerialWombatPWM_18AB pwm6(sw);

//The rest will be 17uS Res
SerialWombatPWM_18AB pwm7(sw);
SerialWombatPWM_18AB pwm8(sw);
SerialWombatPWM_18AB pwm9(sw);
SerialWombatPWM_18AB pwm10(sw);
SerialWombatPWM_18AB pwm11(sw);
SerialWombatPWM_18AB pwm12(sw);
SerialWombatPWM_18AB pwm13(sw);
SerialWombatPWM_18AB pwm14(sw);
SerialWombatPWM_18AB pwm15(sw);
SerialWombatPWM_18AB pwm16(sw);
SerialWombatPWM_18AB pwm17(sw);
SerialWombatPWM_18AB pwm18(sw);
void setup() {

  Wire.begin(2, 0);  //begin(0,2) for ESP-01, begin() for most other boards.

  sw.begin(Wire,0x6B);  // Your I2C address may be different than 0x68 depending on your address resistor
  
  // put your setup code here, to run once:
  pwm1.begin(0);  //High Res
  pwm2.begin(1);  //High Res
  pwm3.begin(2);  //High Res
  // Pins 3 and 4 are I2C
  pwm4.begin(5);  // Low Res (not an enhanced digital pin)
  pwm5.begin(6);  // Low Res (not an enhanced digital pin)
  pwm6.begin(7);  // High Res
  pwm7.begin(8);  // Low Res (not an enhanced digital pin)
  pwm8.begin(9);  // High Res
  pwm9.begin(10); // High Res
  pwm10.begin(11);  //Low Res (all 6 high res timing units have been allocated);
  pwm11.begin(12);  //All additional are Low Res
  pwm12.begin(13);
  pwm13.begin(14);
  pwm14.begin(15);
  pwm15.begin(16);
  pwm16.begin(17);
  pwm17.begin(18);
  pwm18.begin(19);

  pwm1.writeFrequency_Hz(10000);
  pwm2.writeFrequency_Hz(5000);
  pwm3.writeFrequency_Hz(10000);
  pwm4.writeFrequency_Hz(100);
  pwm5.writeFrequency_Hz(200);
  pwm6.writeFrequency_Hz(20000);
  pwm7.writeFrequency_Hz(500);
  pwm8.writeFrequency_Hz(15000);
  pwm9.writeFrequency_Hz(12500);
  pwm10.writeFrequency_Hz(1000);
  pwm11.writeFrequency_Hz(1000);
  pwm12.writeFrequency_Hz(1000);
  pwm13.writeFrequency_Hz(1000);
  pwm14.writeFrequency_Hz(1000);
  pwm15.writeFrequency_Hz(1000);
  pwm16.writeFrequency_Hz(1000);
  pwm17.writeFrequency_Hz(1000);
  pwm18.writeFrequency_Hz(1000);

  pwm1.writeDutyCycle(3000); //4.5%
  pwm2.writeDutyCycle(6000);
  pwm3.writeDutyCycle(9000);
  pwm4.writeDutyCycle(12000);
  pwm5.writeDutyCycle(15000);
  pwm6.writeDutyCycle(18000);
  pwm7.writeDutyCycle(21000);
  pwm8.writeDutyCycle(24000);
  pwm9.writeDutyCycle(27000);
  pwm10.writeDutyCycle(30000);
  pwm11.writeDutyCycle(33000);
  pwm12.writeDutyCycle(36000);
  pwm13.writeDutyCycle(39000);
  pwm14.writeDutyCycle(42000);
  pwm15.writeDutyCycle(45000);
  pwm16.writeDutyCycle(48000);
  pwm17.writeDutyCycle(51000);
  pwm18.writeDutyCycle(65535); //100%
}

void loop() {
  // put your main code here, to run repeatedly:

}
