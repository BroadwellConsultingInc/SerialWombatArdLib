
#define PO_WRITE_VALUE(_VALUE) {wombatMatches[pin]->writePublicData(0,_VALUE);}
#define PO_READ_VALUE() wombatMatches[pin]->readPublicData(0)
void protectedOutputTest()
{
  for (int pin = 0; pin < NUM_TEST_PINS ; ++pin)
  {
    updateDisplay();
    resetAll();

    { // Make pin 0 on driving Serial Wombat chip a servo.  We can set the public data here.
      uint8_t tx[] = { 200, 0, PIN_MODE_SERVO, 0, 0x55, 0x55, 0x55, 0x55};
      wombatMatches[pin]->sendPacket(tx);
    }
    protecteds[pin].begin(pinMatches[pin], 0);
    {
      PO_WRITE_VALUE(0);
      delay(2);

      protecteds[pin].configure(PO_FAULT_IF_NOT_EQUAL, 0, 100, SW_LOW, SW_HIGH);
      delay(2);
      if (!wombatsByPin[pin]->digitalRead(pin))
      {
        pass(0);
      }
      else
      {
        fail(0);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read low, got high at test 0\n", pin);
        #endif
      }
      PO_WRITE_VALUE(1);
      delay(2);
      if (!wombatsByPin[pin]->digitalRead(pin))  // Still low until debounce
      {
        pass(1);
      }
      else
      {
        fail(1);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read low, got high at test 1\n", pin);
        #endif
      }
      delay(110);

      PO_READ_VALUE();
      if (wombatsByPin[pin]->digitalRead(pin))
      {

        pass(2);
      }
      else
      {
        fail(2);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read High, got low at test 2\n", pin);
        #endif
      }
    }

    {
      PO_WRITE_VALUE(0);
      delay(2);

      protecteds[pin].configure(PO_FAULT_IF_NOT_EQUAL, 0, 100, SW_HIGH, SW_LOW);
      delay(2);
      if (wombatsByPin[pin]->digitalRead(pin))
      {
        pass(3);
      }
      else
      {
        fail(3);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read high, got low at test 3\n", pin);
        #endif
      }
      PO_WRITE_VALUE(1);
      delay(2);
      if (wombatsByPin[pin]->digitalRead(pin))  // Still high until debounce
      {
        pass(4);
      }
      else
      {
        fail(4);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read high, got low at test 4\n", pin);
        #endif
      }
      delay(110);

      PO_READ_VALUE();
      if (!wombatsByPin[pin]->digitalRead(pin))
      {

        pass(5);
      }
      else
      {
        fail(5);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read High, got low at test 5\n", pin);
        #endif
      }
    }

    {
      PO_WRITE_VALUE(0x70FF);
      delay(2);

      protecteds[pin].configure(PO_FAULT_IF_FEEDBACK_GREATER_THAN_EXPECTED, 0x8000, 100, SW_HIGH, SW_LOW);
      delay(2);
      if (wombatsByPin[pin]->digitalRead(pin))
      {
        pass(6);
      }
      else
      {
        fail(6);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read high, got low at test 6\n", pin);
        #endif
      }
      PO_WRITE_VALUE(0x9000);
      delay(2);
      if (wombatsByPin[pin]->digitalRead(pin))  // Still high until debounce
      {
        pass(7);
      }
      else
      {
        fail(7);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read high, got low at test 7\n", pin);
        #endif
      }
      delay(110);

      PO_READ_VALUE();
      if (!wombatsByPin[pin]->digitalRead(pin))
      {

        pass(8);
      }
      else
      {
        fail(8);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read High, got low at test 8\n", pin);
        #endif
      }
    }

    {
      PO_WRITE_VALUE(0x9000);
      delay(2);

      protecteds[pin].configure(PO_FAULT_IF_FEEDBACK_LESS_THAN_EXPECTED, 0x8000, 100, SW_HIGH, SW_LOW);
      delay(2);
      if (wombatsByPin[pin]->digitalRead(pin))
      {
        pass(9);
      }
      else
      {
        fail(9);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read high, got low at test 9\n", pin);
        #endif
      }
      PO_WRITE_VALUE(0x7000);
      delay(2);
      if (wombatsByPin[pin]->digitalRead(pin))  // Still high until debounce
      {
        pass(10);
      }
      else
      {
        fail(10);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read high, got low at test 10\n", pin);
        #endif
      }
      delay(110);

      PO_READ_VALUE();
      if (!wombatsByPin[pin]->digitalRead(pin))
      {

        pass(11);
      }
      else
      {
        fail(11);
        #ifdef PRINT_FAILURES
        Serial.printf("Pin %d: Expected pin read High, got low at test 11\n", pin);
        #endif
      }
    }
  }
}
