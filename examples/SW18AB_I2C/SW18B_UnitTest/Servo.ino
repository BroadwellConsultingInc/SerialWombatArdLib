 #define SERVO_DEFAULT_BASE 544
#define SERVO_DEFAULT_VARIABLE 1856


SerialWombatServo SWServo0(SW6B);


#define SERVO_TEST_INCREMENTS 100
void servoTest()
{
  resetAll();
  for (int pin = 0; pin < 20; ++ pin)
  {
    if (!test_pinCanBeOutput(pin))
    {
      continue;
    }
    initializePulseReaduS(pin);
  }
  for (uint16_t variable = 800; variable <= 2000; variable += 100)
  {
    for (uint16_t base = 500; base <= 1200; base += 100)
    {
      for (int reverse = 0; reverse < 2; ++ reverse)
      {
        for (int i = 0; i < SERVO_TEST_INCREMENTS; ++i)
        {

          for (int pin = 0; pin < 20; ++ pin)
          {
            if (!test_pinCanBeOutput(pin))
            {
              continue;
            }

            SWServo0.attach(pin, base, base + variable, reverse);
            SWServo0.write16bit((i * 65535 ) / SERVO_TEST_INCREMENTS + (pin * 65535 / NUM_TEST_PINS));
          }
          delay(100);
          for (int pin = 0; pin < 20; ++ pin)
          {
            if (!test_pinCanBeOutput(pin))
            {
              continue;
            }

            uint16_t result = pulseRead(pin);
            uint16_t setting = (i * 65535 ) / SERVO_TEST_INCREMENTS + (pin * 65535 / NUM_TEST_PINS);
            if (reverse)
            {
              setting = (65535 - setting);
            }
            int32_t expected = (variable) * (uint32_t)setting / 65536 + base;

            if ((result < (expected + (expected / 20)) + 20) && (result > (expected - (expected / 20) - 20))) // 2% clock error on receiver, 2% error on sender, worst case 4% either way plus a little round off error, +/- 20 for quant. error in DMA.
            {
              /*
                Serial.print(pin);
                Serial.print(" ");
                Serial.print(result);
                Serial.print(" ");
                Serial.print(expected);
                Serial.print(" ");
                Serial.print ("Pass");
                Serial.println();
              */
              pass(i);
            }
            else
            {
              /*
              Serial.print(pin);
              Serial.print(" ");
              Serial.print(result);
              Serial.print(" ");
              Serial.print(expected);
              Serial.print(" ");
              Serial.print ("Fail");
              Serial.println();
              */
              fail(i);
            }


          }
        }
      }
    }
  }
}
