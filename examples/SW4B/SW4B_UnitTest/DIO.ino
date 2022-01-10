void dioTest()
{

  resetAll();  // Serial Wombat chips are all inputs now.

  //Set pins to output, then back to input to make sure output to input pin changes work
  for (int i = 0; i < 4; ++ i)
  {
    SW6C.pinMode(i, OUTPUT);
    SW6C.pinMode(i, INPUT);
  }

  // Set 6D pins to outputs, 6E pin to output
  SW6D.pinMode (1, OUTPUT);
  SW6D.pinMode (2, OUTPUT);
  SW6D.pinMode (3, OUTPUT);
  SW6E.pinMode (2, OUTPUT);

  for (int i = 0; i < 0x10; ++i)
  {
    dioWrite(i);
    int result = dioRead();
    if (result == i)
    {
      pass(i);
    }
    else
    {
      #ifdef PRINT_FAILURES
      Serial.printf("Digital I/O expected (A): 0x%X, got 0x%X\r\n", i, result);
      #endif
      fail(i);
    }
  }
  for (int i = 0; i < 0x10; ++i)
  {
    dioPublicWrite(i);
    int result = dioRead();
    if (result == i)
    {
      pass(i);
    }
    else
    {
      #ifdef PRINT_FAILURES
      Serial.printf("Digital I/O expected (B): 0x%X, got 0x%X\r\n", i, result);
      #endif
      fail(i);
    }
  }
}

void dioWrite(int val)
{
  SW6E.digitalWrite(2, ((val & 0x01) > 0));
  SW6D.digitalWrite(3, ((val & 0x02) > 0));
  SW6D.digitalWrite(2, ((val & 0x04) > 0));
  SW6D.digitalWrite(1, ((val & 0x08) > 0));
}

void dioPublicWrite(int val)
{
  SW6E.writePublicData(2, ((val & 0x01) > 0));
  SW6D.writePublicData(3, ((val & 0x02) > 0));
  SW6D.writePublicData(2, ((val & 0x04) > 0));
  SW6D.writePublicData(1, ((val & 0x08) > 0));
}
int dioRead()
{
  return (
           SW6C.digitalRead(0) +
           (SW6C.digitalRead(1) << 1) +
           (SW6C.digitalRead(2) << 2) +
           (SW6C.digitalRead(3) << 3));
}
