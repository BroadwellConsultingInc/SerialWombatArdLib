SerialWombatPWM_18AB scalingInput18AB(SW18AB_6B), scalingOutput18AB(SW18AB_6B), scalingInput8B(SW8B_68), scalingOutput8B(SW8B_68);


#define SCALING_INPUT_PIN 5 //18
#define SCALING_OUTPUT_PIN 6 //19
#define SCALING_ENDSTOP0_PIN 7
#define SCALING_ENDSTOP1_PIN 0

SerialWombatPWM_18AB  *scalingInput, *scalingOutput;
SerialWombatChip* scalingSWC;
void scalingTest(SerialWombatChip &sw)
{
  scalingSWC = &sw;
   if (&sw ==  &SW18AB_6B)
   {
   scalingInput = &scalingInput18AB;
   scalingOutput = &scalingOutput18AB;
   
   }
   else
   {
       scalingInput = &scalingInput8B;
   scalingOutput = &scalingOutput8B;
   }
    scalingTimeoutTest();
    scalingEndstopTest();

    scalingInputScalingTest();
    scalingInvertScalingTest();
    scalingOutputScalingTest();

    scalingRateControl16HzTest();
    scaling1stOrderTest();
    scalingHysteresisTest();

  scalingSOLinearInterpolationTest();

  if (0)
  { //PID
    scalingInput->begin(SCALING_INPUT_PIN);
    scalingOutput->begin(SCALING_OUTPUT_PIN);
    scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);

    uint16_t kp = 0x100;  //256.00
    uint16_t ki = 0x0; //0
    uint16_t kd = 0;
    uint16_t target = 10000;
    uint16_t processOutput = 8000;

    scalingOutput->writePID(kp, ki, kd, target, SerialWombatAbstractScaledOutput::Period::PERIOD_128mS);
    scalingInput->writePublicData(processOutput);
    scalingOutput->writeScalingEnabled(true, SCALING_INPUT_PIN);

    test("SCALE_PID_01", scalingOutput->readPublicData(), ((uint32_t)target - processOutput) * kp >> 8);

    kp = 200;
    ki = 300;
    scalingOutput->writePID(kp, ki, kd, target, SerialWombatAbstractScaledOutput::Period::PERIOD_128mS);
    uint16_t result = 0;
    while (result < 65535)
    {
      result = scalingOutput->readPublicData();
      int32_t process = result - 16000;
      if (process < 0)
      {
        process = 0;
      }

      scalingInput->writePublicData((uint16_t)process);
      Serial.print(result);
      Serial.print(' ');
      Serial.println(process);
    }
  }
}

void scalingTimeoutTest()
{
  //Timeout Test
  resetAll();

  scalingInput->begin(SCALING_INPUT_PIN);
  scalingOutput->begin(SCALING_OUTPUT_PIN);
  scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);
  scalingOutput->writeScalingEnabled(false, SCALING_OUTPUT_PIN);
  scalingOutput->writeScalingEnabled(true, SCALING_OUTPUT_PIN);
  scalingSWC->writePublicData(SCALING_OUTPUT_PIN, 0x0000);
  scalingOutput->writeTimeout(1000, 0x8000);
  uint32_t startTime = millis();
  while (millis() < startTime + 900)
  {
    if (scalingOutput->readLastOutputValue() == 0)
    {
      pass(0);
    }
    else
    {
      fail(1);
      Serial.println("F0");
    }
    delay(100);
  }
  delay(200);
  uint16_t v = scalingOutput->readLastOutputValue();
  uint16_t x = 0x8000;
  if ( v == x)
  {
    pass(0);
  }
  else
  {
    fail(2);
    Serial.print("F0. ");  Serial.print(" V: "); Serial.print(v); Serial.print(" X:"); Serial.println(x);
  }


}

