#include "SerialWombatSimulatedQuadEnc.h"
#define USEQEA

#ifdef USEQEA
SerialWombatQuadEnc_18AB sw18qeA(SW18AB_6B);
SerialWombatSimulatedQuadEnc sw18qeSimA(SW4B_6D, SW4B_6D, 1, 2, true, false);
#endif
SerialWombatSimulatedQuadEnc sw18qeSimB(SW4B_6E, SW4B_6E, 1, 2, true, false);
SerialWombatQuadEnc_18AB sw18qeB(SW18AB_6B);


#ifdef USEQEA
SerialWombatQuadEnc_18AB sw8qeA(SW8B_68);
SerialWombatSimulatedQuadEnc sw8qeSimA(SW4B_6F, SW4B_6E, 1, 3, true, false);
#endif
SerialWombatSimulatedQuadEnc sw8qeSimB(SW4B_6E, SW4B_6E, 1, 2, true, false);
SerialWombatQuadEnc_18AB sw8qeB(SW8B_68);




void QuadEncTest(SerialWombatChip& sw)
{
  char testStr[40];
  resetAll();

  SerialWombatQuadEnc_18AB* qeA = NULL;
  SerialWombatQuadEnc_18AB* qeB = NULL;
  SerialWombatSimulatedQuadEnc* qeSimA = NULL;
  SerialWombatSimulatedQuadEnc* qeSimB = NULL;

  if (&sw == &SW18AB_6B)
  {
#ifdef USEQEA
    qeA = &sw18qeA;
    qeSimA = &sw18qeSimA;
#endif
    qeB = &sw18qeB;

    qeSimB = &sw18qeSimB;
  }
  else if (&sw == &SW8B_68)
  {
#ifdef USEQEA
    qeA = &sw8qeA;
    qeSimA = &sw8qeSimA;
#endif
    qeB = &sw8qeB;

    qeSimB = &sw8qeSimB;
  }

  if (&sw == &SW18AB_6B)
  { //Test different port error
    Serial.print("EXPECTED ERROR HERE - PART OF TEST: ");
    uint16_t priorErrorCount = sw.errorCount;
    qeA->begin(7, 8,10,true,QE_ONBOTH_INT);
    test("Quad Enc Port Mismatch: ",sw.errorCount, priorErrorCount + 1);
  }

  for (int pollType = 2; pollType < 7; pollType += 4)
  {

    if (&sw == &SW18AB_6B) {
#ifdef USEQEA
      qeSimA->initialize();
      qeA->begin(5, 6, 10, true, (QE_READ_MODE_t)pollType);
#endif

      qeSimB->initialize();
      qeB->begin(18, 19, 10, true, (QE_READ_MODE_t)pollType);
    }
    else if (&sw == &SW8B_68)
    {
#ifdef USEQEA
      qeSimA->initialize();
      qeA->begin(3, 2, 10, true, (QE_READ_MODE_t)pollType);
#endif

      qeSimB->initialize();
      qeB->begin(6, 7, 10, true, (QE_READ_MODE_t)pollType);

    }
    else
    {
      test("Quad Enc invalid Serial Wombat Chip", 0);
    }

    uint16_t target = 30000;

#ifdef USEQEA
    qeA->writePublicData(target);
    qeSimA->targetPosition = qeSimA->currentPosition = target;
#endif

    qeB->writePublicData(target);
    qeSimB->targetPosition = qeSimB->currentPosition = target;

    int testIteration;
    for (testIteration = 0; testIteration < 20; ++ testIteration)
    {
      //Serial.print("Test iteration: ");
      //Serial.println(testIteration);

#ifdef USEQEA
      qeSimA->targetPosition += testIteration;
      //uint16_t lastASample = qeA->readPublicData();
#endif

      qeSimB->targetPosition += testIteration;

      //uint16_t lastBSample = qeB->readPublicData();
      while (qeSimB->targetPosition != qeSimB->currentPosition
#ifdef USEQEA
             || qeSimA->targetPosition != (uint16_t)qeSimA->currentPosition
#endif
            )
      {

        //Serial.print("B+: T: ");
        //Serial.print(qeSimB->targetPosition);
        //Serial.print ("C: ");
        //Serial.println(qeSimB->currentPosition);

#ifdef USEQEA
        qeSimA->service();
#endif

        qeSimB->service();
        //delay(15);
      }
      delay(10);

#ifdef USEQEA

      sprintf(testStr, "QuadEnc_00A_I%d", testIteration);
      test(testStr, qeA->readPublicData(), qeSimA->currentPosition);

      qeSimA->targetPosition -= 2 * testIteration;
#endif

      sprintf(testStr, "QuadEnc_00B_I%d", testIteration);
      test(testStr, qeB->readPublicData(), qeSimB->currentPosition);

      qeSimB->targetPosition -= 2 * testIteration;

      while (qeSimB->targetPosition != (uint16_t)qeSimB->currentPosition
#ifdef USEQEA
             || qeSimA->targetPosition != (uint16_t)qeSimA->currentPosition
#endif
            )
      {

#ifdef USEQEA
        qeSimA->service();
#endif
        qeSimB->service();
        delay(1);
        //uint16_t newSample;
        // newSample= qeB->readPublicData();
      }
      delay(10);
#ifdef USEQEA
      sprintf(testStr, "QuadEnc_01A_I%d", testIteration);
      test(testStr, qeA->read(), qeSimA->currentPosition);

#endif
      sprintf(testStr, "QuadEnc_01B_I%d", testIteration);
      test(testStr, qeB->read(), qeSimB->currentPosition);

    }
  }
  { // Increment Increment testing
    uint16_t increment =  10;
    qeSimB->initialize();
     if (&sw == &SW18AB_6B)
     {
    qeB->begin(18, 19, 10, true);
     }
    else if (&sw == &SW8B_68)
    {
    qeB->begin(6, 7, 10, true);
    }
    qeB->writeMinMaxIncrementTargetPin(0, 65535, increment);

    uint16_t target = 30000;

    qeB->writePublicData(target);
    qeSimB->targetPosition = qeSimB->currentPosition = target;

    int testIteration;
    int32_t expectedOutput = 30000;
    for (testIteration = 0; testIteration < 20; ++ testIteration)
    {

      qeSimB->targetPosition += testIteration;
      expectedOutput += testIteration * increment;
      while (qeSimB->targetPosition != qeSimB->currentPosition          )
      {
        qeSimB->service();
        delay(1);
      }
      delay(10);

      test("QUADENC_INC_A", qeB->readPublicData(), expectedOutput);

      qeSimB->targetPosition -= 2 * testIteration;
      expectedOutput -= 2 * testIteration * increment;

      while (qeSimB->targetPosition != (uint16_t)qeSimB->currentPosition          )
      {
        qeSimB->service();
        delay(1);
      }
      delay(10);
      test("QUADENC_INC_B", qeB->readPublicData(), expectedOutput);

    }
  }

  { // Minimum testing
    uint16_t increment =  10;
    qeSimB->initialize();
    if (&sw == &SW18AB_6B)
     {
    qeB->begin(18, 19, 10, true);
     }
    else if (&sw == &SW8B_68)
    {
    qeB->begin(6, 7, 10, true);
    }
    qeB->writeMinMaxIncrementTargetPin(29999, 65535, increment);

    uint16_t target = 30000;

    qeB->writePublicData(target);
    qeSimB->targetPosition = qeSimB->currentPosition = target;





    qeSimB->targetPosition = 29500;

    while (qeSimB->targetPosition != (uint16_t)qeSimB->currentPosition          )
    {
      qeSimB->service();
      delay(1);
    }
    delay(10);
    test("QUADENC_MIN_A", qeB->readPublicData(), 29999);

  }

  { // Maximum testing
    uint16_t increment =  10;
    qeSimB->initialize();
     if (&sw == &SW18AB_6B)
     {
    qeB->begin(18, 19, 10, true);
     }
    else if (&sw == &SW8B_68)
    {
    qeB->begin(6, 7, 10, true);
    }
    qeB->writeMinMaxIncrementTargetPin(0, 30500, increment);

    uint16_t target = 30000;

    qeB->writePublicData(target);
    qeSimB->targetPosition = qeSimB->currentPosition = target;





    qeSimB->targetPosition = 36000;

    while (qeSimB->targetPosition != (uint16_t)qeSimB->currentPosition          )
    {
      qeSimB->service();
      delay(1);
    }
    delay(10);
    test("QUADENC_MAX_A", qeB->readPublicData(), 30500);

  }

  { // Target Pin testing

    SerialWombatPin servo(sw,0);
    servo.pinMode(OUTPUT);
    servo.writePublicData(30000);
    uint16_t increment =  10;
    qeSimB->initialize();
     if (&sw == &SW18AB_6B)
     {
    qeB->begin(18, 19, 10, true);
     }
    else if (&sw == &SW8B_68)
    {
    qeB->begin(6, 7, 10, true);
    }
    qeB->writeMinMaxIncrementTargetPin(0, 65535, increment, 0);

    uint16_t target = 30000;

    qeB->writePublicData(target);
    qeSimB->targetPosition = qeSimB->currentPosition = target;

    int testIteration;
    int32_t expectedOutput = 30000;
    for (testIteration = 0; testIteration < 20; ++ testIteration)
    {

      qeSimB->targetPosition += testIteration;
      expectedOutput += testIteration * increment;
      while (qeSimB->targetPosition != qeSimB->currentPosition          )
      {
        qeSimB->service();
        delay(1);
      }
      delay(10);

      test("QUADENC_TARGET_PIN_A", servo.readPublicData(), expectedOutput);

      qeSimB->targetPosition -= 2 * testIteration;
      expectedOutput -= 2 * testIteration * increment;

      while (qeSimB->targetPosition != (uint16_t)qeSimB->currentPosition          )
      {
        qeSimB->service();
        delay(1);
      }
      delay(10);
      test("QUADENC_TARGET_PIN_B", servo.readPublicData(), expectedOutput);

    }


  }


  { // Frequency testing

    resetAll();
     if (&sw == &SW18AB_6B)
     {
    qeB->begin(18, 19, 1, true);  // 1mS debounce
     }
    else if (&sw == &SW8B_68)
    {
    qeB->begin(6, 7, 1, true); // 1mS debounce
    }

    uint8_t sw4bPin = 0;
    // If no SW4B available, continue
    // Initialize the pin mode to pulse timer, uS
    // Set up the PWM to 32Hz (31250 uS) duty cycle to 0x1000 + 0x1000 * pin

    SerialWombatChip& sw4b = SWChipAndPinTo4BChip(sw, qeB->pin());
   
   
      sw4bPin = SWChipAndPinTo4BPin(sw,qeB->pin());

   
    if (sw4bPin == 0)
    {
      test("QuadEnc Test SW4B 0 pin isn't output", 0); // Pin 0 can't output
    }

    SerialWombatPWM_4AB pwm4B(sw4b);
    pwm4B.begin(sw4bPin);
    pwm4B.setFrequency_SW4AB(SW4AB_PWMFrequency_125_Hz);

    pwm4B.writePublicData(0x8000);





    delay(3000);


    test("QUADENC_FREQ_A", qeB->readFrequency(), 125, 10);
    test("QUADENC_FREQ_B", sw.readPublicData((uint8_t)(qeB->pin() + 1)), 125, 10);

  }



}
