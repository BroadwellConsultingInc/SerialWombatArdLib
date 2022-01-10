SerialWombatPWM SWPWM0(SW6B);


#define PWM_TEST_INCREMENTS 100

#define PWM_NUM_TEST_PINS NUM_TEST_PINS
void pwmTest()
{
  resetAll();
  for (int pin = 0; pin < PWM_NUM_TEST_PINS; ++ pin)
  {
    if (!test_pinCanBeOutput(pin))
    {
      continue;
    }
    initializePulseReaduS(pin);
  }
  
  for (uint16_t period = 500; period <= 60000; period += 500)
  {
   
      for (int reverse = 0; reverse < 2; ++ reverse)
      {
        for (int i = 1000; i < 65535; i += 5000)
        {

          for (int pin = 0; pin < PWM_NUM_TEST_PINS; ++ pin)
          {
            if (!test_pinCanBeOutput(pin))
            {
              continue;
            }
            uint16_t setting = i  + (pin * 63000 / NUM_TEST_PINS);
            
            int32_t expected = period * (uint32_t)setting / 65536 ;

            if (expected < 30 || period - expected < 30)
            {
              continue;
            }
            
            SWPWM0.begin(pin);
            SWPWM0.setPeriod_SW18AB_uS(period);
            SWPWM0.writeDutyCycle(setting);
 
          }
          delay(100);
          for (int pin = 0; pin < PWM_NUM_TEST_PINS; ++ pin)
          {
            if (!test_pinCanBeOutput(pin))
            {
              continue;
            }

            uint16_t result = pulseRead(pin);
            uint16_t setting = i  + (pin * 65535 / NUM_TEST_PINS);
            
            int32_t expected = period * (uint32_t)setting / 65536 ;
            if (expected < 30 || period - expected < 30)
            {
              continue;
            }

            if ((result < (expected + (expected / 20)) + 20) && (result > (expected - (expected / 20) - 20))) // 2% clock error on receiver, 2% error on sender, worst case 4% either way plus a little round off error, +/- 20 for quant. error in DMA.
            {
              /*
                            Serial.print(pin);
              Serial.print(" ");
              Serial.print(setting);
              Serial.print(" ");
              Serial.print(result);
              Serial.print(" ");
              Serial.print(expected);
              Serial.print("/");
              Serial.print(period);
              Serial.print(" ");
              Serial.print ("Pass (");
              Serial.print (failCount);
              Serial.print("/");
              Serial.print (failCount + passCount);
              Serial.print(") ");
              Serial.println();
              */
              pass(i);
            }
            else
            {
              /*
              Serial.print(pin);
              Serial.print(" ");
              Serial.print(setting);
              Serial.print(" ");
              Serial.print(result);
              Serial.print(" ");
              Serial.print(expected);
              Serial.print("/");
              Serial.print(period);
              Serial.print(" ");
              Serial.print ("Fail (");
              Serial.print (failCount);
              Serial.print("/");
              Serial.print (failCount + passCount);
              Serial.print(") ");
              Serial.println();
              */
              fail(i);
            }


          }
        }
      }
    
  }
}
