void watchdogTest()
{
  for (int pin = 0; pin < NUM_TEST_PINS; ++pin)
  {
    pulses[pin].begin(pin, SW_PULSETIMER_mS, false);
  }
  
  for (int pin = 0; pin < NUM_TEST_PINS ; ++pin)
  {
    updateDisplay();
    resetAll();
  
    pulses[pin].begin(pin, SW_PULSETIMER_mS, false);
    watchdogs[pin].begin(pinMatches[pin],SW_LOW,SW_LOW,100,false);
    watchdogs[pin].begin(pinMatches[pin],SW_HIGH,SW_LOW,100,false);
    delay(200);
    uint16_t duration = pulses[pin].readHighCounts();
    if (duration >80 && duration < 120)
    {
      pass(pin);
    }
    else
    {
      fail(pin);
      #ifdef PRINT_FAILURES
      Serial.printf("pin %d: expected %d, got %d\n",pin,100,duration);
      #endif
    }
    watchdogs[pin].begin(pinMatches[pin],SW_HIGH,SW_HIGH,200,false);
    watchdogs[pin].begin(pinMatches[pin],SW_LOW,SW_HIGH,200,false);
    delay(300);
    duration = pulses[pin].readLowCounts();
    if (duration >180 && duration < 220)
    {
      pass(pin);
    }
    else
    {
      fail(pin);
      #ifdef PRINT_FAILURES
      Serial.printf("pin %d: expected %d, got %d\n",pin,100,duration);
      #endif
    }
    watchdogs[pin].begin(pinMatches[pin],SW_LOW,SW_LOW,100,false);
    watchdogs[pin].begin(pinMatches[pin],SW_HIGH,SW_LOW,100,false);
    delay(50);
    watchdogs[pin].updateResetCountdown(100);
    delay(200);
    duration = pulses[pin].readHighCounts();
    if (duration >140 && duration < 160)
    {
      pass(pin);
    }
    else
    {
      fail(pin);
      #ifdef PRINT_FAILURES
        Serial.printf("pin %d: expected %d, got %d\n",pin,100,duration);
      #endif
    }
   
  }
}
