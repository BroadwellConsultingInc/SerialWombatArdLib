void vSupplyTest()
{
  uint16_t supplyVoltage = SW6C.readSupplyVoltage_mV();
  if ((supplyVoltage > 3100 && supplyVoltage < 3400) || (supplyVoltage >4400 && supplyVoltage < 5200))
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
