void echoTest()
{
  uint32_t seed = 1;
  for (int i = 0; i < 5000; ++i)
  {
    if ((i & 0xFF) == 0)
    {
 
      updateDisplay();
    }
    bool passed = true;
    uint8_t tx[8];
    uint8_t rx[8];
 
    for (int b = 1; b < 8; ++b)
    {
      tx[b] = wrandom(&seed) & 0x1F;
    }
    tx[0] = '!';
    SW6B.sendPacket(tx, rx);
    if (rx[0] != '!')
    {
      passed = false;
    }

    for (int b = 1; b < 8; ++b)
    {
      if (tx[b] != rx[b])
      {
        passed = false;
      }
    }
    if (passed)
    {
      pass(i);
    }
    else
    {
      fail(i);
    }
  }
}
