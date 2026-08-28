SerialWombatRandomBlink SWRandomBlink18AB(SW18AB_6B), SWRandomBlink8B(SW8B_68);

#define RANDOM_BLINK_FIXED_TEST_TIME_mS 40
#define RANDOM_BLINK_RANGE_TEST_MIN_mS 20
#define RANDOM_BLINK_RANGE_TEST_MAX_mS 50
#define RANDOM_BLINK_TEST_OBSERVATION_mS 700
#define RANDOM_BLINK_FIXED_TEST_TIME_uS ((uint16_t)((uint32_t)RANDOM_BLINK_FIXED_TEST_TIME_mS * 1000UL))
#define RANDOM_BLINK_RANGE_TEST_MIN_uS ((uint16_t)((uint32_t)RANDOM_BLINK_RANGE_TEST_MIN_mS * 1000UL))
#define RANDOM_BLINK_RANGE_TEST_MAX_uS ((uint16_t)((uint32_t)RANDOM_BLINK_RANGE_TEST_MAX_mS * 1000UL))

SerialWombatRandomBlink* randomBlinkObject(SerialWombatChip &sw)
{
  if (&sw == &SW18AB_6B)
  {
    return &SWRandomBlink18AB;
  }
  else if (&sw == &SW8B_68)
  {
    return &SWRandomBlink8B;
  }
  else
  {
    test("TEST ERROR:  INVALID CHIP in randomBlinkObject", 0);
    return NULL;
  }
}

SerialWombatPulseTimer* randomBlinkPulseTimerObject(SerialWombatChip &sw, uint8_t pin)
{
  if (&sw == &SW18AB_6B)
  {
    if (pin < NUM_TEST_PINS && PulseTimerArray18AB[pin] != NULL)
    {
      return PulseTimerArray18AB[pin];
    }
  }
  else if (&sw == &SW8B_68)
  {
    if (pin < NUM_TEST_PINS && PulseTimerArray08B[pin] != NULL)
    {
      return PulseTimerArray08B[pin];
    }
  }

  test("TEST ERROR:  INVALID PIN in randomBlinkPulseTimerObject", 0);
  return NULL;
}

uint16_t randomBlinkReadLowCounts(SerialWombatChip &sw, uint8_t pin)
{
  SerialWombatPulseTimer* pt = randomBlinkPulseTimerObject(sw, pin);
  if (pt != NULL)
  {
    pt->refreshHighCountsLowCounts();
    return pt->LowCounts;
  }
  return 0;
}

void randomBlinkTestValueInRange(const char* designator, uint16_t value, uint16_t lowLimit, uint16_t highLimit)
{
  if (value >= lowLimit && value <= highLimit)
  {
    pass(1);
  }
  else
  {
    fail(1);
    Serial.print(designator); Serial.print(" V: "); Serial.print(value); Serial.print(" Low:"); Serial.print(lowLimit); Serial.print(" High:"); Serial.println(highLimit);
  }
}

void randomBlinkTestBool(const char* designator, bool value)
{
  if (value)
  {
    pass(1);
  }
  else
  {
    fail(1);
    Serial.println(designator);
  }
}

void randomBlinkTest(SerialWombatChip &sw, uint8_t pin)
{
  if (!test_pinCanBeOutput(sw, pin))
  {
    test("TEST ERROR:  RandomBlink test pin cannot be output", 0);
    return;
  }

  if (&sw == &SW8B_68 && !SW8B_68.isPinModeSupported(PIN_MODE_RANDOMBLINK))
  {
    Serial.println("Random Blink Pin Mode Not Available in this build of SW8B");
    return;
  }

  randomBlinkFixedTimingTest(sw, pin);
  randomBlinkRangeTimingTest(sw, pin);
  randomBlinkPWMValueSelectionTest(sw, pin);
}

