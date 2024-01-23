/*
This example shows how to measure servo pulses from a radio control reciever on a Serial Wombat 18AB chip.  

IMPORTANT:   This example requires firmware version 2.1.1 or later to work.

This example assumes 6 channels of RC receiver hooked up to pins 14 through 19.  The measured pulse length in uS is
printed to Serial.

This sketch makes use of the SerialWombat18ABOscillatorTuner to improve the meaurement accuracy.  About 1 minute of
runtime is required after reset to achieve full accuracy improvement.

A video demonstrating the use of the SerialWombatPulseTimer_18AB class for RC measurement  on the Serial Wombat 18AB chip is available at:
TBD

Documentation for the SerialWombatPulseTimer_18AB Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_pulse_timer__18_a_b.html
*/

#include <SerialWombat.h>

SerialWombatChip sw;

SerialWombatPulseTimer_18AB rcWheel(sw),rcThrottle(sw),rcSwitch3(sw),rcSwitch4(sw),rcKnob5(sw),rcKnob6(sw);

SerialWombat18ABOscillatorTuner oscTun(sw);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  sw.begin(Wire,0x6B);

  rcWheel.begin(14);
  rcThrottle.begin(15);
  rcSwitch3.begin(16);
  rcSwitch4.begin(17);
  rcKnob5.begin(18);
  rcKnob6.begin(19);

}

void loop() {
  // put your main code here, to run repeatedly:
  char s[80];
  sprintf(s,"%5d %5d %5d %5d %5d %5d",rcWheel.readHighCounts(), rcThrottle.readHighCounts(), rcSwitch3.readHighCounts(),
        rcSwitch4.readHighCounts(),rcKnob5.readHighCounts(),rcKnob6.readHighCounts());
  Serial.println(s);
        delay(200);
  oscTun.update();

}