void scalingEndstopTest()
{
  // Endstop tests.  Endstop processing occurs after communication timeout
  // processing and before output filtering.
  resetAll();

  scalingInput->begin(SCALING_ENDSTOP0_PIN);
  scalingInput->begin(SCALING_ENDSTOP1_PIN);
  scalingInput->begin(SCALING_INPUT_PIN);
  scalingOutput->begin(SCALING_OUTPUT_PIN);
  scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);

  const uint16_t normalOutput = 0x3456;
  const uint16_t endstop0Trigger = 10000;
  const uint16_t endstop0Output = 0x1111;
  const uint16_t endstop1Trigger = 50000;
  const uint16_t endstop1Output = 0xEEEE;

  scalingInput->writePublicData(normalOutput);
  scalingSWC->writePublicData(SCALING_ENDSTOP0_PIN, endstop0Trigger + 1); // Inactive for <= comparison
  scalingSWC->writePublicData(SCALING_ENDSTOP1_PIN, endstop1Trigger - 1); // Inactive for >= comparison

  scalingOutput->writeEndstops(
    SCALING_ENDSTOP0_PIN, endstop0Trigger, endstop0Output, false,
    SCALING_ENDSTOP1_PIN, endstop1Trigger, endstop1Output, true);
  scalingOutput->writeScalingEnabled(true, SCALING_INPUT_PIN);

  // Neither endstop active: normal scaled output passes through.
  test("SCALE_END_01", scalingOutput->readPublicData(), normalOutput);

  // Endstop 0 uses <= comparison.  Test both below and exactly equal to the threshold.
  scalingSWC->writePublicData(SCALING_ENDSTOP0_PIN, endstop0Trigger - 1);
  test("SCALE_END_02", scalingOutput->readPublicData(), endstop0Output);

  scalingSWC->writePublicData(SCALING_ENDSTOP0_PIN, endstop0Trigger);
  test("SCALE_END_03", scalingOutput->readPublicData(), endstop0Output);

  scalingSWC->writePublicData(SCALING_ENDSTOP0_PIN, endstop0Trigger + 1);
  test("SCALE_END_04", scalingOutput->readPublicData(), normalOutput);

  // Endstop 1 uses >= comparison.  Test both above and exactly equal to the threshold.
  scalingSWC->writePublicData(SCALING_ENDSTOP1_PIN, endstop1Trigger + 1);
  test("SCALE_END_05", scalingOutput->readPublicData(), endstop1Output);

  scalingSWC->writePublicData(SCALING_ENDSTOP1_PIN, endstop1Trigger);
  test("SCALE_END_06", scalingOutput->readPublicData(), endstop1Output);

  scalingSWC->writePublicData(SCALING_ENDSTOP1_PIN, endstop1Trigger - 1);
  test("SCALE_END_07", scalingOutput->readPublicData(), normalOutput);

  // If both endstops are active, endstop 0 has priority.
  scalingSWC->writePublicData(SCALING_ENDSTOP0_PIN, endstop0Trigger);
  scalingSWC->writePublicData(SCALING_ENDSTOP1_PIN, endstop1Trigger);
  test("SCALE_END_08", scalingOutput->readPublicData(), endstop0Output);

  // The default 0xFF pin disables both endstops even when the old monitored
  // pins still contain values which would have activated them.
  scalingOutput->writeEndstops();
  test("SCALE_END_09", scalingOutput->readPublicData(), normalOutput);

  // Verify ordering relative to the communication timeout.  Once the timeout
  // expires its output is used, but an active endstop must override it because
  // endstop processing occurs after timeout processing.
  resetAll();
  scalingInput->begin(SCALING_ENDSTOP0_PIN);
  scalingInput->begin(SCALING_ENDSTOP1_PIN);
  scalingInput->begin(SCALING_INPUT_PIN);
  scalingOutput->begin(SCALING_OUTPUT_PIN);
  scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);

  const uint16_t timeoutNormalOutput = 0x2222;
  const uint16_t timeoutValue = 0x4444;
  const uint16_t timeoutEndstopTrigger = 0x0800;
  const uint16_t timeoutEndstopOutput = 0x3333;
   
  scalingSWC->writePublicData(SCALING_ENDSTOP0_PIN, timeoutEndstopTrigger + 1);
  scalingOutput->writeEndstops(
    SCALING_ENDSTOP0_PIN, timeoutEndstopTrigger, timeoutEndstopOutput, false);
  scalingOutput->writeTimeout(20, timeoutValue);
  scalingOutput->writeScalingEnabled(true, SCALING_INPUT_PIN);
  scalingInput->writePublicData(timeoutNormalOutput);
  delay(30);
  test("SCALE_END_10", scalingOutput->readPublicData(), timeoutValue);

  scalingSWC->writePublicData(SCALING_ENDSTOP0_PIN, timeoutEndstopTrigger);
  test("SCALE_END_11", scalingOutput->readPublicData(), timeoutEndstopOutput);

  // Verify ordering relative to output filtering.  The endstop substitutes a
  // value before filtering, so rate limiting should ramp toward the endstop
  // output instead of jumping directly to it.
  resetAll();
  scalingInput->begin(SCALING_ENDSTOP0_PIN);
  scalingInput->begin(SCALING_ENDSTOP1_PIN);
  scalingInput->begin(SCALING_INPUT_PIN);
  scalingOutput->begin(SCALING_OUTPUT_PIN);
  scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);
  scalingInput->writePublicData(0);
  scalingSWC->writePublicData(SCALING_ENDSTOP0_PIN, 0xFFFF); // Inactive
  scalingOutput->writeRateControl(SerialWombatAbstractScaledOutput::Period::PERIOD_64mS, 100);
  scalingOutput->writeEndstops(SCALING_ENDSTOP0_PIN, 0x1000, 1000, false);
  scalingOutput->writeScalingEnabled(true, SCALING_INPUT_PIN);

  scalingSWC->writePublicData(SCALING_ENDSTOP0_PIN, 0x1000); // Activate at equality
  uint32_t timeout = millis() + 10000;
  while (scalingOutput->readPublicData() == 0 && millis() < timeout);
  test("SCALE_END_12", scalingOutput->readPublicData(), 100);
  delay(64);
  test("SCALE_END_13", scalingOutput->readPublicData(), 200);
}

