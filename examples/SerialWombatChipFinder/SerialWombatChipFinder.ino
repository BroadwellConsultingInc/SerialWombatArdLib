#include <SerialWombat.h>

const PROGMEM  char pinModeArray[][40] = {
  "DIGITAL_IO",
  "CONTROLLED",
  "ANALOGINPUT",
  "SERVO",
  "THROUGHPUT_CONSUMER",
  "QUADRATURE_ENC",
  "HBRIDGE",
  "WATCHDOG",
  "PROTECTEDOUTPUT",
  "COUNTER",
  "DEBOUNCE",
  "TM1637",
  "WS2812",
  "SW_UART",
  "INPUT_PROCESSOR",
  "MATRIX_KEYPAD",
  "PWM",
  "UART0_TXRX",
  "PULSE_TIMER",
  "DMA_PULSE_OUTPUT",
  "ANALOG_THROUGHPUT",
  "FRAME_TIMER",
  "TOUCH",
  "UART1_TXRX",
  "RESISTANCE_INPUT",
  "PULSE_ON_CHANGE",
  "HF_SERVO",
  "ULTRASONIC_DISTANCE",
  "LIQUID_CRYSTAL",
  "HS_CLOCK",
  "HS_COUNTER",
  "VGA",
  "PS2_KEYBOARD",
  "I2C_CONTROLLER",
  "QUEUED_PULSE_OUTPUT",
  "MAX7219MATRIX",
  "FREQUENCY_OUTPUT",
  "IR_RX",
  "IR_TX",
  "RC_PPM", // 39
};

// A video tutorial on this example is available at:
SerialWombatChip sw;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(); //Initialize I2C
  Serial.begin(115200);  //Initialize Serial
  while (!Serial); // Wait for initialization



}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Serial Wombat chip Finder");
  Serial.println();

  Serial.println();
  Serial.println("Special Thanks to SW8B Kickstarter Backers Joshua Sefchek and Joshua De Yonge");
  Serial.println("=======================================================");
  Serial.println();
  for (int i2cAddress = 0x0E; i2cAddress <= 0x77; ++ i2cAddress)  // Scan through all valid addresses
  {
    Wire.beginTransmission(i2cAddress); // Look for an I2C Ack
    int error = Wire.endTransmission();


    if (error == 0)
    {
      //Got an Ack. Does it behave like a Serial Wombat Chip?
      Serial.print("I2C Device found at address 0x");
      Serial.println(i2cAddress, HEX);
      sw.begin(Wire, i2cAddress, false); //Try to initialize it as a Serial Wombat Chip
      if (sw.queryVersion())  // Does the response to a version query look reasonable?
      {
        //Yes
        Serial.println("Serial Wombat chip Found.");
        if (sw.inBoot)
        {
          Serial.println("Serial Wombat chip is in boot mode.");
        }
        Serial.print("Model: ");
        Serial.println((char*)sw.model);
        if (sw.inBoot)
        {
          Serial.print ("Boot ");
        }
        Serial.print("FW Version: ");
        Serial.println((char*)sw.fwVersion);


        Serial.print("UniqueID: ");
        for (int i = 0; i < sw.uniqueIdentifierLength; ++i)
        {
          char st[5];
          sprintf(st, "%X ", sw.uniqueIdentifier[i]);
          Serial.print(st);
        }
        Serial.println();

        Serial.print("Microcontroller DeviceId: ");
        Serial.println(sw.deviceIdentifier);

        Serial.print("Microcontroller DeviceRevision: ");
        Serial.println(sw.deviceRevision);
        Serial.print("Source voltage (mV):  ");
        Serial.println(sw.readSupplyVoltage_mV());

        if (sw.isSW18() || sw.isSW08())
        {
          Serial.println("Supported Pin Modes: ");
          for (int pinModeNumber = 1; pinModeNumber < 256; ++ pinModeNumber)
          {
            uint8_t tx[8] = {201, 1, pinModeNumber, 0x55, 0x55, 0x55, 0x55, 0x55};
            int16_t returnVal = sw.sendPacket(tx);
            returnVal *= -1;
            if (returnVal == SW_ERROR_PIN_CONFIG_WRONG_ORDER)
            {
             
              if (pinModeNumber <= 39)
              {
                Serial.print((const __FlashStringHelper *)(&pinModeArray[pinModeNumber][0]));
              }
               Serial.print(" ("); Serial.print(pinModeNumber);  Serial.println(")");

            }

          }
        }
        if (sw.isSW18())
        {
          Serial.println();
          Serial.print("Low Accuracy Temperature (deg C):  ");
          uint16_t temperature = sw.readTemperature_100thsDegC();
          Serial.print (temperature / 100);
          Serial.print(".");
          Serial.println(temperature % 100);

          Serial.print("Birthday: ");
          Serial.println(sw.readBirthday());
          char brand[33];
          sw.readBrand(brand);
          Serial.print("Brand: ");
          Serial.println(brand);


        }
      }
      else
      {
        //No
        Serial.println("Device did not respond properly to Serial Wombat version information inquiry");
      }
      Serial.println();
      Serial.println();
    }
  }
  delay(30000);  // Wait 30 seconds and do it again.
}
