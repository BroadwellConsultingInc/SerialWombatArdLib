
#define POC_IN_PIN2 7
#define POC_IN_PIN1 6
#define POC_OUT_PIN 0

SerialWombatPin *pocInput1, *pocInput2;
SerialWombatPulseOnChange *pocPointer;

void pulseOnChangeTest(SerialWombatChip &sw)
{
  resetAll();
  
  SerialWombatPin pocInput1_18(sw,POC_IN_PIN1);
  SerialWombatPin pocInput2_18(sw,POC_IN_PIN2);
  SerialWombatPulseOnChange pocPin18(sw);
  pocPointer = &pocPin18;
  pocInput1 = &pocInput1_18;
  pocInput2 = &pocInput2_18;
  pocPulseOnChange(sw);
  pocPulseOnIncrease(sw);
  pocPulseOnDecrease(sw);
  pocPulseOnEqualValue(sw);
  pocPulseOnLessThanValue(sw);
   pocPulseOnGreaterThanValue(sw);
     pocPulseOnNotEqualValue(sw);
     pocPulseOnPinsEqual(sw);
     pocPulseOnPinsNotEqual(sw);
  pocInput1->readPublicData();
  pocInput2->readPublicData();
  pocPointer->readPublicData();
  
}

void pocPulseOnChange(SerialWombatChip &sw)
{
  resetAll();
  //pocInput1->begin(POC_IN_PIN1);
  pocInput1->pinMode(OUTPUT);
  pocInput1->writePublicData(0x8000);
  pocPointer->begin(POC_OUT_PIN);
  initializePulseReaduS(sw,POC_OUT_PIN);
  delay(1000);
  test("POC_POC_01", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
  pocPointer->setEntryOnChange(0,POC_IN_PIN1);
   pocInput1->writePublicData(0x8001);
   delay(1000);
     test("POC_POC_02", pulseCounts(sw,POC_OUT_PIN), 1);  // 1 change
      test("POC_POC_02", pulseRead(sw,POC_OUT_PIN), 50000,10000);  // Should be a 50 mS pulse
}

void pocPulseOnIncrease(SerialWombatChip &sw)
{

  pocPointer->begin(POC_OUT_PIN);
   // pocInput1->begin(POC_IN_PIN1);
  pocInput1->writePublicData(0x8000);
  pocInput1->pinMode(OUTPUT);
  initializePulseReaduS(sw,POC_OUT_PIN);
  delay(1000);
  test("POC_INC_01", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
  pocPointer->setEntryOnIncrease(1,POC_IN_PIN1);
   pocInput1->writePublicData(0x7FFF);
   delay(1000);
     test("POC_INC_02", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
        pocInput1->writePublicData(0x8000);
           delay(1000);
     test("POC_INC_03", pulseCounts(sw,POC_OUT_PIN), 1);  // 1 change
      test("POC_INC_04", pulseRead(sw,POC_OUT_PIN), 50000,10000);  // Should be a 50 mS pulse
       pocInput1->writePublicData(0xC000);
           delay(1000);
     test("POC_INC_05", pulseCounts(sw,POC_OUT_PIN), 2);  // 2 changes
      test("POC_INC_06", pulseRead(sw,POC_OUT_PIN), 50000,10000);  // Should be a 50 mS pulse
}
void pocPulseOnDecrease(SerialWombatChip &sw)
{

  pocPointer->begin(POC_OUT_PIN,SW_HIGH,SW_LOW,20);
    //pocInput1->begin(POC_IN_PIN1);
    pocInput1->pinMode(OUTPUT);
  pocInput1->writePublicData(0x8000);
  initializePulseReaduS(sw,POC_OUT_PIN);
  delay(1000);
  test("POC_DEC_01", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
  pocPointer->setEntryOnDecrease(1,POC_IN_PIN1);
   pocInput1->writePublicData(0xC000);
   delay(1000);
     test("POC_DEC_02", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
        pocInput1->writePublicData(0x8000);
           delay(1000);
     test("POC_DEC_03", pulseCounts(sw,POC_OUT_PIN), 1);  // 1 change
      test("POC_DEC_04", pulseRead(sw,POC_OUT_PIN), 20000,5000);  // Should be a 20 mS pulse
       pocInput1->writePublicData(0x0000);
           delay(1000);
     test("POC_DEC_05", pulseCounts(sw,POC_OUT_PIN), 2);  // 2 changes
      test("POC_DEC_06", pulseRead(sw,POC_OUT_PIN), 20000,5000);  // Should be a 50 mS pulse
}

void pocPulseOnEqualValue(SerialWombatChip &sw)
{

  pocPointer->begin(POC_OUT_PIN);
    //pocInput1->begin(POC_IN_PIN1);
    pocInput1->pinMode(OUTPUT);
  pocInput1->writePublicData(0x8000);
  initializePulseReaduS(sw,POC_OUT_PIN);
  delay(1000);
  test("POC_EQV_01", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
  pocPointer->setEntryOnEqualValue(1,POC_IN_PIN1,0x1234);
   pocInput1->writePublicData(0xC000);
   delay(1000);
     test("POC_EQV_02", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
        pocInput1->writePublicData(0x1234);
           delay(1000);
     test("POC_EQV_03", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
      test("POC_EQV_04", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
       pocInput1->writePublicData(0x0000);
           delay(1000);
     test("POC_EQV_05", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
             pocInput1->writePublicData(0x1234);
           delay(1000);
     test("POC_EQV_06", pulseCounts(sw,POC_OUT_PIN), 20,6);  // 2 changes
      test("POC_EQV_07", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
     
}
void pocPulseOnLessThanValue(SerialWombatChip &sw)
{

  pocPointer->begin(POC_OUT_PIN);
    //pocInput1->begin(POC_IN_PIN1);
    pocInput1->pinMode(OUTPUT);
  pocInput1->writePublicData(0x8000);
  initializePulseReaduS(sw,POC_OUT_PIN);
  delay(1000);
  test("POC_LTV_01", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
  pocPointer->setEntryOnLessThanValue(1,POC_IN_PIN1,0x1234);
   pocInput1->writePublicData(0xC000);
   delay(1000);
     test("POC_LTV_02", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
        pocInput1->writePublicData(0x1233);
           delay(1000);
     test("POC_LTV_03", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
      test("POC_LTV_04", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
       pocInput1->writePublicData(0x6000);
           delay(1000);
     test("POC_LTV_05", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
             pocInput1->writePublicData(0x1234);
           delay(1000);
                 pocInput1->writePublicData(0x6000);
           delay(1000);
     test("POC_LTV_06", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
             pocInput1->writePublicData(0x1000);
           delay(1000);
     test("POC_LTV_07", pulseCounts(sw,POC_OUT_PIN), 20,6);  // 2 changes
      test("POC_LTV_08", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
     
}

void pocPulseOnGreaterThanValue(SerialWombatChip &sw)
{

  pocPointer->begin(POC_OUT_PIN);
    //pocInput1->begin(POC_IN_PIN1);
    pocInput1->pinMode(OUTPUT);
  pocInput1->writePublicData(0x0000);
  initializePulseReaduS(sw,POC_OUT_PIN);
  delay(1000);
  test("POC_GTV_01", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
  pocPointer->setEntryOnGreaterThanValue(1,POC_IN_PIN1,0x1234);
   pocInput1->writePublicData(0x0000);
   delay(1000);
     test("POC_GTV_02", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
        pocInput1->writePublicData(0x1235);
           delay(1000);
     test("POC_GTV_03", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
      test("POC_GTV_04", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
       pocInput1->writePublicData(0x0500);
           delay(1000);
     test("POC_GTV_05", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
                 pocInput1->writePublicData(0x0500);
           delay(1000);
     test("POC_GTV_06", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
             pocInput1->writePublicData(0x6000);
           delay(1000);
     test("POC_GTV_07", pulseCounts(sw,POC_OUT_PIN), 20,6);  // 2 changes
      test("POC_GTV_08", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
     
}


void pocPulseOnNotEqualValue(SerialWombatChip &sw)
{

  pocPointer->begin(POC_OUT_PIN);
  pocInput1->pinMode(OUTPUT);
    //pocInput1->begin(POC_IN_PIN1);
  pocInput1->writePublicData(0x1234);
  initializePulseReaduS(sw,POC_OUT_PIN);
  delay(1000);
  test("POC_NEV_01", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
  pocPointer->setEntryOnNotEqualValue(1,POC_IN_PIN1,0x1234);
  
        pocInput1->writePublicData(0x1233);
           delay(1000);
     test("POC_NEV_03", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
      test("POC_NEV_04", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
       pocInput1->writePublicData(0x1234);
           delay(1000);
     test("POC_NEV_05", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
             pocInput1->writePublicData(0x1235);
           delay(1000);
     test("POC_NEV_06", pulseCounts(sw,POC_OUT_PIN), 20,6);  // 2 changes
      test("POC_NEV_07", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
     
}

void pocPulseOnPinsEqual(SerialWombatChip &sw)
{

  pocPointer->begin(POC_OUT_PIN);
   //pocInput1->begin(POC_IN_PIN1);
   pocInput1->pinMode(OUTPUT);
    //pocInput2->begin(POC_IN_PIN2);
    pocInput2->pinMode(OUTPUT);

  pocInput1->writePublicData(0x1234);
  pocInput2->writePublicData(0x1235);
  initializePulseReaduS(sw,POC_OUT_PIN);
  delay(1000);
  test("POC_EQP_01", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
  pocPointer->setEntryOnPinsEqual(1,POC_IN_PIN1,POC_IN_PIN2);
  
        pocInput1->writePublicData(0x1235);
           delay(1000);
     test("POC_EQP_03", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
      test("POC_EQP_04", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
       pocInput1->writePublicData(0x1234);
           delay(1000);
     test("POC_EQP_05", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
             pocInput2->writePublicData(0x1234);
           delay(1000);
     test("POC_EQP_06", pulseCounts(sw,POC_OUT_PIN), 20,6);  // 2 changes
      test("POC_EQP_07", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
     
}

void pocPulseOnPinsNotEqual(SerialWombatChip &sw)
{

  pocPointer->begin(POC_OUT_PIN);
  pocInput1->pinMode(OUTPUT);
  pocInput2->pinMode(OUTPUT);
    //pocInput1->begin(POC_IN_PIN1);
    //pocInput2->begin(POC_IN_PIN2);

  pocInput1->writePublicData(0x1234);
  pocInput2->writePublicData(0x1234);
  initializePulseReaduS(sw,POC_OUT_PIN);
  delay(1000);
  test("POC_EQP_01", pulseCounts(sw,POC_OUT_PIN), 0);  // No Pulses So far
  pocPointer->setEntryOnPinsNotEqual(1,POC_IN_PIN1,POC_IN_PIN2);
  
        pocInput1->writePublicData(0x1235);
           delay(1000);
     test("POC_EQP_03", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
      test("POC_EQP_04", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
       pocInput1->writePublicData(0x1234);
           delay(1000);
     test("POC_EQP_05", pulseCounts(sw,POC_OUT_PIN), 10,3);  // 1 change
             pocInput2->writePublicData(0x1200);
           delay(1000);
     test("POC_EQP_06", pulseCounts(sw,POC_OUT_PIN), 20,6);  // 2 changes
      test("POC_EQP_07", pulseRead(sw,POC_OUT_PIN), 50000,5000);  // Should be a 50 mS pulse
     
}
