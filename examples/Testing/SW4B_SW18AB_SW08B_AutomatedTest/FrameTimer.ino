void frameTimerTest()
{
  resetAll();
  SerialWombatThroughputConsumer tc(SW18AB_6B);
   tc.begin(17);
  tc.writeAll(200);
  SW18AB_6B.writeFrameTimerPin(15);
  initializePulseReaduS(SW18AB_6B,15);
  delay(2000);
  uint16_t highTime = pulseRead(SW18AB_6B,15);
  uint32_t systemUtilization = SW18AB_6B.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_SYSTEM_UTILIZATION);
  test("FrameTimer_00", highTime, (systemUtilization * 1000 ) >>16 ,50);  

  tc.writeAll(400);
  delay(2000);
  highTime = pulseRead(SW18AB_6B,15);
  systemUtilization = SW18AB_6B.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_SYSTEM_UTILIZATION);
  test("FrameTimer_01", highTime, (systemUtilization * 1000 ) >>16 ,50);  


  resetAll();
  
}