void scalingInputScalingTest()
{ //Input Scaling Test
  resetAll();

  scalingInput->begin(SCALING_INPUT_PIN);
  scalingOutput->begin(SCALING_OUTPUT_PIN);
  uint16_t lowLimit = 3000;
  uint16_t highLimit = 50000;

  scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);
  scalingOutput->writeScalingEnabled(true, SCALING_INPUT_PIN);

  for (uint32_t i = 0; i < 65536; i += 10)
  {

    scalingSWC->writePublicData(SCALING_INPUT_PIN, i);
    scalingOutput->writeInputScaling(lowLimit, highLimit);
    delay(10);
    uint16_t expected = 0;
    if (i > lowLimit)
    {
      if (i > highLimit)
      {
        expected = 65535;
      }
      else
      {
        expected = (uint16_t) (((i - lowLimit) / (float)(highLimit - lowLimit)) * 65535);
      }
    }
    uint16_t value = scalingOutput->readPublicData();

    if (withinRange(value, expected, 0, 1))
    {
      pass(1);
    }
    else
    {
      fail(1);
      Serial.print("F1. i: "); Serial.print(i); Serial.print(" V: "); Serial.print(value); Serial.print(" X:"); Serial.println(expected);
    }
  }

}

void scalingInvertScalingTest()
{ //Invert Scaling Test
  scalingInput->begin(SCALING_INPUT_PIN);
  scalingOutput->begin(SCALING_OUTPUT_PIN);
  uint16_t lowLimit = 3000;
  uint16_t highLimit = 50000;
  scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);
  scalingOutput->writeScalingEnabled(true, SCALING_INPUT_PIN);
  scalingOutput->writeScalingInvertedInput(true);

  for (uint32_t i = 0; i < 65536; i += 10)
  {

    scalingSWC->writePublicData(SCALING_INPUT_PIN, i);
    scalingOutput->writeOutputScaling(lowLimit, highLimit);
    delay(10);
    uint16_t expected = 0;


    expected = (uint16_t) ((highLimit - lowLimit) * (float)(65535 - i) / 65535 + lowLimit);

    uint16_t value = scalingOutput->readPublicData();

    if (withinRange(value, expected, 0, 1))
    {
      pass(1);
    }
    else
    {
      fail(1);
      Serial.print("F2. i: "); Serial.print(i); Serial.print(" V: "); Serial.print(value); Serial.print(" X:"); Serial.println(expected);
    }
  }
}

