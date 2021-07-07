#include <SerialWombat.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define NUM_TEST_PINS 4
#define USE_DISPLAY
#define PRINT_FAILURES
SerialWombat SW6C, SW6D, SW6E;

SerialWombat* wombatsByPin[] = {&SW6C, &SW6C, &SW6C, &SW6C};
SerialWombat* wombatMatches[] = {&SW6E, &SW6D, &SW6D, &SW6D};
uint8_t pinMatches[] = {2, 3, 2, 1};
#define MATCH wombatMatches[pin]
#define MATCH_HIGH() {MATCH->digitalWrite(pinMatches[pin],HIGH);}
#define MATCH_LOW() {MATCH->digitalWrite(pinMatches[pin],LOW);}
#define MATCH_OUTPUT() {MATCH->pinMode(pinMatches[pin],OUTPUT);};

 
SerialWombatDebouncedInput  debounce1(SW6C), debounce2(SW6C), debounce3(SW6C), debounce0(SW6C);
SerialWombatDebouncedInput debounces[] = {debounce0, debounce1, debounce2, debounce3};

SerialWombatProtectedOutput swpo0(SW6E), swpo1(SW6D), swpo2(SW6D), swpo3(SW6D);
SerialWombatProtectedOutput protecteds[] = {swpo0, swpo1, swpo2, swpo3};

SerialWombatPulseTimer pulse1(SW6C), pulse2(SW6C), pulse3(SW6C), pulse0(SW6C);
SerialWombatPulseTimer pulses[] = {pulse0, pulse1, pulse2, pulse3};

SerialWombatPWM pwm1(SW6D), pwm2(SW6D), pwm3(SW6D), pwm0(SW6E);
SerialWombatPWM pwms[] = {pwm0, pwm1, pwm2, pwm3};

SerialWombatServo servo0(SW6E), servo1(SW6D), servo2(SW6D), servo3(SW6D);
SerialWombatServo servos[] = {servo0, servo1, servo2, servo3};

SerialWombatWatchdog watchdog0(SW6E), watchdog1(SW6D), watchdog2(SW6D), watchdog3(SW6D);
SerialWombatWatchdog watchdogs[] = {watchdog0, watchdog1, watchdog2, watchdog3};

SerialWombatUART uart1(SW6C), uart2(SW6D), uart3(SW6E);

Adafruit_SSD1306 display(128, 64);

int passCount = 0;
int failCount = 0;

void servoTest();
void echoTest();

void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  //Turn off pull ups
  digitalWrite(SCL,LOW);
  digitalWrite(SDA,LOW);

  
  

  Serial.begin(115200);
  resetAll();
  Serial.println("Serial Wombat 4B Unit Test");
  Serial.print("0x6C Version: "); Serial.println(SW6C.readVersion());
  Serial.print("0x6C UniqueIDLength: ");
  Serial.println(SW6C.uniqueIdentifierLength);
  Serial.print("0x6C UniqueID: "); 
  for (int i = 0; i < SW6C.uniqueIdentifierLength; ++i)
  Serial.printf("%X ",SW6C.uniqueIdentifier[i]);
  Serial.println();
  Serial.print("0x6D Version: "); Serial.println(SW6D.readVersion());
  Serial.print("0x6E Version: "); Serial.println(SW6E.readVersion());
  
 // pinMode(D3, OUTPUT);
  //digitalWrite(D3, LOW);

  // initialize with the I2C addr 0x3C
 #ifdef USE_DISPLAY
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.println("SW4B Unit Test");
  display.display();
#endif
  Wire.setClock(100000UL);
}

void updateDisplay()
{
#ifdef USE_DISPLAY
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Pass: ");
  display.println(passCount);
  display.print("Fail: ");
  display.println(failCount);
    display.setTextSize(1);
  display.print("Run time (s): ");
  display.println(millis() / 1000);
  display.display();
#endif
}

