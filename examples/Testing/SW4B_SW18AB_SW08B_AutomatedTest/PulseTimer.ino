/*
SerialWombatPulseTimer_18AB SW18AB_PulseTimer0(SW18AB_6B);
//SerialWombatPulseTimer_18AB SW18AB_PulseTimer1(SW18AB_6B); 
//SerialWombatPulseTimer_18AB SW18AB_PulseTimer2(SW18AB_6B);
//SerialWombatPulseTimer_18AB SW18AB_PulseTimer3(SW18AB_6B);
//SerialWombatPulseTimer_18AB SW18AB_PulseTimer4(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer5(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer6(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer7(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer8(SW18AB_6B);   
SerialWombatPulseTimer_18AB SW18AB_PulseTimer9(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer10(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer11(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer12(SW18AB_6B);   
SerialWombatPulseTimer_18AB SW18AB_PulseTimer13(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer14(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer15(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer16(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer17(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer18(SW18AB_6B);
SerialWombatPulseTimer_18AB SW18AB_PulseTimer19(SW18AB_6B);

SerialWombatPulseTimer_18AB* SW18ABPulseTimers[] = {
  &SW18AB_PulseTimer0,
 NULL,
  NULL,
   NULL,
    NULL,
  &SW18AB_PulseTimer5,
  &SW18AB_PulseTimer6,
  &SW18AB_PulseTimer7,
  &SW18AB_PulseTimer8,   
  &SW18AB_PulseTimer9,
  &SW18AB_PulseTimer10,
  &SW18AB_PulseTimer11,
  &SW18AB_PulseTimer12,   
  &SW18AB_PulseTimer13,
  &SW18AB_PulseTimer14,
  &SW18AB_PulseTimer15,
  &SW18AB_PulseTimer16,
  &SW18AB_PulseTimer17,
  &SW18AB_PulseTimer18,
  &SW18AB_PulseTimer19
};

SerialWombatPulseTimer_18AB SW8B_PulseTimer0(SW8B_68);
SerialWombatPulseTimer_18AB SW8B_PulseTimer1(SW8B_68); 
SerialWombatPulseTimer_18AB SW8B_PulseTimer2(SW8B_68);
SerialWombatPulseTimer_18AB SW8B_PulseTimer3(SW8B_68);
SerialWombatPulseTimer_18AB SW8B_PulseTimer4(SW8B_68);
SerialWombatPulseTimer_18AB SW8B_PulseTimer5(SW8B_68);
SerialWombatPulseTimer_18AB SW8B_PulseTimer6(SW8B_68);
SerialWombatPulseTimer_18AB SW8B_PulseTimer7(SW8B_68);

SerialWombatPulseTimer_18AB* SW8BPulseTimers[] = {
  &SW8B_PulseTimer0,
  &SW8B_PulseTimer1,
  &SW8B_PulseTimer2,
  &SW8B_PulseTimer3,
  &SW8B_PulseTimer4,
  &SW8B_PulseTimer5,
  &SW8B_PulseTimer6,
  &SW8B_PulseTimer7
};
*/

