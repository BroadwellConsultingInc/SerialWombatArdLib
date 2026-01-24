#include <SerialWombat.h>

/* This file is used to update the Serial Wombat 8B chip to a desired firmware.  Firmware
   images are stored in C arrays


   This sketch will connect to a single Serial Wombat 8B chip at the address
   specified by the I2C_ADDRESS below.    Use the SerialWombatChipFinder example to scan the
   I2C bus if you're unsure of the address of your chip.

   Comment in one of the XXX_FIRMWARE #define's to determine which firmware to load

   Open a serial terminal at 115200 to monitor progress.

   The user is required to send a Capital Y and Enter to confirm desire to update firmware.
   In arduino this can be done at the top of the terminal window.


   --- THIS SKETCH HAS ONLY BEEN TESTED ON THE Arduino UNO, ESP8266, ESP32 and SEEDUINO XIAO AT THIS TIME ---

*/

//#define I2C_ADDRESS 0x60 // Comment me in and set your I2C address
SerialWombatChip sw;

uint32_t appStartAddress = 0x00000000;
const

uint16_t
#ifdef ESP8266
PROGMEM
#endif
#ifdef ARDUINO_AVR_UNO
PROGMEM
#endif

// comment in one of the firmware defines below or else appimage will be undefined at compile
//#define DEFAULT_FIRMWARE
//#define BRUSHED_MOTOR_FIRMWARE
//#define COMMUNICATIONS_FIRMWARE
//#define FRONT_PANEL_FIRMWARE
//#define KEYPAD_FIRMWARE
//#define TM1637_FIRMWARE
//#define ULTRASONIC_FIRMWARE

#include "default_fw.c"
#include "brushedMotor_fw.c"
#include "frontpanel_fw.c"
#include "motorcontrol_fw.c"
#include "communications_fw.c"
#include "tm1637_fw.c"
#include "ultrasonic_fw.c"
#include "keypad_fw.c"

 bool mismatch = false;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  while (!Serial); // Wait for initialization


  delay(500);
  Serial.println(F("Serial Wombat SW8B I2C Firmware Update"));

  delay(10000);
#if (defined(ESP8266)  || defined(ARDUINO_ARCH_SAMD)  || defined(ESP32)  |defined (ARDUINO_AVR_UNO ) )
#else
  {
    Serial.println(F("This Sketch is currently only tested with ESP8266, ESP32, SAMD21 and Arduino Uno based boards.  Attempting to run it on other processors may result in firmware update failure."));
    while (1)
    {
      yield();
    }
  }
