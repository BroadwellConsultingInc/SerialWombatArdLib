void pwmTest()
{
  pwmIteration(SW4AB_PWMFrequency_1_Hz, 1000000 / 1);
  pwmIteration(SW4AB_PWMFrequency_2_Hz, 1000000 / 2);
  pwmIteration(SW4AB_PWMFrequency_4_Hz, 1000000 / 4);
  pwmIteration(SW4AB_PWMFrequency_8_Hz, 1000000 / 8);
  pwmIteration(SW4AB_PWMFrequency_16_Hz, 1000000 / 16);
  pwmIteration(SW4AB_PWMFrequency_32_Hz, 1000000 / 32);
  pwmIteration(SW4AB_PWMFrequency_63_Hz, 1000000 / 63);
  pwmIteration(SW4AB_PWMFrequency_125_Hz, 1000000 / 125);
  pwmIteration(SW4AB_PWMFrequency_244_Hz, 1000000 / 244);
  pwmIteration(SW4AB_PWMFrequency_488_Hz, 1000000 / 488);
  pwmIteration(SW4AB_PWMFrequency_976_Hz, 1000000 / 976);
  pwmIteration(SW4AB_PWMFrequency_1952_Hz, 1000000 / 1952);
  pwmIteration(SW4AB_PWMFrequency_3900_Hz, 1000000 / 3900);
  pwmIteration(SW4AB_PWMFrequency_7800_Hz, 1000000 / 7800);
}

void pwmIteration(Wombat4A_B_PWMFrequencyValues_t frequency, int period)
{
  updateDisplay();
  resetAll();
  for (int i = 0; i < NUM_TEST_PINS ; ++i)
  {
    pwms[i].begin(pinMatches[i], 0);
    pwms[i].setFrequency_SW4AB(frequency);
  }
  /*
    pwm1.begin(1,0);
    pwm2.begin(2,0);
    pwm3.begin(3,0);
    pwm4.begin(2,0);

    pwm1.setFrequency_SW4AB(frequency);
    pwm4.setFrequency_SW4AB(frequency);
  */
  SerialWombatPulseTimerUnits units = SW_PULSETIMER_uS;
  if (period > 60000)
  {
    units = SW_PULSETIMER_mS;
  }

  for (int pin = 0; pin < NUM_TEST_PINS; ++pin)
  {
    pulses[pin].begin(pin, units, false);
    /*
      switch (pin)
      {
    	case 0:
    		pulse1.begin(3,units,false);
    		break;

    	case 1:
    		pulse2.begin(2,units,false);
    		break;

    	case 2:
    		pulse3.begin(1,units,false);
    		break;

    	case 3:
    		pulse4.begin(0,units,false);
    		break;
      }
    */



    for (int dutyCycle = 1000; dutyCycle < 64000; dutyCycle += 2500)
    {
      int pulseWidthMeasured;
      int pulseWidthExpected;

      if (period > 60000)
      {
        //mS
        pulseWidthExpected = (dutyCycle * (period / 1000)) / 65536 ;
        if (pulseWidthExpected < 2)
        {
          continue;
        }
        if (period - (pulseWidthExpected * 1000) < 3000)
        {
          continue;
        }
      }
      else
      {
        //uS
        pulseWidthExpected = (dutyCycle * period) / 65536 ;
        if (pulseWidthExpected < 15)
        {
          continue;
        }
        if (period - pulseWidthExpected < 15)
        {
          continue;
        }
        if (pulseWidthExpected * 200 < period )
        {
          continue;
        }
      }

      for (int i = 0; i < NUM_TEST_PINS; ++i)
      {
        pwms[i].writeDutyCycle(0);
      }
      /*
        pwm1.writeDutyCycle(0);
        pwm2.writeDutyCycle(0);
        pwm3.writeDutyCycle(0);
        pwm4.writeDutyCycle(0);
      */
      delay(75);

      pwms[pin].writeDutyCycle((uint16_t) dutyCycle);
      delay(period * 3 / 1000);
      delay(10);
      pulseWidthMeasured = pulses[pin].readHighCounts();

      /*
        switch(pin)
        {
      	case 0:

      		pwm1.writeDutyCycle((uint16_t)dutyCycle);
      		delay(period * 3 / 1000);
      		delay(10);
      		pulseWidthMeasured = pulse1.readHighCounts();
      		break;

      	case 1:
      		pwm2.writeDutyCycle((uint16_t)dutyCycle);
      		delay(period * 3 / 1000);
      		delay(10);
      		pulseWidthMeasured = pulse2.readHighCounts();
      		break;

      	case 2:
      		pwm3.writeDutyCycle((uint16_t)dutyCycle);
      		delay(period * 3 / 1000);
      		delay(10);
      		pulseWidthMeasured = pulse3.readHighCounts();
      		break;

      	case 3:
      		pwm4.writeDutyCycle((uint16_t)dutyCycle);
      		delay(period * 3 / 1000);
      		delay(10);
      		pulseWidthMeasured = pulse4.readHighCounts();
      		break;
        }
      */


      int threshold = 3; // +/- 3 ms
      if (units == SW_PULSETIMER_uS)
      {
        threshold = 50; // 50uS
      }

      // Pass criteria:  +/- 10% (Due to clock variation between chips)
      // or +/- threshold (for measurement variation on short pulses)
      if ((pulseWidthMeasured > (pulseWidthExpected * 0.9)
           && pulseWidthMeasured < (pulseWidthExpected * 1.1))
          || ((pulseWidthMeasured - pulseWidthExpected) < threshold
              && (pulseWidthExpected - pulseWidthMeasured ) < threshold))
      {
        pass(1000000 + dutyCycle);
      }
      else
      {
        fail(1000000 + dutyCycle);
        Serial.print(pin);
        Serial.print(": ");
        Serial.print("Measured: ");
        Serial.print(pulseWidthMeasured);
        Serial.print(" Expected: ");
        Serial.print(pulseWidthExpected);
        Serial.print(" DutyCycle: ");
        Serial.print(dutyCycle);
        Serial.print(" Period: ");
        Serial.println(period);
        Serial.println();
      }
    }
  }
}
