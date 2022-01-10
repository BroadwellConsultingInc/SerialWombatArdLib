#include <SerialWombat.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define NUM_TEST_PINS 20

#define SW18B
//#define USE_DISPLAY
#define UNIT_TEST_SOURCE_VOLTAGE
#define UNIT_TEST_ECHO
//#define UNIT_TEST_SERVO
#define UNIT_TEST_PWM
#define UNIT_TEST_PULSE_TIMER
#define UNIT_TEST_QUAD_ENC
//#define UNIT_TEST_SEQUENCE_TEST
SerialWombat SW6C;
SerialWombat SW6D;
SerialWombat SW6E;
SerialWombat SW6F;

SerialWombat SW6B;
#ifdef SW18B
SerialWombat* SWMatch[NUM_TEST_PINS] =
{
  &SW6C,//0
  NULL,// 1
  NULL,//2
  NULL, //3
  NULL, //4
  &SW6C,//5
  &SW6C,//6
  &SW6C,//7
  &SW6D,//8
  &SW6D,//9
  &SW6D,//10
  &SW6D,//11
  &SW6E,//12
  &SW6E,//13
  &SW6E,//14
  &SW6E,//15
  &SW6F,//16
  &SW6F,//17
  &SW6F,//18
  &SW6F//19
};

bool test_pinCanBeOutput(int pin)
{
  if (pin < NUM_TEST_PINS)
  {
    return (SWMatch[pin] != NULL);
  }
  return (false);

}

uint8_t SWMatchPin[NUM_TEST_PINS] =
{
  0,//0
  0,// 1
  0,//2
  0, //3
  0, //4
  1,//5
  2,//6
  3,//7
  0,//8
  1,//9
  2,//10
  3,//11
  0,//12
  1,//13
  2,//14
  3,//15
  0,//16
  1,//17
  2,//18
  3//19
};
#endif


SerialWombatServo servo0(SW6C);

#ifdef USE_DISPLAY
Adafruit_SSD1306 display(128, 64);
#endif

int passCount = 0;
int failCount = 0;

void servoTest();
void echoTest();

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  //Turn off pull ups
  //digitalWrite(SCL,LOW);
  //digitalWrite(SDA,LOW);
  //pinMode(SCL,OUTPUT);
  //pinMode(SDA,OUTPUT);

  
 

  Serial.begin(115200);
  resetAll();
  Serial.println("Serial Wombat 18B Unit Test");
  //Serial.print("0x6C Version: "); Serial.println(SW6C.readVersion());
  
  delay(1000);
  Serial.println("Setting Failure Line");
  delay(1000);

  pinMode(D7, OUTPUT);
  digitalWrite(D7, LOW);

#ifdef USE_DISPLAY
  Serial.println("Initializing Display");
  delay(1000);
  // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.println("SW4B Unit Test");
  display.display();
