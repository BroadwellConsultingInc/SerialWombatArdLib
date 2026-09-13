/*
   Verify that a Serial Wombat 8B firmware image contains all of the pin modes
   required by at least one of the optional builds in swFeatureEnable.h.

   PIN_MODE_DIGITALIO is required separately because PIN_MODE_DIGITAL_IO_ENABLE
   is enabled outside of the optional build blocks and is therefore common to
   every valid SW8B build.
*/

static bool sw8BPinModeSetSupported(SerialWombatChip& sw, const uint8_t* pinModes, uint8_t pinModeCount)
{
  for (uint8_t i = 0; i < pinModeCount; ++i)
  {
    if (!sw.isPinModeSupported(pinModes[i]))
    {
      return false;
    }
  }
  return true;
}

void imageCheckTest(SerialWombatChip& sw)
{


    if (sw.isSW18())
  {
    test("SW18 Latest Firmware Check Failed ", sw.isLatestFirmware());
  }

  if (sw.isSW08())  //TODO add SW08B, SW08C
  {
    test("SW8B Latest Firmware Check Failed ", sw.isLatestFirmware());
    // Pin modes enabled by DEFAULT_BUILD
    const uint8_t defaultBuild[] = {
      PIN_MODE_ANALOGINPUT,
      PIN_MODE_SERVO,
      PIN_MODE_QUADRATUREENCODER,
      PIN_MODE_HBRIDGE,
      PIN_MODE_PWM,
      PIN_MODE_INPUT_PROCESSOR,
      PIN_MODE_FREQUENCY_OUTPUT,
      PIN_MODE_BLINK
    };

    // Pin modes enabled by FRONT_PANEL_BUILD
    const uint8_t frontPanelBuild[] = {
      PIN_MODE_ANALOGINPUT,
      PIN_MODE_QUADRATUREENCODER,
      PIN_MODE_DEBOUNCE,
      PIN_MODE_PULSE_ON_CHANGE,
      PIN_MODE_INPUT_PROCESSOR,
      PIN_MODE_PWM,
      PIN_MODE_FREQUENCY_OUTPUT,
      PIN_MODE_RANDOMBLINK,
      PIN_MODE_BLINK
    };

    // Pin modes enabled by TM1637_BUILD
    const uint8_t tm1637Build[] = {
      PIN_MODE_ANALOGINPUT,
      PIN_MODE_TM1637,
      PIN_MODE_QUADRATUREENCODER,
      PIN_MODE_DEBOUNCE,
      PIN_MODE_PULSE_ON_CHANGE,
      PIN_MODE_BLINK
    };

    // Pin modes enabled by KEYPAD_BUILD
    const uint8_t keypadBuild[] = {
      PIN_MODE_MATRIX_KEYPAD,
      PIN_MODE_BLINK,
      PIN_MODE_DEBOUNCE
    };

    // Pin modes enabled by COMMUNICATIONS_BUILD
    const uint8_t communicationsBuild[] = {
      PIN_MODE_SW_UART,
      PIN_MODE_UART_RX_TX,
      PIN_MODE_BLINK,
      PIN_MODE_SPI,
      PIN_MODE_THROUGHPUT_CONSUMER
    };

    // Pin modes enabled by IR_BUILD
    const uint8_t irBuild[] = {
      PIN_MODE_ANALOGINPUT,
      PIN_MODE_SERVO,
      PIN_MODE_PWM,
      PIN_MODE_BLINK,
      PIN_MODE_IRRX,
      PIN_MODE_IRTX
    };

    // Pin modes enabled by ULTRASONIC_BUILD
    const uint8_t ultrasonicBuild[] = {
      PIN_MODE_SERVO,
      PIN_MODE_PULSETIMER,
      PIN_MODE_ULTRASONIC_DISTANCE,
      PIN_MODE_INPUT_PROCESSOR,
      PIN_MODE_ANALOGINPUT,
      PIN_MODE_PWM,
      PIN_MODE_BLINK
    };

    // Pin modes enabled by BRUSHED_MOTOR_BUILD
    const uint8_t brushedMotorBuild[] = {
      PIN_MODE_ANALOGINPUT,
      PIN_MODE_QUADRATUREENCODER,
      PIN_MODE_HBRIDGE,
      PIN_MODE_PWM,
      PIN_MODE_INPUT_PROCESSOR,
      PIN_MODE_PULSETIMER,
      PIN_MODE_DEBOUNCE,
      PIN_MODE_BLINK
    };

    bool digitalIOSupported = sw.isPinModeSupported(PIN_MODE_DIGITALIO);

    bool defaultBuildSupported = sw8BPinModeSetSupported(sw, defaultBuild, sizeof(defaultBuild) / sizeof(defaultBuild[0]));
    bool frontPanelBuildSupported = sw8BPinModeSetSupported(sw, frontPanelBuild, sizeof(frontPanelBuild) / sizeof(frontPanelBuild[0]));
    bool tm1637BuildSupported = sw8BPinModeSetSupported(sw, tm1637Build, sizeof(tm1637Build) / sizeof(tm1637Build[0]));
    bool keypadBuildSupported = sw8BPinModeSetSupported(sw, keypadBuild, sizeof(keypadBuild) / sizeof(keypadBuild[0]));
    bool communicationsBuildSupported = sw8BPinModeSetSupported(sw, communicationsBuild, sizeof(communicationsBuild) / sizeof(communicationsBuild[0]));
    bool irBuildSupported = sw8BPinModeSetSupported(sw, irBuild, sizeof(irBuild) / sizeof(irBuild[0]));
    bool ultrasonicBuildSupported = sw8BPinModeSetSupported(sw, ultrasonicBuild, sizeof(ultrasonicBuild) / sizeof(ultrasonicBuild[0]));
    bool brushedMotorBuildSupported = sw8BPinModeSetSupported(sw, brushedMotorBuild, sizeof(brushedMotorBuild) / sizeof(brushedMotorBuild[0]));

    bool validBuild = digitalIOSupported &&
                      (defaultBuildSupported ||
                       frontPanelBuildSupported ||
                       tm1637BuildSupported ||
                       keypadBuildSupported ||
                       communicationsBuildSupported ||
                       irBuildSupported ||
                       ultrasonicBuildSupported ||
                       brushedMotorBuildSupported);

    Serial.println("SW8B image build pin mode check:");
    Serial.print("  Digital I/O common mode: "); Serial.println(digitalIOSupported ? "Present" : "MISSING");
    Serial.print("  Default Build:          "); Serial.println(defaultBuildSupported ? "MATCH" : "No");
    Serial.print("  Front Panel Build:      "); Serial.println(frontPanelBuildSupported ? "MATCH" : "No");
    Serial.print("  TM1637 Build:           "); Serial.println(tm1637BuildSupported ? "MATCH" : "No");
    Serial.print("  Keypad Build:           "); Serial.println(keypadBuildSupported ? "MATCH" : "No");
    Serial.print("  Communications Build:   "); Serial.println(communicationsBuildSupported ? "MATCH" : "No");
    Serial.print("  IR Build:               "); Serial.println(irBuildSupported ? "MATCH" : "No");
    Serial.print("  Ultrasonic Build:       "); Serial.println(ultrasonicBuildSupported ? "MATCH" : "No");
    Serial.print("  Brushed Motor Build:    "); Serial.println(brushedMotorBuildSupported ? "MATCH" : "No");

    test("SW8B image contains a complete optional build", validBuild);
  }

  if (sw.isSW18())
  {
  typedef struct
{
  uint8_t pinMode;
  const char* name;
} SW18ABPinModeTestEntry;

static const SW18ABPinModeTestEntry sw18ABPinModesToTest[] =
{
  {0x00, "Digital I/O"},
  //{0x01, "Controlled"},
  {0x02, "Analog Input"},
  {0x03, "Servo"},
  {0x04, "Throughput Consumer"},
  {0x05, "Quadrature Encoder"},
  {0x06, "H-Bridge"},
  {0x07, "Watchdog"},
  {0x08, "Protected Output"},
  //{0x09, "Counter"},
  {0x0A, "Debounced Input"},
  {0x0B, "TM1637"},
  {0x0C, "WS2812"},
  {0x0D, "Software UART"},
  {0x0E, "Input Processor"},
  {0x0F, "Matrix Keypad"},
  {0x10, "PWM"},
  {0x11, "UART0 TX/RX"},
  {0x12, "Pulse Timer"},
  //{0x13, "DMA Pulse Output"},
  //{0x14, "Analog Throughput"},
  {0x15, "Frame Timer"},
  {0x16, "Touch"},
  {0x17, "UART1 TX/RX"},
  {0x18, "Resistance Input"},
  {0x19, "Pulse On Change"},
  {0x1A, "High Frequency Servo"},
  {0x1B, "Ultrasonic Distance"},
  {0x1C, "Liquid Crystal"},
  {0x1D, "High Speed Clock"},
  {0x1E, "High Speed Counter"},
  {0x1F, "VGA"},
  {0x20, "PS/2 Keyboard"},
  {0x21, "I2C Controller"},
  {0x22, "Queued Pulse Output"},
  //{0x23, "MAX7219 Matrix"},
  {0x24, "Frequency Output"},
  {0x25, "IR RX"},
  {0x26, "IR TX"},
  //{0x27, "RC PPM"},
  {0x28, "Blink"},
  {0x29, "SPI"},
  {0x2A, "Random Blink"},
  {0x2B, "Charlieplex"},
  //{0x2C, "Matrix Keypad Simulator"},
};

  const uint8_t count =
    sizeof(sw18ABPinModesToTest) / sizeof(sw18ABPinModesToTest[0]);

  Serial.println("SW18AB pin mode support test:");

  for (uint8_t i = 0; i < count; ++i)
  {
    bool supported = sw.isPinModeSupported((int)sw18ABPinModesToTest[i].pinMode);

    char testName[64];
    snprintf(testName,
             sizeof(testName),
             "SW18AB supports 0x%02X %s",
             sw18ABPinModesToTest[i].pinMode,
             sw18ABPinModesToTest[i].name);

    test(testName, supported);
  }

}


}