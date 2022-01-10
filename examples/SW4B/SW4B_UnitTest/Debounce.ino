void debounceTest()
{
  resetAll();
  for (int invert = 0; invert <= 1; ++ invert)
  {
    for (int pin = 0; pin < NUM_TEST_PINS; ++pin)
    {
      updateDisplay();
      MATCH_OUTPUT();
      MATCH_LOW();
      debounces[pin].begin(pin, 30, invert, false);
      delay(50);
      if (!(debounces[pin].digitalRead() ^ (bool)(invert)))
      {
        pass(1);
      }
      else
      {
        #ifdef PRINT_FAILURES
        Serial.printf("Fail debounce 1 pin %d, %d\r\n", pin, invert);
        #endif
        fail(1);
      }
      MATCH_HIGH();
      delay(50);
      if (debounces[pin].digitalRead() ^ (bool)(invert))
      {
        pass(2);
      }
      else
      {
        #ifdef PRINT_FAILURES
        Serial.printf("Fail debounce 2 pin %d, %d\r\n", pin, invert);
        #endif

        fail(2);
      }
      MATCH_LOW();
      if (debounces[pin].digitalRead() ^ (bool)(invert))
      {
        pass(3);
      }
      else
      {
        #ifdef PRINT_FAILURES
        Serial.printf("Fail debounce 3 pin %d, %d\r\n", pin, invert);
        #endif

        fail(3);
      }
      delay(50);
      if (!(debounces[pin].digitalRead() ^ (bool)(invert)))
      {
        pass(4);
      }
      else
      {
        #ifdef PRINT_FAILURES
        Serial.printf("Fail debounce 4 pin %d, %d\r\n", pin, invert);
        #endif
        fail(4);
      }
      if (invert)
      {
        MATCH_HIGH()
      }
      else
      {
        MATCH_LOW();
      }
      delay(100);
      if (invert)
      {
        MATCH_LOW();
      }
      else
      {
        MATCH_HIGH();
      }
      delay(100);
      uint16_t duration = debounces[pin].readDurationInTrueState_mS();
      if (duration > 60 && duration < 80)
      {

        pass(5);
      }
      else
      {
        #ifdef PRINT_FAILURES
        Serial.printf("Fail debounce 1 pin %d, %d\r\n", pin, invert);
        #endif

        fail(5);
      }
      if (debounces[pin].readTransitionsState() ) // No invert, because output was inverted above
      {

        pass(6);
      }
      else
      {
        #ifdef PRINT_FAILURES
        Serial.printf("Fail debounce 6 pin %d, %d\r\n", pin, invert);
        #endif

        fail(6);
      }
      if (debounces[pin].transitions != 3)
      {

        pass(7);
      }
      else
      {
        #ifdef PRINT_FAILURES
        Serial.printf("Fail debounce 7 pin %d, invert %d.  %d transitions\r\n", pin, invert, debounces[pin].transitions);
        #endif

        fail(7);
      }
    }
  }

}
