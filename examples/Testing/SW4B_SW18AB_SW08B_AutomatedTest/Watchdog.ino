SerialWombatWatchdog swwd(SW18AB_6B);
void watchdogTest18AB()
{
  for (int pin = 11; pin < NUM_TEST_PINS; ++pin)
  {
        resetAll();
       initializePulseReaduS(SW18AB_6B,pin);

    swwd.begin(pin,SW_LOW,SW_LOW,40,false);
    swwd.begin(pin,SW_HIGH,SW_LOW,40,false);
    delay(200);
    uint16_t duration = pulseRead(SW18AB_6B,pin);
    if (duration >30000 && duration < 50000)
    {
      pass(pin);
    }
    else
    {
      fail(pin);
      //Serial.printf("pin %d: expected %d, got %d\n",pin,100,duration);
    }    
  }
}
