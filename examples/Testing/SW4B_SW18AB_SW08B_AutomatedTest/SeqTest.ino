void SequenceTest()
{
  uint8_t resetSeq[] = {0xB4,'R','E','S','E','T',0x55,0x55};

  SW18AB_6B.sendPacket(resetSeq);

  uint8_t seqNum = 0; 
  for (int i = 0; i < 500; ++i)
  {
    uint8_t packet[8];
    packet[0] = 0xB4;
    
    for (int p = 1; p <=7; ++p)
    {
      packet[p] = seqNum;
      ++seqNum;
    }
    uint8_t rxPacket[8];
    SW18AB_6B.sendPacket(packet,rxPacket);
    for (int p = 0; p < 8; ++p)
    {
      if (packet[p] != rxPacket[p])
      {
        fail(i * 100 + p);
      }
      else
      {
        pass(i * 100 + p);
      }
    }
  }
}