void scalingOutputScalingTest()
{ //Output Scaling Test
  scalingInput->begin(SCALING_INPUT_PIN);
  scalingOutput->begin(SCALING_OUTPUT_PIN);
  uint16_t lowLimit = 3000;
  uint16_t highLimit = 50000;
  scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);
  scalingOutput->writeScalingEnabled(true, SCALING_INPUT_PIN);

  for (uint32_t i = 0; i < 65536; i += 10)
  {

    scalingSWC->writePublicData(SCALING_INPUT_PIN, i);
    scalingOutput->writeOutputScaling(lowLimit, highLimit);
    delay(10);
    uint16_t expected = 0;


    expected = (uint16_t) ((highLimit - lowLimit) * (float)(i) / 65535 + lowLimit);

    uint16_t value = scalingOutput->readPublicData();

    if (withinRange(value, expected, 0, 1))
    {
      pass(1);
    }
    else
    {
      fail(1);
      Serial.print("F3. i: "); Serial.print(i); Serial.print(" V: "); Serial.print(value); Serial.print(" X:"); Serial.println(expected);
    }
  }
}

void scalingRateControl16HzTest()
{ //Rate Control Test 16 Hz, dual pin
  scalingInput->begin(SCALING_INPUT_PIN);
  scalingOutput->begin(SCALING_OUTPUT_PIN);
  scalingSWC->writePublicData(SCALING_OUTPUT_PIN, 0);

  scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);

  scalingOutput->writeRateControl(SerialWombatAbstractScaledOutput::Period::PERIOD_64mS, 100);
  scalingOutput->writeScalingEnabled(true, SCALING_INPUT_PIN);
  scalingSWC->writePublicData(SCALING_INPUT_PIN, 1000);
  uint32_t endTime = millis() + 10000;
  while (scalingOutput->readPublicData() == 0 && millis() < endTime);

  for (uint32_t i = 1; i < 10; i += 1)
  {
    uint16_t value = scalingOutput->readPublicData();

    if (withinRange(value, i * 100, 0, 0))
    {
      pass(1);
    }
    else
    {
      fail(1);
      Serial.print("Rate Control 1. i: "); Serial.print(i); Serial.print(" V: "); Serial.print(value); Serial.print(" X:"); Serial.println(i * 100);
    }
    delay(64);
  }

  for (uint32_t i = 1; i < 10; i += 1)
  {
    uint16_t value = scalingOutput->readPublicData();

    if (withinRange(value, 1000, 0, 0))
    {
      pass(1);
    }
    else
    {
      fail(1);
      Serial.print("Rate Control 2. i: "); Serial.print(i); Serial.print(" V: "); Serial.print(value); Serial.print(" X:"); Serial.println(1000);
    }
    delay(64);
  }
  scalingSWC->writePublicData(SCALING_INPUT_PIN, 500);
  uint16_t expected = 1000;
  for (uint32_t i = 0; i < 5; i += 1)
  {
    uint16_t value = scalingOutput->readPublicData();

    if (withinRange(value, expected, 0, 0))
    {
      pass(1);
    }
    else
    {
      fail(1);
      Serial.print("Rate Control 3. i: "); Serial.print(i); Serial.print(" V: "); Serial.print(value); Serial.print(" X:"); Serial.println(expected);
    }

    expected -= 100;
    delay(64);
  }
  for (uint32_t i = 1; i < 10; i += 1)
  {
    uint16_t value = scalingOutput->readPublicData();

    if (withinRange(value, 500, 0, 0))
    {
      pass(1);
    }
    else
    {
      fail(1);
      Serial.print("Rate Control 4. i: "); Serial.print(i); Serial.print(" V: "); Serial.print(value); Serial.print(" X:"); Serial.println(500);
    }
    delay(64);
  }
}

void scaling1stOrderTest()
{ //1stOrderFiltering, different pins
  scalingInput->begin(SCALING_INPUT_PIN);
  scalingOutput->begin(SCALING_OUTPUT_PIN);
  scalingSWC->writePublicData(SCALING_OUTPUT_PIN, 0);

  scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);
  scalingOutput->write1stOrderFiltering(SerialWombatAbstractScaledOutput::Period::PERIOD_8mS, 65000);
  scalingOutput->writeScalingEnabled(true, SCALING_INPUT_PIN);
  scalingSWC->writePublicData(SCALING_INPUT_PIN, 10000);


  uint16_t value = scalingOutput->readPublicData();
  uint32_t startTime = millis();
   uint32_t timeout = millis() + 10000;
  while (value < 9700 && millis() < timeout)
  {
    //Serial.println(value);
    value = scalingOutput->readPublicData();
    delay(0);
  }
  uint32_t endTime = millis();
  uint32_t elapsed = endTime - startTime; // Should take about 3400mS


  if (withinRange(elapsed, 3400, 0, 200))
  {
    pass(1);
  }
  else
  {
    fail(1);
    Serial.print("Scaling1stOrder 1.  Critical! ");  Serial.print(" V: "); Serial.print(elapsed); Serial.print(" X:"); Serial.println(3400);
  }

}

