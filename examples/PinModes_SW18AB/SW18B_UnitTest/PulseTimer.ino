SerialWombatPulseTimer SWPulseTimer5(SW6B);
SerialWombatPulseTimer SWPulseTimer19(SW6B);
SerialWombatPWM SWPWM5(SW6C);
SerialWombatPWM SWPWM19(SW6F);
void pulseTimerTest()
{
  {
    resetAll();
    uint16_t setting = 0x2000;

    SWPWM5.begin(1);
    SWPWM19.begin(3);


    SWPWM5.setFrequency_SW4AB(SW4AB_PWMFrequency_16_Hz);
    SWPWM19.setFrequency_SW4AB(SW4AB_PWMFrequency_16_Hz);
    SWPWM5.writeDutyCycle(setting);

    SWPulseTimer5.begin(5);
    SWPulseTimer19.begin(19);
    uint32_t startTime_millis = millis();
    for (setting = 2000; setting < 65000; setting += 1000)
    {
      SWPWM5.writeDutyCycle(setting);
      SWPWM19.writeDutyCycle(setting);

      delay(100);

      int pt5h = SWPulseTimer5.readHighCounts();
      int pt19h = SWPulseTimer19.readHighCounts();
      int pt5l = SWPulseTimer5.readLowCounts();
      int pt19l = SWPulseTimer19.readLowCounts();

      if ( (pt5h > setting + setting / 20) || (pt5h < setting - setting / 20))
      {
        fail(setting);
        Serial.print("Fail 5: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.println(SWPulseTimer5.readHighCounts());
      }
      else
      {
        pass(0);
      }
      if ( (pt19h > setting + setting / 20) || (pt19h < setting - setting / 20))
      {
        fail(setting);
        Serial.print("Fail 19: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.println(SWPulseTimer19.readHighCounts());
      }
      else
      {
        pass(0);
      }

      uint32_t measuredDuty5 = (uint32_t)pt5h * 65536 / ((uint32_t)pt5h + pt5l);

      if ( (measuredDuty5 > setting + setting / 20) || (measuredDuty5 < setting - setting / 20))
      {
        fail(setting);
        Serial.print("Fail 5 duty: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.println(measuredDuty5);
      }
      else
      {
        pass(0);
      }

      uint32_t measuredDuty19 = (uint32_t)pt5h * 65536 / ((uint32_t)pt5h + pt5l);

      if ( (measuredDuty19 > setting + setting / 20) || (measuredDuty19 < setting - setting / 20))
      {
        fail(setting);
        Serial.print("Fail 19 duty: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.println(measuredDuty19);
      }
      else
      {
        pass(0);
      }

    }
    uint16_t pulses5 = SWPulseTimer5.readPulses();
    uint16_t pulses19 = SWPulseTimer19.readPulses();
    uint32_t endTime_millis = millis();

    uint32_t expectedPulses = (endTime_millis - startTime_millis) * 16 / 1000;

    if ( (pulses19 > (expectedPulses + expectedPulses / 20 + 2)) || (pulses19 < (expectedPulses - expectedPulses / 20 - 2)))
    {
      fail(setting);
      Serial.print("Fail Pulses 19: ");
      Serial.print(expectedPulses);
      Serial.print(" ");
      Serial.println(pulses19);
    }
    else
    {
      pass(0);
    }

    if ( (pulses5 > (expectedPulses + expectedPulses / 20 + 2)) || (pulses19 < (expectedPulses - expectedPulses / 20 - 2)))
    {
      fail(setting);
      Serial.print("Fail Pulses 5: ");
      Serial.print(expectedPulses);
      Serial.print(" ");
      Serial.println(pulses19);
    }
    else
    {
      pass(0);
    }
  }

  {
    resetAll();
    uint16_t setting = 0x2000;

    SWPWM5.begin(1);
    SWPWM19.begin(3);


    SWPWM5.setFrequency_SW4AB(SW4AB_PWMFrequency_1_Hz);
    SWPWM19.setFrequency_SW4AB(SW4AB_PWMFrequency_1_Hz);
    SWPWM5.writeDutyCycle(setting);

    SWPulseTimer5.begin(5, SW_PULSETIMER_mS, false);
    SWPulseTimer19.begin(19, SW_PULSETIMER_mS, false);
    uint32_t startTime_millis = millis();
    for (setting = 2000; setting < 65000; setting += 5000)
    {
      SWPWM5.writeDutyCycle(setting);
      SWPWM19.writeDutyCycle(setting);

      delay(3200);

      int pt5h = SWPulseTimer5.readHighCounts();
      int pt19h = SWPulseTimer19.readHighCounts();
      int pt5l = SWPulseTimer5.readLowCounts();
      int pt19l = SWPulseTimer19.readLowCounts();

      int32_t expected = ((uint32_t)1000 * setting) >> 16;
      if ( (pt5h > expected + expected / 20 +2) || (pt5h < expected - expected / 20 - 2))
      {
        fail(setting);
        Serial.print("Fail 5 mS: ");
        Serial.print(setting);
         Serial.print(expected);
        Serial.print(" ");
        Serial.print(" ");
        Serial.println(SWPulseTimer5.readHighCounts());
      }
      else
      {
        pass(0);
      }
      if ( (pt19h > expected + expected / 20 + 2) || (pt19h < expected - expected / 20 - 2))
      {
        fail(setting);
        Serial.print("Fail 19 mS: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.print(expected);
        Serial.print(" ");
        Serial.println(SWPulseTimer19.readHighCounts());
      }
      else
      {
        pass(0);
      }

      uint32_t measuredDuty5 = (uint32_t)pt5h * 65536 / ((uint32_t)pt5h + pt5l);

      if ( (measuredDuty5 > setting + setting / 20 + 100) || (measuredDuty5 < setting - setting / 20 - 100))
      {
        fail(setting);
        Serial.print("Fail 5 duty mS: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.println(measuredDuty5);
      }
      else
      {
        pass(0);
      }

      uint32_t measuredDuty19 = (uint32_t)pt5h * 65536 / ((uint32_t)pt5h + pt5l);

      if ( (measuredDuty19 > setting + setting / 20 + 100) || (measuredDuty19 < setting - setting / 20 - 100))
      {
        fail(setting);
        Serial.print("Fail 19 duty mS: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.println(measuredDuty19);
      }
      else
      {
        pass(0);
      }

    }
  }

}
