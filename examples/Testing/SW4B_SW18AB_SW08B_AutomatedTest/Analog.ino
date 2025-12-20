//Adafruit_MCP4728 volt16171819;
//Adafruit_MCP4728 volt00070906;
#define MCP4728_ADDR 0x60
SerialWombatAnalogInput analog16(SW18AB_6B), analog17(SW18AB_6B), analog18(SW18AB_6B), analog19(SW18AB_6B);
SerialWombatAnalogInput analog16SW4B(SW4B_6F), analog17SW4B(SW4B_6E), analog18SW4B(SW4B_6E), analog19SW4B(SW4B_6E);
SerialWombatAnalogInput analog0SW8B(SW8B_68), analog1SW8B(SW8B_68), analog2SW8B(SW8B_68), analog3SW8B(SW8B_68);
SerialWombatAnalogInput analog4SW8B(SW8B_68), analog5SW8B(SW8B_68), analog6SW8B(SW8B_68), analog7SW8B(SW8B_68);


SerialWombatAnalogInput* SW8BAnalogs[] = {
  &analog0SW8B,
  &analog1SW8B,
  &analog2SW8B,
  &analog3SW8B,
  &analog4SW8B,
  &analog5SW8B,
  &analog6SW8B,
  &analog7SW8B
};

SerialWombatAnalogInput* SW18ABAnalogs[] = {
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog16,
  &analog17,
    &analog18,
      &analog19,
};

uint16_t wrandom(uint32_t *seed);

uint32_t analogSeed = 1;
void setAnalogRatio(uint8_t pin, uint16_t ratio)
{
  switch (pin)
  {
    case 0:
      softWire.beginTransmission(MCP4728_ADDR);
      softWire.write(0x40);
      softWire.write((uint8_t)(ratio >> 12));
      softWire.write((uint8_t)(ratio >> 4));
      softWire.endTransmission();
      break;

    case 7:
      softWire.beginTransmission(MCP4728_ADDR);
      softWire.write(0x42);
      softWire.write((uint8_t)(ratio >> 12));
      softWire.write((uint8_t)(ratio >> 4));
      softWire.endTransmission();
      break;

    case 9:
      softWire.beginTransmission(MCP4728_ADDR);
      softWire.write(0x44);
      softWire.write((uint8_t)(ratio >> 12));
      softWire.write((uint8_t)(ratio >> 4));
      softWire.endTransmission();
      break;

    case 6:
      softWire.beginTransmission(MCP4728_ADDR);
      softWire.write(0x46);
      softWire.write((uint8_t)(ratio >> 12));
      softWire.write((uint8_t)(ratio >> 4));
      softWire.endTransmission();
      break;


    case 16:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x40);
      Wire.write((uint8_t)(ratio >> 12));
      Wire.write((uint8_t)(ratio >> 4));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_A, ratio >> 4, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_NORMAL);
      break;

    case 17:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x42);
      Wire.write((uint8_t)(ratio >> 12));
      Wire.write((uint8_t)(ratio >> 4));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_B, ratio >> 4, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_NORMAL);
      break;

    case 18:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x44);
      Wire.write((uint8_t)(ratio >> 12));
      Wire.write((uint8_t)(ratio >> 4));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_C, ratio >> 4, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_NORMAL);
      break;

    case 19:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x46);
      Wire.write((uint8_t)(ratio >> 12));
      Wire.write((uint8_t)(ratio >> 4));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_D, ratio >> 4, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_NORMAL);
      break;
  }

}

void analogShutdown()
{
  /*
    volt16171819.begin();
    volt16171819.setChannelValue(MCP4728_CHANNEL_A, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_500K);
    volt16171819.setChannelValue(MCP4728_CHANNEL_B, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_500K);
    volt16171819.setChannelValue(MCP4728_CHANNEL_C, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_500K);
    volt16171819.setChannelValue(MCP4728_CHANNEL_D, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_500K);
  */
  for (uint8_t x = 0x40; x <= 0x46; x += 2)
  {
    softWire.beginTransmission(MCP4728_ADDR);
    softWire.write(x);
    softWire.write(0x60);
    softWire.write(0x00);
    softWire.endTransmission();


    Wire.beginTransmission(MCP4728_ADDR);
    Wire.write(x);
    Wire.write(0x60);
    Wire.write(0x00);
    Wire.endTransmission();
  }

}

void analog1k(uint8_t pin)
{
  switch (pin)
  {
    case 16:

      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x40);
      Wire.write((uint8_t)(0x20));  //TODO Double check this...
      Wire.write((uint8_t)(0));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_A, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_1K);
      break;

    case 17:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x42);
      Wire.write((uint8_t)(0x20));  //TODO Double check this...
      Wire.write((uint8_t)(0));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_B, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_1K);
      break;

    case 18:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x44);
      Wire.write((uint8_t)(0x20));  //TODO Double check this...
      Wire.write((uint8_t)(0));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_C, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_1K);
      break;

    case 19:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x46);
      Wire.write((uint8_t)(0x20));  //TODO Double check this...
      Wire.write((uint8_t)(0));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_D, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_1K);
      break;
  }

}