#endif

  WombatFinder();


  sw.begin(Wire, I2C_ADDRESS, false);


  delay(1000);
  sw.queryVersion();
  while (Serial.available())
  {
    char t = Serial.read();
  }
  Serial.print("Serial Wombat Firmware version ");
  Serial.print(sw.readVersion_uint32());
  Serial.println("detected.");
  if (sw.isLatestFirmware())
  {
    Serial.println(F("Firmware is already the latest version.  Update?  Send 'Y' to update"));
    int i = -1;
    while (i == -1)
    {
      delay(50);
      i = Serial.read();
    }

    if (i != 'Y')
    {
      Serial.println(F("Non 'Y' character received.  Going into infinite loop.  Reset to try again"));
      while (1) {
        delay(50);
      }

    }
  }
  if (!sw.inBoot)
  {
    Serial.println(F("Jumping to boot"));
    sw.jumpToBoot();
    Serial.println("Resetting");
    sw.hardwareReset();
    delay(2000);
  }
  else
  {
    Serial.println("Skipping reset...");
  }

  WombatFinder();

  sw.begin(Wire, I2C_ADDRESS, false);
  uint32_t address;

  Serial.print("Connecting to Serial Wombat chip");
  bool found = false;
  while (!found)
  {
    found = sw.queryVersion();
    Serial.print('.');
    delay(200);
  }
  Serial.println();

  Serial.println("Erasing");
  sw.eraseFlashPage(0);

  for (address = 0; address < (0x1000); address += 16)
  {
    uint32_t page[16];
    //   Serial.printf("Loading words starting with 0x%X  :  ", address * 4);
    for (uint32_t i = 0; i < 16; ++i)
    {
#if defined(ESP8266)
      page[i] = pgm_read_byte(((uint8_t*)appImage) + 4 * (i + address));
      page[i] += pgm_read_byte(((uint8_t*)appImage) + 4 * (i + address) + 1) << 8;
      page[i] += ((uint32_t) pgm_read_byte(((uint8_t*)appImage) + 4 * (i + address) + 2)) << 16;
      page[i] += ((uint32_t) pgm_read_byte(((uint8_t*)appImage) + 4 * (i + address) + 3)) << 24;
#elif defined(ARDUINO_AVR_UNO)
      page[i] = pgm_read_word(&appImage[(address + i) * 2]);  // 16 bit words on UNO
      page[i] += ((uint32_t)pgm_read_word(&appImage[(address + i) * 2 + 1])) << 16;

#else
      page[i] = appImage[(address + i) * 2];
      page[i] += ((uint32_t)(appImage[(address + i) * 2 + 1])) << 16;

#endif

    }
    bool dirty = false;
    for (int i = 0; i < 16;  i ++)
    {
      dirty |= (page[i] != 0xFFFFFFFF);
    }
    if (dirty)
    {
      sw.writeUserBuffer(0, (uint8_t*)page, 64);
      sw.writeFlashRow(address * 4 + 0x08000000);
      char s[50];
      sprintf(s, "Programming address: 0x%X", address);
      Serial.println(s);
      delay(10);
    }
    else
    {
      char s[50];
      sprintf(s, "Skipping blank Row  0x%X", address);
      Serial.println(s);

    }

  }

  //Verify step

  Serial.println("Beginning 100% verification...");
  int successes = 0;
  int tries = 0;
  for (address = 0; address < (0x1000); address += 16)
  {
    uint32_t page[16];
    
    for (uint32_t i = 0; i < 16; ++i)
    {
      ++tries;
#if defined(ESP8266)
      page[i] = pgm_read_byte(((uint8_t*)appImage) + 4 * (i + address));
      page[i] += pgm_read_byte(((uint8_t*)appImage) + 4 * (i + address) + 1) << 8;
      page[i] += ((uint32_t) pgm_read_byte(((uint8_t*)appImage) + 4 * (i + address) + 2)) << 16;
      page[i] += ((uint32_t) pgm_read_byte(((uint8_t*)appImage) + 4 * (i + address) + 3)) << 24;
#elif defined(ARDUINO_AVR_UNO)
      page[i] = pgm_read_word(&appImage[(address + i) * 2]);  // 16 bit words on UNO
      page[i] += ((uint32_t)pgm_read_word(&appImage[(address + i) * 2 + 1])) << 16;

#else
      page[i] = appImage[(address + i) * 2];
      page[i] += ((uint32_t)(appImage[(address + i) * 2 + 1])) << 16;


#endif
      uint32_t readback = sw.readFlashAddress((address + i) * 4 + 0x08000000);
      if (readback != page[i])
      {
        mismatch = true;
        char s[50];
      sprintf(s, "Verify fail at address: 0x%X", address + i);
      Serial.println(s);
      sprintf(s, "Expected: 0x%X  Got: 0x%X", page[i],readback);
      }
      else
      {
        ++successes;
      }

    }

  }
  Serial.print (successes); Serial.print(" of ");Serial.print(tries);Serial.println(" words verified");
  if (!mismatch)
  {
    uint8_t tx[] = { 164, 4, 0, 0, 0, 0, 0, 0 }; // Set boot flag

    sw.sendPacket(tx);
    delay(100);
    sw.hardwareReset();

  }




}

void loop() {

  // put your main code here, to run repeatedly:
  Serial.println("Serial Wombat chips found:");
  Serial.println();

  Serial.println();
  Serial.println("=======================================================");
  Serial.println();
  WombatFinder();

  Serial.println();
  if (mismatch)
  {
  Serial.println("Update failed.");
    
  }
  else
  {
  Serial.println("Update complete.  Load a new sketch such as the SerialWombatChipFinder sketch, then power cycle the Arduino and Serial Wombat chip.");
  }
  Serial.println();
  Serial.println("Resetting or power cycling without loading a new sketch will cause the flash download to happen again.");
  Serial.println();

  delay(5000);
}