void randomBlinkFixedTimingTest(SerialWombatChip &sw, uint8_t pin)
{
  resetAll();
  SerialWombatRandomBlink* rb = randomBlinkObject(sw);
  if (rb == NULL)
  {
    return;
  }

  rb->begin(pin,
            RANDOM_BLINK_FIXED_TEST_TIME_mS,
            RANDOM_BLINK_FIXED_TEST_TIME_mS,
            RANDOM_BLINK_FIXED_TEST_TIME_mS,
            RANDOM_BLINK_FIXED_TEST_TIME_mS,
            0xFFFF,
            0xFFFF,
            0,
            0);

  initializePulseReaduS(sw, pin);
  delay(RANDOM_BLINK_TEST_OBSERVATION_mS);

  // With 40 mS on and 40 mS off, 700 mS should produce about 9 high pulses.
  // Allow a broad tolerance because the first measured pulse can be partial
  // depending on where the PulseTimer starts relative to the RandomBlink state.
  test("RANDOMBLINK_FIXED_01", pulseCounts(sw, pin), 9, 4);
  test("RANDOMBLINK_FIXED_02", pulseRead(sw, pin), RANDOM_BLINK_FIXED_TEST_TIME_uS, 5000);
  test("RANDOMBLINK_FIXED_03", randomBlinkReadLowCounts(sw, pin), RANDOM_BLINK_FIXED_TEST_TIME_uS, 5000);
}

void randomBlinkRangeTimingTest(SerialWombatChip &sw, uint8_t pin)
{
  resetAll();
  SerialWombatRandomBlink* rb = randomBlinkObject(sw);
  if (rb == NULL)
  {
    return;
  }

  rb->begin(pin,
            RANDOM_BLINK_RANGE_TEST_MAX_mS,
            RANDOM_BLINK_RANGE_TEST_MAX_mS,
            RANDOM_BLINK_RANGE_TEST_MIN_mS,
            RANDOM_BLINK_RANGE_TEST_MIN_mS,
            0xFFFF,
            0xFFFF,
            0,
            0);

  initializePulseReaduS(sw, pin);
  delay(RANDOM_BLINK_TEST_OBSERVATION_mS);

  uint16_t pulseCount = pulseCounts(sw, pin);
  randomBlinkTestValueInRange("RANDOMBLINK_RANGE_01", pulseCount, 7, 18);
  randomBlinkTestValueInRange("RANDOMBLINK_RANGE_02", pulseRead(sw, pin), RANDOM_BLINK_RANGE_TEST_MIN_uS - 5000, RANDOM_BLINK_RANGE_TEST_MAX_uS + 5000);
  randomBlinkTestValueInRange("RANDOMBLINK_RANGE_03", randomBlinkReadLowCounts(sw, pin), RANDOM_BLINK_RANGE_TEST_MIN_uS - 5000, RANDOM_BLINK_RANGE_TEST_MAX_uS + 5000);
}

void randomBlinkPWMValueSelectionTest(SerialWombatChip &sw, uint8_t pin)
{
  resetAll();
  SerialWombatRandomBlink* rb = randomBlinkObject(sw);
  if (rb == NULL)
  {
    return;
  }

  rb->begin(pin,
            RANDOM_BLINK_FIXED_TEST_TIME_mS,
            RANDOM_BLINK_FIXED_TEST_TIME_mS,
            RANDOM_BLINK_FIXED_TEST_TIME_mS,
            RANDOM_BLINK_FIXED_TEST_TIME_mS,
            0x4000,
            0x4000,
            0x1000,
            0x1000);
  initializePulseReaduS(sw, pin);
  bool sawOnPWM = false;
  bool sawOffPWM = false;
  uint32_t startTime = millis();

  while (millis() - startTime < RANDOM_BLINK_TEST_OBSERVATION_mS)
  {
    if (withinRange(dutyCycleRead(sw,pin), 0x4000, 0, 0x0400))
    {
      sawOnPWM = true;
    }
    if (withinRange(dutyCycleRead(sw,pin), 0x1000, 0, 0x0400))
    {
      sawOffPWM = true;
    }
    delay(10);
  }

  randomBlinkTestBool("RANDOMBLINK_PWM_01:  Never saw ON PWM value", sawOnPWM);
  randomBlinkTestBool("RANDOMBLINK_PWM_02:  Never saw OFF PWM value", sawOffPWM);
}