#endif
  Serial.println("Setting Wire Clock");
  delay(1000);
   Wire.setClock(100000);
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
 digitalWrite(D7, HIGH);  //Wiggle a line for triggering by Logic Analyzer
 digitalWrite(D7, LOW);
  //delay(500);
  if (lastDisplayUpdate + 250 < millis()) //Yield periodically in case in tight loop
  {
    lastDisplayUpdate = millis();
    yield();
  }

}
void loop() {
/*
  Serial.println ("Starting Digital IO Test.  This test takes less than a minute");
  dioTest();
  Serial.print(millis() / 1000); Serial.print (": Digital I/O test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
*/
#ifdef UNIT_TEST_SOURCE_VOLTAGE
  Serial.println ("Starting supply voltage test.  This test takes less than a minute");
  delay(1000);
  vSupplyTest();
  Serial.print ("vSupply test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef UNIT_TEST_ECHO
  // put your main code here, to run repeatedly:
 Serial.println ("Starting ECHO Test.  This test takes about 2 minutes");
 echoTest();
 Serial.print(millis() / 1000); Serial.print (": Echo test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
 #endif 

#ifdef UNIT_TEST_SERVO
  Serial.println ("Starting Servo Test.  This test takes about 1 hour 20 minutes.");
  servoTest();
  Serial.print ("Servo test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef UNIT_TEST_PWM
  Serial.println ("Starting PWM Test.  This test takes about 15 minutes.");
  pwmTest();
  Serial.print ("PWM test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef UNIT_TEST_QUAD_ENC
  Serial.println ("Starting Quadrature Encoder Test.  This test takes about 15 minutes.");
  QuadEncTest();
  Serial.print ("Quadrature Encoder test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
/*
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
#ifdef UNIT_TEST_PULSE_TIMER
  Serial.println ("Starting PulseTimer Test.  This test takes less than a minute");
  pulseTimerTest();
  Serial.print(millis() / 1000); Serial.print (": Pulse Timer test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef UNIT_TEST_SEQUENCE_TEST
  Serial.println ("Starting Sequence Test.  This test takes less than a minute");
  SequenceTest();
  Serial.print(millis() / 1000); Serial.print (": Sequence test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

}

void disablePPS(SerialWombat& sw)
{
  byte b[] = {219,1,16,0x55,0x55,0x55,0x55,0x55};
  sw.sendPacket(b);
  b[1] = 2;
  sw.sendPacket(b);
  b[1] = 3;
  sw.sendPacket(b);

  sw.pinMode(1,INPUT);
  sw.pinMode(2,INPUT);
  sw.pinMode(3,INPUT);
}

void resetAll()
{
  SW6B.begin(Wire,0x69,false); 
  while (!SW6B.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6B did not respond to version query");
    SW6C.begin(Wire,0x6B,false);
  }
  
  
  SW6C.begin(Wire,0x6C,false); 
  while (!SW6C.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6C did not respond to version query");
    SW6C.begin(Wire,0x6C);
  }
  disablePPS(SW6C);
  
  SW6D.begin(Wire,0x6D); 
  while (!SW6D.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6D did not respond to version query");
    SW6D.begin(Wire,0x6D);
  }
  disablePPS(SW6D);
  
  SW6E.begin(Wire,0x6E); 
  while (!SW6E.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6E did not respond to version query");
    SW6E.begin(Wire,0x6E);
  }
  disablePPS(SW6E);
  
  SW6F.begin(Wire,0x6F); 
  while (!SW6F.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6F did not respond to version query");
    SW6F.begin(Wire,0x6F);
  }
  disablePPS(SW6F);
}

SerialWombatPulseTimer SWPT00(SW6C);
SerialWombatPulseTimer SWPT05(SW6C);
SerialWombatPulseTimer SWPT06(SW6C);
SerialWombatPulseTimer SWPT07(SW6C);
SerialWombatPulseTimer SWPT08(SW6D);
SerialWombatPulseTimer SWPT09(SW6D);
SerialWombatPulseTimer SWPT10(SW6D);
SerialWombatPulseTimer SWPT11(SW6D);
SerialWombatPulseTimer SWPT12(SW6E);
SerialWombatPulseTimer SWPT13(SW6E);
SerialWombatPulseTimer SWPT14(SW6E);
SerialWombatPulseTimer SWPT15(SW6E);
SerialWombatPulseTimer SWPT16(SW6F);
SerialWombatPulseTimer SWPT17(SW6F);
SerialWombatPulseTimer SWPT18(SW6F);
SerialWombatPulseTimer SWPT19(SW6F);

SerialWombatPulseTimer* PulseTimerArray[NUM_TEST_PINS] =
{
  &SWPT00,
  NULL,
  NULL,
  NULL,
  NULL,
  &SWPT05,
  &SWPT06,
  &SWPT07,
  &SWPT08,
  &SWPT09,
  &SWPT10,
  &SWPT11,
  &SWPT12,
  &SWPT13,
  &SWPT14,
  &SWPT15,
  &SWPT16,
  &SWPT17,
  &SWPT18,
  &SWPT19,
};

void initializePulseReaduS(int pin)
{
    if (pin < NUM_TEST_PINS)
    {
       if (PulseTimerArray[pin] != NULL)
       {
          PulseTimerArray[pin]->begin(SWMatchPin[pin]);
       }
       else
       {
          Serial.println("TEST ERROR:  NULL PIN in initializePulseReaduS");   
       }
    }
    else
    {
      Serial.println("TEST ERROR:  INVALID PIN in initializePulseReaduS");
    }
}

uint16 pulseRead(int pin)
{
    if (pin < NUM_TEST_PINS)
    {
       if (PulseTimerArray[pin] != NULL)
       {
          return PulseTimerArray[pin]->readHighCounts();
       }
       else
       {
          Serial.println("TEST ERROR:  NULL PIN in pulseRead"); 
          return (0);  
       }
    }
    else
    {
      Serial.println("TEST ERROR:  INVALID PIN in pulseRead");
      return 0;
    }
}