uint32_t lastDisplayUpdate = 0;
void pass(int i)
{
  ++passCount;
  if (lastDisplayUpdate + 2000 < millis())
  {
    yield();
    lastDisplayUpdate = millis();
  }
}


void fail(int i)
{
  // Serial.print("Fail at iteration "); Serial.println(i);
  ++failCount;
 // digitalWrite(D3, HIGH);  //Wiggle a line for triggering by Logic Analyzer
 // digitalWrite(D3, LOW);
  
  if (lastDisplayUpdate + 250 < millis()) //Yield periodically in case in tight loop
  {
    lastDisplayUpdate = millis();
    yield();
  }

}
void loop() {
  Serial.print("0x6C UniqueIDLength: ");
  Serial.println(SW6C.uniqueIdentifierLength);
  Serial.print("0x6C UniqueID: "); 
  for (int i = 0; i < SW6C.uniqueIdentifierLength; ++i)
  Serial.printf("%X ",SW6C.uniqueIdentifier[i]);
  Serial.println();
  Serial.print("0x6C DeviceId: ");
  Serial.println(SW6C.deviceIdentifier);
   Serial.print("0x6C DeviceRevision: ");
  Serial.println(SW6C.deviceRevision);
  
 Serial.print("0x6D UniqueIDLength: ");
  Serial.println(SW6D.uniqueIdentifierLength);
  Serial.print("0x6D UniqueID: "); 
  for (int i = 0; i < SW6D.uniqueIdentifierLength; ++i)
  Serial.printf("%X ",SW6D.uniqueIdentifier[i]);
  Serial.println();
    Serial.print("0x6E UniqueIDLength: ");
  Serial.println(SW6E.uniqueIdentifierLength);
  Serial.print("0x6E UniqueID: "); 
  for (int i = 0; i < SW6E.uniqueIdentifierLength; ++i)
  Serial.printf("%X ",SW6E.uniqueIdentifier[i]);
  Serial.println();
  
  Serial.println ("Starting Digital IO Test.  This test takes less than a minute");
  dioTest();
  Serial.print(millis() / 1000); Serial.print (": Digital I/O test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);

  Serial.println ("Starting supply voltage test.  This test takes less than a minute");
  vSupplyTest();
  Serial.print ("vSupply test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
/*
  // put your main code here, to run repeatedly:
  Serial.println ("Starting ECHO Test.  This test takes about 2 minutes");
  echoTest();
  Serial.print(millis() / 1000); Serial.print (": Echo test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);


  Serial.println ("Starting Servo Test.  This test takes about 15 minutes.");
  servoTest();
  Serial.print ("Servo test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);

  Serial.println ("Starting PWM Test.  This test takes about 15 minutes.");
  pwmTest();
  Serial.print ("PWM test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);

  Serial.println ("Starting UART Test.  This test takes about 1 minute.");
  uartTest();
  Serial.print ("UART test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);

  Serial.println ("Starting Debounce Test.  This test takes less than a minute");
  debounceTest();
  Serial.print(millis() / 1000); Serial.print (": Debounce test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);

  Serial.println ("Starting Watchdog Test.  This test takes less than a minute");
  watchdogTest();
  Serial.print(millis() / 1000); Serial.print (": Watchdog test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);

  Serial.println ("Starting Protected Output Test.  This test takes less than a minute");
  protectedOutputTest();
  Serial.print(millis() / 1000); Serial.print (": Protected Output test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
*/
}

void resetAll()
{
  SW6C.begin(0x6C);
  SW6D.begin(0x6D);
  SW6E.begin(0x6E);
  while (!SW6C.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6C did not respond to version query");
    SW6C.begin(0x6C);
  }
  while (!SW6D.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6D did not respond to version query");
    SW6C.begin(0x6D);
  }
  while (!SW6E.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6E did not respond to version query");
    SW6C.begin(0x6E);
  }


}
