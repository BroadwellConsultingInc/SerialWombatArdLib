#include <stdint.h>
void echoTest(SerialWombatChip& sw)
{
  if (sw.isPinModeSupported(PIN_MODE_THROUGHPUT_CONSUMER))  
  {
    // Push the clock stretching....
    SerialWombatThroughputConsumer swtc(sw);
    swtc.begin(0);
    for (int i = 0; i < 16; ++i)
    {
      swtc.write(i,i * 40);
    }
    
  }
  uint32_t seed = 1;
  for (int i = 0; i < 5000; ++i)
  {
   
    bool passed = true;
    uint8_t tx[8];
    uint8_t rx[8];
 
    for (int b = 1; b < 8; ++b)
    {
      tx[b] = wrandom(&seed) & 0x1F;
    }
    tx[0] = '!';
    sw.sendPacket(tx, rx);
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