void scalingHysteresisTest()
{ //Hysteresis
  resetAll();
  scalingInput->begin(SCALING_INPUT_PIN);
  scalingOutput->begin(SCALING_OUTPUT_PIN);
  scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);

  uint16_t lowLimit = 0x5000;
  uint16_t highLimit = 0xA000;
  uint16_t lowValue = 500;
  uint16_t highValue = 1000;
  uint16_t startValue = 750;
  uint16_t midValue = lowLimit + (highLimit - lowLimit) / 2;

  scalingInput->writePublicData(midValue);
  scalingOutput->writeScalingEnabled(true, SCALING_INPUT_PIN);
  scalingOutput->writeHysteresis(lowLimit, lowValue, highLimit, highValue, startValue);

  test("SCALE_HYS_01", scalingOutput->readPublicData(), startValue);

  scalingInput->writePublicData(highLimit);
  test("SCALE_HYS_02", scalingOutput->readPublicData(), highValue);

  scalingInput->writePublicData(lowLimit);
  test("SCALE_HYS_03", scalingOutput->readPublicData(), lowValue);

  scalingInput->writePublicData(65535);
  test("SCALE_HYS_04", scalingOutput->readPublicData(), highValue);

  scalingInput->writePublicData(midValue);
  test("SCALE_HYS_05", scalingOutput->readPublicData(), highValue);

  scalingInput->writePublicData(0);
  test("SCALE_HYS_06", scalingOutput->readPublicData(), lowValue);

  scalingInput->writePublicData(midValue);
  test("SCALE_HYS_07", scalingOutput->readPublicData(), lowValue);

}

void scalingSOLinearInterpolationTest()
{
  resetAll();
  // Linear scaling test.  Table is:
  uint16_t table[] = {
    0, 0x1000,
    10000, 0x0000,
    20000, 0x8000,
    30000, 0xC000,
    40000, 0xC000,
    0xFFFF, 0
  };
 //TODO SW18AB_6B.writeUserBuffer(0x220, (uint8_t*)table, sizeof(table));

 uint16_t bufferAddr = 0x20; // Was 220 on 6B test
  scalingSWC->writeUserBuffer(bufferAddr, (uint8_t*)table, sizeof(table));

  scalingInput->begin(SCALING_INPUT_PIN);
  scalingOutput->begin(SCALING_OUTPUT_PIN);
  scalingOutput->writeScalingEnabled(false, SCALING_INPUT_PIN);
  scalingOutput->Enable2DLookupOutputScaling(bufferAddr);
  scalingOutput->writeScalingEnabled(true, SCALING_INPUT_PIN);
  int32_t i;

  for (i = 0; i <= 65535 ; ++i)
  {
    scalingInput->writePublicData(i);
    if ( i == 0)
    {
      test("SCALE_LI_00", scalingOutput->readPublicData(), 0x1000);
    }
    else if (i <= 10000)
    {
      test("SCALE_LI_01", scalingOutput->readPublicData(), (10000 - i) * 0x1000 / 10000, 2, 3  );
    }
    else if (i <= 20000)
    {
      test("SCALE_LI_02", scalingOutput->readPublicData(), (i - 10000) * 0x8000 / 10000, 2, 3  );
    }
    else if (i <= 30000)
    {
        test("SCALE_LI_03", scalingOutput->readPublicData(), (i - 20000) * 0x4000 / (10000) + 0x8000, 2, 3  );
    }
    else if (i <= 40000)
    {
      test("SCALE_LI_04", scalingOutput->readPublicData(), 0xC000 );
    }
    else
    {
        test("SCALE_LI_05", scalingOutput->readPublicData(),0xC000- (i - 40000) * 0xC000 / (0xFFFF-40000) , 2, 3  );
    }
    yield();
  }


}