void pulseTimerTest(SerialWombatChip& sw, uint8_t startPin, uint8_t endPin)
{

  {
    resetAll();

    //Duty cycle test
    for (uint8_t pin = startPin; pin <= endPin; ++pin)
    {
      delay(100);
       uint8_t sw4bPin = 0;
       // If no SW4B available, continue
       // Initialize the pin mode to pulse timer, uS
       // Set up the PWM to 32Hz (31250 uS) duty cycle to 0x1000 + 0x1000 * pin

       SerialWombatChip& sw4b = SWChipAndPinTo4BChip(sw,pin);
       if (&sw4b == &SW_NULL)
       {
        continue;
       }
       if (&sw == &SW18AB_6B)
       {
          sw4bPin = SW18ABPinTo4BPin(pin);
         
       }
       else if (&sw == &SW8B_68)
       {
         sw4bPin = SW8BPinTo4BPin(pin);
         
         
       }
       else 
       {
          test("Pulse Timer Test invalid Serial Wombat Chip",0);
       }
        if (sw4bPin == 0)
          {
            continue;  // Pin 0 can't output
          }
          SerialWombatPulseTimer_18AB pulseTimer(sw);
          pulseTimer.begin(pin);
       SerialWombatPWM_4AB pwm4B(sw4b);
       pwm4B.begin(sw4bPin);
       pwm4B.setFrequency_SW4AB(SW4AB_PWMFrequency_32_Hz);
      
       pwm4B.writePublicData(0x2000 + 0x800*pin);
       
    }

    delay(1000);

    // Read each pin.  High Should be 31250 * dutyCycle / 65536
    // Low should be 31250 - that

    for (uint8_t pin = startPin; pin <= endPin; ++pin)
    {
      delay(100);
       uint8_t sw4bPin=0;
       // If no SW4B available, continue
       // Initialize the pin mode to pulse timer, uS
       // Set up the PWM to 32Hz (31250 uS) duty cycle to 0x1000 + 0x1000 * pin

       SerialWombatChip& sw4b = SWChipAndPinTo4BChip(sw,pin);
       if (&sw4b == &SW_NULL)
       {
        continue;
       }
       if (&sw == &SW18AB_6B)
       {
          sw4bPin = SW18ABPinTo4BPin(pin);
         
       }
       else if (&sw == &SW8B_68)
       {
         sw4bPin = SW8BPinTo4BPin(pin);
         
         
       }
       else 
       {
          test("Pulse Timer Test invalid Serial Wombat Chip",0);
       }
        if (sw4bPin == 0)
          {
            continue;  // Pin 0 can't output
          }
       uint16_t  pulse = sw.readPublicData(pin);
       uint32_t expected = (0x2000 + 0x800*pin) * 31250;
       expected >>= 16;
       test("Pulse Timer High time ", pulse, expected, 50, 5);

       SerialWombatPulseTimer_18AB pulseTimer(sw);
       pulseTimer.setPinNumberForTesting(pin);

       uint16_t low = pulseTimer.readLowCounts();
       
       test("Pulse Timer Low Time ", low, 31250 - expected, 50, 5);

      pulseTimer.configurePublicDataOutput (SerialWombatPulseTimer_18AB::publicDataOutput::LOW_TIME);
      delay(200);
       
       test("Pulse Timer Low Time Public Data", pulseTimer.readPublicData(), 31250 - expected, 50, 5);

             pulseTimer.configurePublicDataOutput (SerialWombatPulseTimer_18AB::publicDataOutput::HIGH_TIME);
      delay(200);
       
       test("Pulse Timer High  Time Public Data", pulseTimer.readPublicData(), expected, 50, 5);

       
              pulseTimer.configurePublicDataOutput (SerialWombatPulseTimer_18AB::publicDataOutput::PERIOD_ON_LTH_TRANSITION );
      delay(200);
       
       test("Pulse Timer Period LTH Public Data", pulseTimer.readPublicData(), 31250, 2000, 5);  

          pulseTimer.configurePublicDataOutput (SerialWombatPulseTimer_18AB::publicDataOutput::PERIOD_ON_HTL_TRANSITION );
      delay(200);
       
       test("Pulse Timer Period HTL Public Data", pulseTimer.readPublicData(), 31250, 2000, 5);  
    }

  }
  resetAll();
  // Check for overflow on period longer than 65535 us
  {
     //Duty cycle test
    for (uint8_t pin = startPin; pin <= endPin; ++pin)
    {
      delay(100);
       uint8_t sw4bPin = 0;
     
       SerialWombatChip& sw4b = SWChipAndPinTo4BChip(sw,pin);
       if (&sw4b == &SW_NULL)
       {
        continue;
       }
       if (&sw == &SW18AB_6B)
       {
          sw4bPin = SW18ABPinTo4BPin(pin);
         
       }
       else if (&sw == &SW8B_68)
       {
         sw4bPin = SW8BPinTo4BPin(pin);
         
         
       }
       else 
       {
          test("Pulse Timer Test invalid Serial Wombat Chip",0);
       }
        if (sw4bPin == 0)
          {
            continue;  // Pin 0 can't output
          }
          SerialWombatPulseTimer_18AB pulseTimer(sw);
          pulseTimer.begin(pin);
       SerialWombatPWM_4AB pwm4B(sw4b);
       pwm4B.begin(sw4bPin);
       pwm4B.setFrequency_SW4AB(SW4AB_PWMFrequency_8_Hz);
      
       pwm4B.writePublicData(0x8000);
       
    

    delay(1000);

    
      

      
       
              pulseTimer.configurePublicDataOutput (SerialWombatPulseTimer_18AB::publicDataOutput::PERIOD_ON_LTH_TRANSITION );
      delay(1000);
       
       test("Pulse Timer Period LTH Public Data Saturation", pulseTimer.readPublicData(), 65535);  

          pulseTimer.configurePublicDataOutput (SerialWombatPulseTimer_18AB::publicDataOutput::PERIOD_ON_HTL_TRANSITION );
      delay(1000);
       
       test("Pulse Timer Period HTL Public Data Saturation", pulseTimer.readPublicData(), 65535);  
       pulseTimer.begin (pin,SW_PULSETIMER_mS); // Switch to mS to cut cpu usage on 18AB   
    }

  }
  return; //TODO REMOVE
    resetAll();
  {  //Transition Counter test
    // Update Frequency to SW4AB_PWMFrequency_1952_Hz , gather data for 10 seconds
    // Verify 19520 counts

    for (uint8_t pin = startPin; pin <= endPin; ++pin)
    {
      delay(100);
       
      
       SerialWombatChip& sw4b = SWChipAndPinTo4BChip(sw,pin);
       if (&sw4b == &SW_NULL)
       {
        continue;
       }
       uint8_t sw4bPin = 0;
       if (&sw == &SW18AB_6B)
       {
          sw4bPin = SW18ABPinTo4BPin(pin);
         
       }
       else if (&sw == &SW8B_68)
       {
         sw4bPin = SW8BPinTo4BPin(pin);
       }
       else 
       {
          test("Pulse Timer Test invalid Serial Wombat Chip",0);
       }
        if (sw4bPin == 0)
          {
            continue;  // Pin 0 can't output
          }
       SerialWombatPWM_4AB pwm4B(sw4b);
       pwm4B.begin(sw4bPin);
       pwm4B.setFrequency_SW4AB(SW4AB_PWMFrequency_1952_Hz);
      
       pwm4B.writePublicData(0x8000);

          SerialWombatPulseTimer_18AB pulseTimer(sw);
          pulseTimer.begin(pin);
    delay(10000);
       uint16_t counts = pulseTimer.readPulses();
       char s[50];
       sprintf(s,"Pulse Timer pin %d Pulse Counter (uS) ",pin);
       test(s, counts, 19520, 500, 5);   
       pulseTimer.begin (pin,SW_PULSETIMER_mS); // Switch to mS to cut cpu usage on 18AB   
    }
  }

    // Set to 1 Hz and run in mS mode, set pwm to 16384 .  verify 250 (256?) and 750 ms high, low
{
  resetAll();
  
    for (uint8_t pin = startPin; pin <= endPin; ++pin)
    {
   SerialWombatChip& sw4b = SWChipAndPinTo4BChip(sw,pin);

        if (&sw4b == &SW_NULL)
        {
          continue;
        }
       SerialWombatPulseTimer_18AB pulseTimer(sw);
          pulseTimer.begin(pin,SW_PULSETIMER_mS);
       SerialWombatPWM_4AB pwm4B(sw4b);
        uint8_t sw4bPin = 0;
       if (&sw == &SW18AB_6B)
       {
          sw4bPin = SW18ABPinTo4BPin(pin);
         
       }
       else if (&sw == &SW8B_68)
       {
         sw4bPin = SW8BPinTo4BPin(pin);
       }
       else 
       {
          test("Pulse Timer Test invalid Serial Wombat Chip",0);
       }
        if (sw4bPin == 0)
          {
            continue;  // Pin 0 can't output
          }
       pwm4B.begin(sw4bPin);
       pwm4B.setFrequency_SW4AB(SW4AB_PWMFrequency_1_Hz);
      
       pwm4B.writePublicData(16384);
       delay(3000);
       test("Pulse Timer mS high Test", pulseTimer.readPublicData(), 256, 10, 5);
       test("Pulse Timer mS low Test", pulseTimer.readLowCounts(), 768, 10, 5);
    }
}


    
   #if 0

    SWPWM5.writeDutyCycle(setting);

    SWPulseTimer5.begin(5);
    SWPulseTimer19.begin(19);
    uint32_t startTime_millis = millis();
    for (setting = 2000; setting < 65000; setting += 1000)
    {
      SWPWM5.writeDutyCycle(setting);
      SWPWM19.writeDutyCycle(setting);

      delay(100);

      uint16_t pt5h = SWPulseTimer5.readHighCounts();
      uint16_t pt19h = SWPulseTimer19.readHighCounts();
      uint16_t pt5l = SWPulseTimer5.readLowCounts();
      //uint16_t pt19l = SWPulseTimer19.readLowCounts();

      if ( (pt5h > setting + setting / 20) || (pt5h < setting - setting / 20))
      {
        fail(setting);
        Serial.print("FailA 5: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.println(SWPulseTimer5.readHighCounts());
      }
      else
      {
        pass(0);
      }
      if ( (pt19h > setting + setting / 20u) || (pt19h < setting - setting / 20u))
      {
        fail(setting);
        Serial.print("FailB 19: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.println(SWPulseTimer19.readHighCounts());
      }
      else
      {
        pass(0);
      }

      uint32_t measuredDuty5 = 0;

      if (((uint32_t)pt5h + pt5l) > 0)
      {
        measuredDuty5 = ((uint32_t)pt5h * 65536u) / ((uint32_t)pt5h + pt5l);
      }

      if ( (measuredDuty5 > setting + setting / 20u) || (measuredDuty5 < setting - setting / 20u))
      {
        fail(setting);
        Serial.print("FailC 5 duty: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.println(measuredDuty5);
      }
      else
      {
        pass(0);
      }

      uint32_t measuredDuty19;
      if (((uint32_t)pt5h + pt5l) > 0)
      {
        measuredDuty19 = (uint32_t)pt5h * 65536 / ((uint32_t)pt5h + pt5l);
      }

      if ( (measuredDuty19 > setting + setting / 20u) || (measuredDuty19 < setting - setting / 20u))
      {
        fail(setting);
        Serial.print("FailD 19 duty: ");
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

    uint32_t expectedPulses = (endTime_millis - startTime_millis) * 16u / 1000u;

    if ( (pulses19 > (expectedPulses + expectedPulses / 20u + 2u)) || (pulses19 < (expectedPulses - expectedPulses / 20u - 2u)))
    {
      fail(setting);
      Serial.print("FailE Pulses 19: ");
      Serial.print(expectedPulses);
      Serial.print(" ");
      Serial.println(pulses19);
    }
    else
    {
      pass(0);
    }

    if ( (pulses5 > (expectedPulses + expectedPulses / 20u + 2u)) || (pulses19 < (expectedPulses - expectedPulses / 20u - 2u)))
    {
      fail(setting);
      Serial.print("FailF Pulses 5: ");
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

    SWPWM5.begin(2);
    SWPWM19.begin(1);


    SWPWM5.setFrequency_SW4AB(SW4AB_PWMFrequency_1_Hz);
    SWPWM19.setFrequency_SW4AB(SW4AB_PWMFrequency_1_Hz);
    SWPWM5.writeDutyCycle(setting);

    SWPulseTimer5.begin(5, SW_PULSETIMER_mS, false);
    SWPulseTimer19.begin(19, SW_PULSETIMER_mS, false);
    //uint32_t startTime_millis = millis();
    for (setting = 2000; setting < 65000; setting += 5000)
    {
      SWPWM5.writeDutyCycle(setting);
      SWPWM19.writeDutyCycle(setting);

      delay(3200);

      uint16_t pt5h = SWPulseTimer5.readHighCounts();
      uint16_t pt19h = SWPulseTimer19.readHighCounts();
      uint16_t pt5l = SWPulseTimer5.readLowCounts();
      //uint16_t pt19l = SWPulseTimer19.readLowCounts();

      uint32_t expected = ((uint32_t)1000 * setting) >> 16;
      if ( (pt5h > expected + expected / 20u + 2u) || (pt5h < expected - expected / 20u - 2u))
      {
        fail(setting);
        Serial.print("FailG 5 mS: ");
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
      if ( (pt19h > expected + expected / 20u + 2u) || (pt19h < expected - expected / 20u - 2u))
      {
        fail(setting);
        Serial.print("FailH 19 mS: ");
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


      uint32_t measuredDuty5  = 0;
      if (((uint32_t)pt5h + pt5l) > 0)
      {
        measuredDuty5 = (uint32_t)pt5h * 65536u / ((uint32_t)pt5h + pt5l);
      }

      if ( (measuredDuty5 > setting + setting / 20u + 100u) || (measuredDuty5 < setting - setting / 20u - 100u))
      {
        fail(setting);
        Serial.print("FailI 5 duty mS: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.println(measuredDuty5);
      }
      else
      {
        pass(0);
      }

      uint32_t measuredDuty19 = 0;
      if (((uint32_t)pt5h + pt5l) > 0)
      {

        measuredDuty19 = (uint32_t)pt5h * 65536u / ((uint32_t)pt5h + pt5l);
      }

      if ( (measuredDuty19 > setting + setting / 20u + 100u) || (measuredDuty19 < setting - setting / 20u - 100u))
      {
        fail(setting);
        Serial.print("FailJ 19 duty mS: ");
        Serial.print(setting);
        Serial.print(" ");
        Serial.println(measuredDuty19);
      }
      else
      {
        pass(0);
      }

    }
    #endif
  

}