void analogInputTest(SerialWombatChip &sw)
{
  resetAll();



  analogShutdown();
    if (&sw == &SW18AB_6B)
  {
    analogInputTest_SW18AB();
    return;
  }
  if (&sw == &SW8B_68)
  {
    analogInputTest_SW8B();
    return;
  }
  if (&sw == &SW18AB_6B)
  {
    analog16.begin(16);
    analog17.begin(17);
    analog18.begin(18);
    analog19.begin(19);
  }
  if (&sw == &SW4B_6C)
  {
    analog16SW4B.begin(1);
    analog17SW4B.begin(3);
    analog18SW4B.begin(2);
    analog19SW4B.begin(1);
  }
  for (uint32_t i = 0; i < 65535; i += 1024)
  {
    uint16_t ratio = i;
    setAnalogRatio(16, ratio);
    setAnalogRatio(17, ratio + 15000);
    setAnalogRatio(18, ratio + 30000);
    setAnalogRatio(19, ratio + 45000);

    delay(100);


    uint16_t readResult;

    for (uint8_t x = 0; x < 100; ++x)
    {
      if (&sw == &SW18AB_6B)
      {
        SW18AB_6B.readTemperature_100thsDegC();
        readResult = analog16.readCounts();
        test("ANALOGIN_COUNTS_16", readResult, ratio, 256, 3);

        readResult = analog16.readAveragedCounts();
        test("ANALOGIN_AVERAGE_16", readResult, ratio, 256, 3);
      }
      if (&sw == &SW4B_6C)
      {
        readResult = analog16SW4B.readCounts();
        test("ANALOGIN_COUNTS_16_4B", readResult, ratio, 256, 3);

        readResult = analog16SW4B.readAveragedCounts();
        test("ANALOGIN_AVERAGE_16_4B", readResult, ratio, 256, 3);

      }

      if (&sw == &SW8B_68)
      {
        readResult = analog16SW4B.readCounts();
        test("ANALOGIN_COUNTS_16_4B", readResult, ratio, 256, 3);

        readResult = analog16SW4B.readAveragedCounts();
        test("ANALOGIN_AVERAGE_16_4B", readResult, ratio, 256, 3);

      }
      delay(0);
    }

    ratio += 15000;

    for (uint8_t x = 0; x < 100; ++x)
    {
      if (&sw == &SW18AB_6B)
      {
        SW18AB_6B.readTemperature_100thsDegC();
        readResult = analog17.readCounts();
        test("ANALOGIN_COUNTS_17", readResult, ratio, 256, 3);

      }
      if (&sw == &SW4B_6C)
      {
        readResult = analog17SW4B.readCounts();
        test("ANALOGIN_COUNTS_17_4B", readResult, ratio, 256, 3);

      }
      delay(0);
    }
    ratio += 15000;
    for (uint8_t x = 0; x < 100; ++x)
    {
      if (&sw == &SW18AB_6B)
      {
        SW18AB_6B.readTemperature_100thsDegC();
        readResult = analog18.readCounts();
        test("ANALOGIN_COUNTS_18", readResult, ratio, 256, 3);
      }
      if (&sw == &SW4B_6C)
      {
        readResult = analog18SW4B.readCounts();
        test("ANALOGIN_COUNTS_18_4B", readResult, ratio, 256, 3);
      }
      delay(0);
    }



    ratio += 15000;
    for (uint8_t x = 0; x < 100; ++x)
    {
      if (&sw == &SW18AB_6B)
      {
        SW18AB_6B.readTemperature_100thsDegC();
        readResult = analog19.readCounts();
        test("ANALOGIN_COUNTS_19", readResult, ratio, 256, 3);
      }

      if (&sw == &SW4B_6C)
      {
        readResult = analog19SW4B.readCounts();
        test("ANALOGIN_COUNTS_10_4B", readResult, ratio, 256, 3);
      }
      delay(0);
    }
    delay(100);
  }

 

}

