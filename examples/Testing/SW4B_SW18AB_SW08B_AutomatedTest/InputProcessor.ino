SerialWombatPWM_18AB ipInput18(SW18AB_6B), ipInput8(SW8B_68);
SerialWombatProcessedInputPin processedInput18(SW18AB_6B), processedInput8(SW8B_68);
SerialWombatPWM_18AB* ipInputPtr;
SerialWombatProcessedInputPin* ipProcessedInputPtr;

void inputProcessorTest(SerialWombatChip &sw, uint8_t pin)
{
  if (&sw == &SW18AB_6B)
  {
    ipInputPtr = &ipInput18;
    ipProcessedInputPtr = &processedInput18;
  }
  else if (&sw == &SW8B_68)
  {
    ipInputPtr = &ipInput8;
    ipProcessedInputPtr = &processedInput8;
  }
  ipDisabledTest(pin);
  ipExclusionTest(pin);
  ipAverageTest(pin);
  ipMxbTest(pin);
}

void ipDisabledTest(uint8_t pin)
{
  resetAll();
  ipInputPtr->begin(pin + 1);
  ipProcessedInputPtr->begin(pin,pin + 1);

  for (int32_t i = 0; i < 65536; i += 13)
  {
    ipInputPtr->writePublicData(i);
    uint16_t result = ipProcessedInputPtr->readPublicData();
    test("IP_DIS_01", result, i);
    delay(0);
  }
  ipProcessedInputPtr->writeInverted(true);
  for (int32_t i = 0; i < 65536; i += 13)
  {
    ipInputPtr->writePublicData(i);
    uint16_t result = ipProcessedInputPtr->readPublicData();
    test("IP_DIS_02", result, i);
    delay(0);
  }
   ipProcessedInputPtr->writeProcessedInputEnable(true);
  for (int32_t i = 0; i < 65536; i += 13)
  {
    ipInputPtr->writePublicData(i);
    uint16_t result = ipProcessedInputPtr->readPublicData();
    test("IP_INV_01", result, 65535 - i);
    delay(0);
  }
  ipProcessedInputPtr->writeInverted(false);
  for (int32_t i = 0; i < 65536; i += 13)
  {
    ipInputPtr->writePublicData(i);
    uint16_t result = ipProcessedInputPtr->readPublicData();
    test("IP_INV_02", result, i);
    delay(0);
  }
  
}

void ipExclusionTest(uint8_t pin)
{
  resetAll();
  ipInputPtr->begin(pin + 1);
  ipProcessedInputPtr->begin(pin,pin + 1);
   ipProcessedInputPtr->writeProcessedInputEnable(true);
   ipInputPtr->writePublicData(12500);
   ipProcessedInputPtr->writeExcludeBelowAbove(20000,40000);
   
  for (int32_t i = 0; i < 19999; i += 13)
  {
    ipInputPtr->writePublicData(i);
    uint16_t result = ipProcessedInputPtr->readPublicData();
    test("IP_EX_01", result, 12500);
    delay(0);
  }
  uint16_t lastVal;
  for (int32_t i = 20000; i < 40001; i += 13)
  {
    ipInputPtr->writePublicData(i);
    lastVal = ipProcessedInputPtr->readPublicData();
    test("IP_EX_02", lastVal, i);
    delay(0);
  }
   for (int32_t i = 40001; i < 65536; i += 13)
  {
    ipInputPtr->writePublicData(i);
    uint16_t result = ipProcessedInputPtr->readPublicData();
    test("IP_EX_03", result, lastVal);
    delay(0);
  }

}

void ipAverageTest(uint8_t pin)
{

  resetAll();

    ipProcessedInputPtr->begin(pin,SerialWombatDataSource::SW_DATA_SOURCE_LFSR);


   ipProcessedInputPtr->writeAveragingNumberOfSamples(4000);

      ipProcessedInputPtr->writeProcessedInputEnable(true);

  delay(5000);

  uint16_t result = ipProcessedInputPtr->readAverage();
  test("IP_AVG_01", result, 32768, 500);  // Random should average out to 32768, will allow +/- 500

     ipProcessedInputPtr->writeExcludeBelowAbove(40000,60000);

   delay(30000);


   result = ipProcessedInputPtr->readAverage();
  test("IP_AVG_01", result, 50000, 500);  // Random should average out to 50000, will allow +/- 500
}

void ipMxbTest(uint8_t pin)
{
  resetAll();
  ipInputPtr->begin(pin + 1);
  ipProcessedInputPtr->begin(pin,pin + 1);
   ipProcessedInputPtr->writeProcessedInputEnable(true);
   
   ipProcessedInputPtr->writeTransformLinearMXB(5 * 256,32);
   
  for (int32_t i = 0; i < 65535; i += 13)
  {
    ipInputPtr->writePublicData(i);
    uint16_t result = ipProcessedInputPtr->readPublicData();
    int32_t expected = (int32_t)i * 5  + 32;
    if (expected > 65535)
    {
      expected = 65535;
    }
    test("IP_MXB_01", result, expected);
    delay(0);
  }
  
  ipProcessedInputPtr->writeTransformLinearMXB(5 * 256,-20000);
   
  for (int32_t i = 0; i < 65535; i += 13)
  {
    ipInputPtr->writePublicData(i);
    uint16_t result = ipProcessedInputPtr->readPublicData();
    int32_t expected = (int32_t)i * 5 - 20000;
    if (expected > 65535)
    {
      expected = 65535;
    }
    if (expected < 0)
    {
      expected = 0;
    }
    
    test("IP_MXB_02", result, expected);
    delay(0);
  }
   ipProcessedInputPtr->writeTransformLinearMXB(-5 * 256,100000);
   
  for (int32_t i = 0; i < 65535; i += 13)
  {
    ipInputPtr->writePublicData(i);
    uint16_t result = ipProcessedInputPtr->readPublicData();
    int32_t expected = (int32_t)i * -5  + 100000;
    if (expected > 65535)
    {
      expected = 65535;
    }
    if (expected < 0)
    {
      expected = 0;
    }
    
    test("IP_MXB_03", result, expected);
    delay(0);
  }
}
