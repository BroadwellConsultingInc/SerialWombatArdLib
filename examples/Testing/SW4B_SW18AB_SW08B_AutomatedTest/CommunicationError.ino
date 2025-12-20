
void CommunicationErrorTest(SerialWombatChip& sw)
{
  uint16_t priorErrorCount = sw.errorCount;
  {
    uint8_t tx[] = {200,63,1,0x55,0x55,0x55,0x55,0x55}; // Should trigger invalid pin
    sw.sendPacket(tx);

    if (sw.errorCount != priorErrorCount + 1)
    {
      fail(1);
    }
    else
    {
      pass(1);
    }
  }
}