void analogInputTest_SW8B()
{
  for (uint8_t i = 0; i < 8; ++i)
  {
    SW8BAnalogs[i]->begin(i);
  }

  for (uint32_t i = 0; i < 65535; i += 100)
  {
    uint16_t adc = i;

    for (uint8_t i = 0; i < 8; ++i)
    {
      setAnalogRatio(SW8BPinTo18ABPin(i), adc + 0x1800 * i);
    }
    delay(100);
    for (uint8_t i = 0; i < 8; ++i)
    {

      uint16_t readResult = SW8BAnalogs[i]->readPublicData();
      test("ANALOGIN_COUNTS_8B", readResult,  adc + 0x1800 * i, 256, 3);
      readResult = SW8BAnalogs[i]->readAveragedCounts();
      test("ANALOGIN_COUNTS_8B_AVG", readResult,  adc + 0x1800 * i, 256, 3);
    }


  }
   //void analogMaxMinTest()
{
  setAnalogRatio(SW8BPinTo18ABPin(4), 0x8000);
  SW8BAnalogs[4]->readMaximumCounts(true);

  setAnalogRatio(SW8BPinTo18ABPin(4), 0x4000);
  delay(100);
  setAnalogRatio(SW8BPinTo18ABPin(4), 0xC000);
  delay(100);
  uint16_t minimum = SW8BAnalogs[4]->readMinimumCounts();
  test("ANALOGIN_MIN_8", minimum, 0x4000, 256, 3);

  uint16_t maximum = SW8BAnalogs[4]->readMaximumCounts();
  test("ANALOGIN_MAX_8", maximum, 0xC000, 256, 3);
}

//void analogAverageTest()
{
  setAnalogRatio(SW8BPinTo18ABPin(4), 0x0000);
  SW8BAnalogs[4]->begin(SW8BPinTo18ABPin(4), 10000);
  for (int i = 0; i < 60; ++i)
  {
    setAnalogRatio(SW8BPinTo18ABPin(4), 0x4000);
    delay(100);
    setAnalogRatio(SW8BPinTo18ABPin(4), 0xC000);
    delay(100);
  }

  uint16_t average = SW8BAnalogs[4]->readAveragedCounts();
  test("ANALOGIN_AVG_8", average, 0x4000, 256, 3);
}


//void analogFilterTest()
{
  setAnalogRatio(SW8BPinTo18ABPin(4), 0xFFFF);
  delay(100);
  SW8BAnalogs[4]->begin(SW8BPinTo18ABPin(4), 10000, 65445);
  setAnalogRatio(SW8BPinTo18ABPin(4), 0);
  delay(500);

  uint16_t result = SW8BAnalogs[4]->readFilteredCounts();
  test("ANALOGIN_Filter_8", result, 0x8000, 256, 3);
  delay(500);
  result = SW8BAnalogs[4]->readFilteredCounts();
  test("ANALOGIN_Filter_8_2", result, 0x4000, 256, 3);
}
}

void analogInputTest_SW18AB()
{
  for (uint8_t i = 16; i <= 19; ++i)
  {
    SW18ABAnalogs[i]->begin(i);
  }

  for (uint32_t i = 0; i < 65535; i += 100)
  {
    uint16_t adc = i;

    for (uint8_t i = 16;  i<= 19; ++i)
    {
      setAnalogRatio(i, adc + 0x1800 * i);
    }
    delay(200);
    for (uint8_t i = 16; i <= 19; ++i)
    {

      uint16_t readResult = SW18ABAnalogs[i]->readPublicData();
      test("ANALOGIN_COUNTS_18AB", readResult,  adc + 0x1800 * i, 256, 3);
      readResult = SW18ABAnalogs[i]->readAveragedCounts();
      test("ANALOGIN_COUNTS_18AB_AVG", readResult,  adc + 0x1800 * i, 256, 3);
    }


  }
  //void analogMaxMinTest()
{
  setAnalogRatio(16, 0x8000);
  analog16.readMaximumCounts(true);

  setAnalogRatio(16, 0x4000);
  delay(100);
  setAnalogRatio(16, 0xC000);
  delay(100);
  uint16_t minimum = analog16.readMinimumCounts();
  test("ANALOGIN_MIN_16", minimum, 0x4000, 256, 3);

  uint16_t maximum = analog16.readMaximumCounts();
  test("ANALOGIN_MAX_16", maximum, 0xC000, 256, 3);
}

//void analogAverageTest()
{
  setAnalogRatio(16, 0x0000);
  analog16.begin(16, 10000);
  for (int i = 0; i < 60; ++i)
  {
    setAnalogRatio(16, 0x4000);
    delay(100);
    setAnalogRatio(16, 0xC000);
    delay(100);
  }

  uint16_t average = analog16.readAveragedCounts();
  test("ANALOGIN_AVG_16", average, 0x4000, 256, 3);
}


//void analogFilterTest()
{
  setAnalogRatio(16, 0xFFFF);
  delay(100);
  analog16.begin(16, 10000, 65445);
  setAnalogRatio(16, 0);
  delay(500);

  uint16_t result = analog16.readFilteredCounts();
  test("ANALOGIN_Filter_16", result, 0x8000, 256, 3);
  delay(500);
  result = analog16.readFilteredCounts();
  test("ANALOGIN_Filter_16_2", result, 0x4000, 256, 3);
}
}
