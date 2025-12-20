SerialWombatUltrasonicDistanceSensor USDSensor(SW18AB_6B);
SerialWombatWatchdog usdWD(SW4B_6C);
SerialWombatServo usdsServo(SW18AB_6B);
SerialWombatPWM_4AB SWPWM10(SW4B_6C);

void usdSensorTest(uint8_t pin)
{

  resetAll();

  USDSensor.begin(pin, // Echo Pin, goes to SW6C pin 2
                  SerialWombatUltrasonicDistanceSensor::driver::HC_SR04,
                  pin+1, //Trigger Pin, goes to SW6C Pin 3
                  true, false);
  usdWD.begin(SW18ABPinTo4BPin(pin), SW_HIGH, SW_LOW, 20, false);

  delay(30);
  test("USDSensor_00", USDSensor.readPublicData(), 3400, 500);

  USDServoSweep(pin);
}

void USDServoSweep(uint8_t pin)
{
  uint8_t servoPin = pin + 2;
  resetAll();

  USDSensor.begin(pin, // Echo Pin, goes to SW6C pin 2
                  SerialWombatUltrasonicDistanceSensor::driver::HC_SR04,
                  pin+1, //Trigger Pin, goes to SW6C Pin 3
                  true, false);
                  
  initializePulseReaduS(SW18AB_6B,servoPin);

  SWPWM10.begin(SW18ABPinTo4BPin(pin));
  SWPWM10.setFrequency_SW4AB(SW4AB_PWMFrequency_488_Hz);
  SWPWM10.writePublicData(0x8000); // Setup PWM with frequency 1KHz to trick the echo to move the servof

  usdsServo.attach(servoPin, 1000, // Min
                   2600); //Max  // Chosen so that each each increment of the servo should increase it by 100uS (1/16 of range)
   USDSensor.configureServoSweep(servoPin, 0x0000, //memoryIndex
                                8 ,//Servo Positions
                                0x1000, // servoIncrement,
                                false, //Reverse
                                1000,// servoMoveDelay
                                1000);// uint16_t servoReturnDelay
  USDSensor.enableServoSweep();
  // Wait for servo reading to be between 1000 and 1100

  int trycount = 0;
  for (trycount = 0; trycount < 17; ++trycount)
  {
    uint16_t pulseTime = pulseRead(SW18AB_6B,servoPin);
      if (pulseTime > 930 && pulseTime < 1070)
      {
        break;
      }
      delay(515);
  }
  if (trycount == 17)
  {
    test("USDSensor_SS_00", 0, 1);
    return;
  }

  int i;
  uint32_t testtime = millis();
  for (i = 0; i < 8; ++i)
  {
    while (millis() < testtime) yield();
    uint16_t pulseTime = pulseRead(SW18AB_6B,servoPin);
      test("USDSensor_SS_01", pulseTime, 1000 + i * 100, 50);

    testtime += 1000;
  }

}
