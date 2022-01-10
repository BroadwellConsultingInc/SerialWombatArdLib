
#define SERVO_DEFAULT_BASE 544
#define SERVO_DEFAULT_VARIABLE 1856
void servoTest()
{
  resetAll();
  for (int pin = 0; pin < NUM_TEST_PINS; ++pin)
  {
    pulses[pin].begin(pin, SW_PULSETIMER_uS, false);
    servos[pin].attach(pinMatches[pin]);
  }

  servoIteration(SERVO_DEFAULT_BASE, SERVO_DEFAULT_VARIABLE, false);

  for (int pin = 0; pin < NUM_TEST_PINS; ++pin)
  {
    servos[pin].attach(pinMatches[pin], false);
  }
  servoIteration(SERVO_DEFAULT_BASE, SERVO_DEFAULT_VARIABLE, false);

  for (int pin = 0; pin < NUM_TEST_PINS; ++pin)
  {
    servos[pin].attach(pinMatches[pin], true);
  }
  servoIteration(SERVO_DEFAULT_BASE, SERVO_DEFAULT_VARIABLE, true);


  for (uint16_t base = 200; base <= 1000; base += 200)
  {
    for (uint16_t variable = 1000; variable < 3000; variable += 200)
    {
      for (int pin = 0; pin < NUM_TEST_PINS; ++pin)
      {
        servos[pin].attach(pinMatches[pin], base, base + variable);
      }
      servoIteration(base, variable, false);
      for (int pin = 0; pin < NUM_TEST_PINS; ++pin)
      {
        servos[pin].attach(pinMatches[pin], base, base + variable, false);
      }
      servoIteration(base, variable, false);
      for (int pin = 0; pin < NUM_TEST_PINS; ++pin)
      {
        servos[pin].attach(pinMatches[pin], base, base + variable, true);
      }
      servoIteration(base, variable, true);
    }
  }

}

void servoIteration(uint16_t base, uint16_t variable, bool reverse)
{

      updateDisplay();
  for (int position = 0; position < 64000; position += 1000)
  {
    //for (int pin = 0; pin < NUM_TEST_PINS ; ++pin)
    for (int pin = 0; pin < 1 ; ++pin)  //TODO Restore to all pins
    {
      servos[pin].write16bit((uint16_t)(position  + 7000 * pin));
    }
    delay(200);
    //for (int pin = 0; pin < NUM_TEST_PINS ; ++pin)
    for (int pin = 0; pin < 1 ; ++pin)  //TODO Restore to all pins
    {
      int pulseWidthMeasured;
      int pulseWidthExpected;
      pulseWidthMeasured = pulses[pin].readHighCounts();
      if (reverse)
      {
        pulseWidthExpected = ((65535 - ((uint16_t)(position + 7000 * pin))) * variable) / 65536 + base;
      }
      else
      {
        pulseWidthExpected = (((uint16_t)(position + 7000 * pin)) * variable) / 65536 + base;
      }
      if (pulseWidthMeasured > (pulseWidthExpected - 100) && pulseWidthMeasured < (pulseWidthExpected + 100))
      {
        pass(1000000 + position);
      }
      else
      {
        fail(1000000 + position);
        #ifdef PRINT_FAILURES
        Serial.print(pin);
        Serial.print(": Measured: ");
        Serial.print(pulseWidthMeasured);
        Serial.print(" Expected: ");
        Serial.print(pulseWidthExpected);
        Serial.print(" Position: ");
        Serial.print(position);
        Serial.print (" Base: ");
        Serial.print (base);
        Serial.print(" Variable: ");
        Serial.print (variable);
        Serial.print(" Reverse: ");
        Serial.println(reverse);
        #endif
      }

    }

    /*

      position += 20000;
      pulseWidthMeasured = pulse2.readHighCounts();
      if (reverse)
      {
          pulseWidthExpected = (((65535 - (uint16_t)(position ))) * variable) / 65536 + base;
      }
      else
      {
          pulseWidthExpected = (((uint16_t)(position )) * variable) / 65536 + base;
      }
      if (pulseWidthMeasured > (pulseWidthExpected - 100) && pulseWidthMeasured < (pulseWidthExpected + 100))
      {
      pass(2000000 + position);
      }
      else
      {
      fail(2000000 + position);
      Serial.print("2: Measured: ");
      Serial.print(pulseWidthMeasured);
      Serial.print(" Expected: ");
      Serial.print(pulseWidthExpected);
       Serial.print(" Position: ");
      Serial.print(position);
       Serial.print (" Base: ");
      Serial.print (base);
      Serial.print(" Variable: ");
      Serial.print (variable);
      Serial.print(" Reverse: ");
      Serial.println(reverse);
      }
      position += 20000;
      pulseWidthMeasured = pulse3.readHighCounts();
      if (reverse)
      {
          pulseWidthExpected = (((65535 - (uint16_t)(position ))) * variable) / 65536 + base;
      }
      else
      {
          pulseWidthExpected = (((uint16_t)(position )) * variable) / 65536 + base;
      }
      if (pulseWidthMeasured > (pulseWidthExpected - 100) && pulseWidthMeasured < (pulseWidthExpected + 100))
      {
      pass(3000000 + position);
      }
      else
      {
      fail(3000000 + position);
      Serial.print("3: Measured: ");
      Serial.print(pulseWidthMeasured);
      Serial.print(" Expected: ");
      Serial.print(pulseWidthExpected);
       Serial.print(" Position: ");
      Serial.print(position);
       Serial.print (" Base: ");
      Serial.print (base);
      Serial.print(" Variable: ");
      Serial.print (variable);
      Serial.print(" Reverse: ");
      Serial.println(reverse);
      }

      position += 10000;
         pulseWidthMeasured = pulse4.readHighCounts();
         if (reverse)
         {
          pulseWidthExpected = (((65535 - (uint16_t)(position ))) * variable) / 65536 + base;
         }
         else
         {
      pulseWidthExpected = (((uint16_t)(position )) * variable) / 65536 + base;

         }
      if (pulseWidthMeasured > (pulseWidthExpected - 100) && pulseWidthMeasured < (pulseWidthExpected + 100))
      {
      pass(4000000 + position);
      }
      else
      {
      fail(4000000 + position);
      Serial.print("4: Measured: ");
      Serial.print(pulseWidthMeasured);
      Serial.print(" Expected: ");
      Serial.print(pulseWidthExpected);
       Serial.print(" Position: ");
      Serial.print(position);
       Serial.print (" Base: ");
      Serial.print (base);
      Serial.print(" Variable: ");
      Serial.print (variable);
      Serial.print(" Reverse: ");
      Serial.println(reverse);
      }
      position -=50000;
    */
  }

}
