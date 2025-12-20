void vSupplyTest()
{
  uint16_t supplyVoltage = SW18AB_6B.readSupplyVoltage_mV();
  if ((supplyVoltage > 3100 && supplyVoltage < 3400))
  {
    pass(0);
  }
  else
  {
     Serial.print("VSupply Fail: Measured: ");
        Serial.println(supplyVoltage);
    fail(0);
  }
}
