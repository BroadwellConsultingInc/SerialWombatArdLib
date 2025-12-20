
SerialWombatThroughputConsumer tc18(SW18AB_6B);
void publicDataTest(SerialWombatChip &sw)
{
 
  {
    uint16_t number = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_INCREMENTING_NUMBER);

    for (int i = 1; i < 500; ++i)
    {
      if (sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_INCREMENTING_NUMBER) == number + i)
      {
        pass(1);
      }
      else
      {
        fail(1);
      }
    }
  }
  test("SW_DATA_SOURCE_1024mvCounts test not implemented",0);
  
  {
    
    uint16_t framesLSW = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_LSW);
    uint16_t framesMSW = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW);
    uint32_t startTime = millis();
    uint32_t startFrames = framesMSW;
    startFrames <<=16;
    startFrames += framesLSW;

    delay(100000);

    framesLSW = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_LSW);
    framesMSW = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW);
    uint32_t endTime = millis();

    uint32_t endFrames = framesMSW;
    endFrames <<=16;
    endFrames += framesLSW;
    int32_t netTime = endTime - startTime;
    int32_t netFrames = endFrames - startFrames;

    if (netTime < (( netFrames * 21) / 20)  && netTime > ((netFrames * 19) / 20))
    {
      pass(1);
    }
    else
    {
      fail(1);
    }
    
  }


if (&sw == &SW18AB_6B)
{
  tc18.begin(17);
    uint16_t number = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_OVERRUN_FRAMES);
    uint16_t numDroppedFrames = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_DROPPED_FRAMES);
    delay(100);
    uint16_t finalNumber = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_OVERRUN_FRAMES);
      if (finalNumber == number)
      {
        pass(1);
      }
      else
      {
        fail(1);
      }
      tc18.write(0,1200);
      
     number = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_OVERRUN_FRAMES);

    delay(100);
     finalNumber = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_OVERRUN_FRAMES);
      if (finalNumber >= number + 5)
      {
        pass(1);
      }
      else
      {
        fail(1);
      }
        tc18.begin(17);

         uint16_t finalNumDroppedFrames = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_DROPPED_FRAMES);
         if (numDroppedFrames == finalNumDroppedFrames)
      {
        pass(1);
      }
      else
      {
        fail(1);
      }
      numDroppedFrames = finalNumDroppedFrames;
      tc18.write(0,2200);

       delay(100);
      finalNumDroppedFrames = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_OVERRUN_FRAMES);
      if (finalNumDroppedFrames >= numDroppedFrames + 5)
      {
        pass(1);
      }
      else
      {
        fail(1);
      }

      tc18.begin(17);
      delay(1000);
      uint16_t systemUtilization = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_SYSTEM_UTILIZATION);
      tc18.writeAll(200);
      delay(1000);
      uint16_t systemUtilization2 = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_SYSTEM_UTILIZATION);
      tc18.begin(17);
      int32_t difference = systemUtilization2 - systemUtilization;
      if (difference > 10000 && difference < 16000)
      {
        pass(1);
      }
      else
      {
        Serial.print("SU: ");
        Serial.print(systemUtilization);
        Serial.print(" SU2: ");
                Serial.println(systemUtilization2);
        fail(1);
      }
  }
  test("SW_DATA_SOURCE_TEMPERATURE  test not implemented",0);
  {
    uint16_t number = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_PACKETS_RECEIVED);

    for (int i = 0; i < 10; ++i)
    {
      sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_INCREMENTING_NUMBER);
    }
    uint16_t finalNumber = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_PACKETS_RECEIVED);
      if (finalNumber == number + 11)
      {
        pass(1);
      }
      else
      {
        fail(1);
      }
    
  }
  if (&sw == &SW18AB_6B || &sw == &SW8B_68)
{
    uint16_t sourceErrorsStart = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_ERRORS);

    for (int i = 0; i < 10; ++i)
    {
      sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_INCREMENTING_NUMBER);
    }

    uint8_t tx[8] = {200, 50,0,0,0,0,0,0}; // 50 is an invalid pin number
    sw.sendPacket(tx);
    uint16_t sourceErrorsEnd = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_ERRORS);
      if (sourceErrorsEnd == sourceErrorsStart + 1)
      {
        pass(1);
      }
      else
      {
        fail(1);
      }
    
  }

  test("SW_DATA_SOURCE_LFSR  test not implemented",0);
  if (&sw == &SW18AB_6B)
  {
    uint16_t a = sw.readPublicData(SerialWombatDataSource::SW_DATA_COM_ADDRESS_LOW);
    test("SW_DATA_COM_ADDRESS_LOW SW18AB ",a,0x6B);

  }
  if (&sw == &SW8B_68)
  {
    uint16_t a = sw.readPublicData(SerialWombatDataSource::SW_DATA_COM_ADDRESS_LOW);
    test("SW_DATA_COM_ADDRESS_LOW SW18AB ",a,0x68);

  }
  test("SW_DATA_SOURCE_1024mvCounts test not implemented",0);
  test("SW_DATA_SOURCE_2HZ_SQUARE and similar  test not implemented",0);

  
  
}
