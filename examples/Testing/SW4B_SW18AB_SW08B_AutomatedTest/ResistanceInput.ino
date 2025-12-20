

void resistanceInputTest()
{
  SerialWombatResistanceInput resist16(SW18AB_6B);
  for (uint8_t i = 16; i <= 19; ++i)
  {
  analog1k(i);

  resist16.begin(i);
  delay(100);

  uint16_t result = resist16.readPublicData();
    char s[40];
    sprintf(s,"Resistance %d", i);
    test(s, result, 1000,100);
    analogShutdown();
   
    
  }
  
}
