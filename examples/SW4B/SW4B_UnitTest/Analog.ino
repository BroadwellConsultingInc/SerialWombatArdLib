SerialWombatAnalogInput analog1(SW6D), analog2(SW6D),
                        analog3(SW6D);

void analogTest()
{
  resetAll();

  pwm1.begin(1, 0);
  pwm2.begin(2, 0);
  pwm3.begin(3, 0);
  SW6E.pinMode(1, OUTPUT);
  SW6E.digitalWrite(1, LOW); // Ground capacitors through 1k resistor
  delay(3000);
  SW6E.pinMode(3, OUTPUT);
  SW6E.digitalWrite(3, LOW); // Ground capacitors directly
  delay(500);

  pwm1.begin(1);
  pwm2.begin(2);
  pwm3.begin(3);
  pwm1.setFrequency_SW4AB(SW4AB_PWMFrequency_31250_Hz);
  analog1.begin(3, 64, 65000);
  analog2.begin(2, 64, 65000);
  analog3.begin(1, 64, 65000);

  for (int pin = 0; pin < 3; ++pin)
  {

    for (int dutyCycle = 2000;  dutyCycle < 65535; dutyCycle += 2000)
    {
      uint16_t averagedResult;
      uint16_t filteredResult;
      uint16_t rawResult;
      switch (pin)
      {

        case 0:
          pwm1.writeDutyCycle(dutyCycle);
          delay(3000);
          averagedResult = analog1.readAveragedCounts();
          filteredResult = analog1.readFilteredCounts();
          rawResult = analog1.readCounts();
          break;
        case 1:
          pwm2.writeDutyCycle(dutyCycle);
          delay(3000);
          averagedResult = analog2.readAveragedCounts();
          filteredResult = analog2.readFilteredCounts();
          rawResult = analog2.readCounts();
          break;
        case 2:
          pwm3.writeDutyCycle(dutyCycle);
          delay(3000);
          averagedResult = analog3.readAveragedCounts();
          filteredResult = analog3.readFilteredCounts();
          rawResult = analog3.readCounts();
          break;
      }
      if (averagedResult <= dutyCycle * 1.1 && averagedResult >= dutyCycle * 0.9)
      {
        pass(pin * 100000 + dutyCycle);
      }
      else
      {
        fail (pin * 100000 + dutyCycle);
#ifdef PRINT_FAILURES
        Serial.print(pin);
        Serial.print(": ");
        Serial.print("Average Measured: ");
        Serial.print(averagedResult);
        Serial.print(" Expected: ");
        Serial.println(dutyCycle);
#endif
      }

      if (filteredResult <= dutyCycle * 1.1 && filteredResult >= dutyCycle * 0.9)
      {
        pass(pin * 100000 + dutyCycle);
      }
      else
      {
        fail (pin * 100000 + dutyCycle);
        #ifdef PRINT_FAILURES
        Serial.print(pin);
        Serial.print(": ");
        Serial.print("Filtered Measured: ");
        Serial.print(filteredResult);
        Serial.print(" Expected: ");
        Serial.println(dutyCycle);
        #endif
      }
      if (rawResult <= dutyCycle * 1.2 && rawResult >= dutyCycle * 0.8)
      {
        pass(pin * 100000 + dutyCycle);
      }
      else
      {
        #ifdef PRINT_FAILURES
        fail (pin * 100000 + dutyCycle);
        Serial.print(pin);
        Serial.print(": ");
        Serial.print("Raw Measured: ");
        Serial.print(rawResult);
        Serial.print(" Expected: ");
        Serial.println(dutyCycle);
        #endif
      }

    }
  }
}
