/*
Pin Matching:
SW18AB  SW4B  SW8B  DAC
0       D3    -     SWA
1       -     -
2       -     -
3       -     -
4       -     -
5       D2    -
6       D1    8P0   SWD
7       C0    8P5   SWB
8       D0    -
9       C1    8P4   SWC
10      C2    -
11      C3    -
12      E0    -
13      F0    -
14      F3    -
15      F2    -
16      F1    8P2   HWA
17      E3    8P3   HWB
18      E2    8P6   HWC
19      E1    8P7   HWD
*/
#include "SerialWombat.h"
#include <SoftWire.h>
#include <stdint.h>
#define NUM_TEST_PINS 20
#define FAILUREPIN 8
//#define LOAD_REQUIRED_FIRMWARE

#define TEST_SW18AB
#define TEST_SW8B
#define TEST_SW4B

//#define UNIT_TEST_QUEUE
#define UNIT_TEST_USDSENSOR
#define UNIT_TEST_HSCLOCK
#define UNIT_TEST_HSCOUNTER
#define UNIT_TEST_RESISTANCE_INPUT
#define UNIT_TEST_SCALING
#define UNIT_TEST_BLINK
#define UNIT_TEST_SW_UART

#define UNIT_TEST_HW_UART

#define UNIT_TEST_HBRIDGE
#define UNIT_TEST_ANALOG_INPUT
#define UNIT_TEST_FREQUENCY_OUTPUT
#define UNIT_TEST_INPUT_PROCESSOR

#define UNIT_TEST_COMMUNICATION_ERROR
#define UNIT_TEST_ECHO
#define UNIT_TEST_PWM
#define UNIT_TEST_SERVO
#define UNIT_TEST_PUBLIC_DATA
#define UNIT_TEST_DEBOUNCED_INPUT
#define UNIT_TEST_QUAD_ENC
#define UNIT_TEST_PULSE_ON_CHANGE

#define UNIT_TEST_PULSE_TIMER
#define UNIT_TEST_SOURCE_VOLTAGE
#define UNIT_TEST_SEQUENCE_TEST
#define UNIT_TEST_DATALOGGER
#define UNIT_TEST_PROTECTED_OUTPUT
#define UNIT_TEST_WATCHDOG
#define UNIT_TEST_FRAME_TIMER
#define UNIT_TEST_QUEUED_PULSE_OUTPUT


SerialWombatChip SW4B_6C;
SerialWombatChip SW4B_6D;
SerialWombatChip SW4B_6E;
SerialWombatChip SW4B_6F;
SerialWombatChip SW8B_68;

SerialWombatChip SW18AB_6B;

SerialWombatChip SW_NULL;

SoftWire softWire(2,3);

void test(const char* designator, uint16_t value, uint16_t expected = 1, uint16_t counts = 0, uint16_t sixtyFourths =  0);


uint8_t SW18ABPinTo8BPin(int pin)
{
  switch (pin)
  {
    case 0:
      return 1;
    case 6:
      return 0;
    case 7:
      return 5;
    case 9: 
      return 4;
    case 16:
      return 2;
    case 17:
      return 3;
    case 18: 
      return 6;
    case 19:
      return 7;
    default:
    test("Invalid SW18AB pin to 4B pin",0);
      return 255;
  }
}

uint8_t SW18ABPinTo4BPin(int pin)
{
  switch(pin)
  {
  case 0:
    return 3;
  case 5:
    return 2;
  case 6:
    return 1;
  case 7:
    return 0;
  case 8:
    return 0;
  case 9:
    return 1;
  case 10:
    return 2;
  case 11:
    return 3;
  case 12:
    return 0;
  case 13:
    return 0;
  case 14:
    return 3;
  case 15:
    return 2;
  case 16:
    return 1;
  case 17:
    return 3;
  case 18:
    return 2;
  case 19:
    return 1;
  }
  char s[50];
  sprintf(s,"Invalid SW18B pin %d to 4B pin",pin);
    test(s,0);
  return 255;
}

uint8_t SW8BPinTo18ABPin(int pin)
{
  switch (pin)
  {
  case 0:
    return 6;
  case 1:
    return 0;
  case 2:
    return 16;
  case 3:
    return 17;
  case 4:
    return 9;
  case 5:
    return 7;
  case 6:
    return 18;
  case 7:
    return 19;
  }
  test("Invalid 8B pin to 18AB pin",0);
  
  return 255; // Should never happen
}

uint8_t SW8BPinTo4BPin(int pin)
{
 return (SW18ABPinTo4BPin(SW8BPinTo18ABPin(pin)));
}

SerialWombatChip& SWChipAndPinTo4BChip(SerialWombatChip& sw, uint8_t pin)
{
  if (&sw == &SW18AB_6B)
  {
    return (SW18ABPinTo4BChip( pin));
  }
  else if (&sw == &SW8B_68)
  {
    return (SW8BPinTo4BChip( pin));
  }
  else
  {
    return (SW_NULL);
  }
}

uint8_t SWChipAndPinTo4BPin(SerialWombatChip& sw, uint8_t pin)
{
  if (&sw == &SW18AB_6B)
  {
    return (SW18ABPinTo4BPin( pin));
  }
  else if (&sw == &SW8B_68)
  {
    return (SW8BPinTo4BPin( pin));
  }
  else
  {
    return (0);
  }
}
SerialWombatChip& SW8BPinTo4BChip(int pin)
{
  return SW18ABPinTo4BChip(SW8BPinTo18ABPin(pin));
}
SerialWombatChip& SW18ABPinTo4BChip(int pin)
{
  switch (pin)
  {
  case 0:
    return SW4B_6D;
  case 5:
    return SW4B_6D;
  case 6:
    return SW4B_6D;
  case 7:
    return SW4B_6C;
  case 8:
    return SW4B_6D;
  case 9:
    return SW4B_6C;
  case 10:
    return SW4B_6C;
  case 11:
    return SW4B_6C;
  case 12:
    return SW4B_6E;
  case 13:
    return SW4B_6F;
  case 14:
    return SW4B_6F;
  case 15:
    return SW4B_6F;
  case 16:
    return SW4B_6F;
  case 17:
    return SW4B_6E;
  case 18:
    return SW4B_6E;
  case 19:
    return SW4B_6E;
  }
  
  
  return SW_NULL; // Pin has no associated SW4B
}



bool test_pinCanBeOutput(SerialWombatChip &sw, int pin)
{
  if (&sw == &SW18AB_6B)
  {
    if (pin >=1 && pin <=4)
    {
      return false;
    }
    if (pin > 19)
    {
      return false;
    }

  }
  else if (&sw == &SW8B_68)
  {
    if (pin > 7)
    {
      return false;
    }
  }
  else if (&sw == &SW4B_6C || &sw == &SW4B_6D || &sw == &SW4B_6E || &sw == &SW4B_6F)
  {
    if (pin == 0 || pin > 3)
    {
      return false;
    }
  }
  else
  {
    test("TEST ERROR:  Invalid SW Chip in test_pinCanBeOutput",0);
    return(false);
  }

  return ( true);

}
#if 0
uint8_t SWMatchPin[NUM_TEST_PINS] =
{
  SW18B_0_MATCH_PIN ,//0
  SW18B_1_MATCH_PIN ,// 1
  SW18B_2_MATCH_PIN ,//2
  SW18B_3_MATCH_PIN , //3
  SW18B_4_MATCH_PIN , //4
  SW18B_5_MATCH_PIN ,//5
  SW18B_6_MATCH_PIN ,//6
  SW18B_7_MATCH_PIN ,//7
  SW18B_8_MATCH_PIN ,//8
  SW18B_9_MATCH_PIN ,//9
  SW18B_10_MATCH_PIN,//10
  SW18B_11_MATCH_PIN,//11
  SW18B_12_MATCH_PIN,//12
  SW18B_13_MATCH_PIN,//13
  SW18B_14_MATCH_PIN,//14
  SW18B_15_MATCH_PIN,//15
  SW18B_16_MATCH_PIN,//16
  SW18B_17_MATCH_PIN,//17
  SW18B_18_MATCH_PIN,//18
  SW18B_19_MATCH_PIN//19
};
#endif


int passCount = 0;
int failCount = 0;
char softWireTxBuffer[16];
char softWireRxBuffer[16];

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
     softWire.setTxBuffer(softWireTxBuffer, sizeof(softWireTxBuffer));
  softWire.setRxBuffer(softWireRxBuffer, sizeof(softWireTxBuffer));
  softWire.setDelay_us(5);
  softWire.setTimeout(1000);
  softWire.begin();
  //Turn off pull ups
  //digitalWrite(SCL,LOW);
  //digitalWrite(SDA,LOW);
  //pinMode(SCL,OUTPUT);
  //pinMode(SDA,OUTPUT);

  analogShutdown();


  Serial.begin(115200);
  delay(100);
  resetAll();
  Serial.println();
  Serial.println("#############################################################");
  Serial.println();
  Serial.println("Serial Wombat 18B Unit Test");
  Serial.println();
  Serial.println("#############################################################");
  Serial.println();

  delay(1000);
  Serial.println("Setting Failure Line");
 
  delay(1000);

#ifdef FAILUREPIN
  pinMode(FAILUREPIN, OUTPUT);
  digitalWrite(FAILUREPIN, LOW);
#endif

/*
  pinMode(0,OUTPUT); // Set the switch ICS to open
  digitalWrite(0,LOW);
 */

  Serial.println("Setting Wire Clock");
  delay(1000);
  Wire.setClock(100000);

  SW18AB_6B.registerErrorHandler(SerialWombatSerialErrorHandlerBrief);

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
int lastPassedTest = -1;
void pass(int i)
{
  ++passCount;
  lastPassedTest = i;
  if (lastDisplayUpdate + 2000 < millis())
  {
    yield();
    lastDisplayUpdate = millis();
  }
}

int lastFailedTest = -1;
void fail(int i)
{
  lastFailedTest = i;
  // Serial.print("Fail at iteration "); Serial.println(i);
  ++failCount;
#ifdef FAILUREPIN
  digitalWrite(FAILUREPIN, HIGH);  //Wiggle a line for triggering by Logic Analyzer
  delay(1);
  digitalWrite(FAILUREPIN, LOW);
#endif
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

#ifdef UNIT_TEST_COMMUNICATION_ERROR
#ifdef TEST_SW18AB
  Serial.println ("Starting SW18AB communication error test.  This test takes less than a minute");
  resetAll();
  CommunicationErrorTest(SW18AB_6B);
  Serial.print ("Communication error test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif

#ifdef TEST_SW8B
  Serial.println ("Starting SW8B communication error test.  This test takes less than a minute");
  resetAll();
  CommunicationErrorTest(SW8B_68);
  Serial.print ("Communication error test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif

#endif
#ifdef UNIT_TEST_SOURCE_VOLTAGE
  Serial.println ("Starting supply voltage test.  This test takes less than a minute");
  resetAll();
  vSupplyTest();
  Serial.print ("vSupply test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef UNIT_TEST_ECHO
  // put your main code here, to run repeatedly:
 
  #ifdef TEST_SW18AB
   Serial.println ("Starting SW18AB ECHO Test.  This test takes about 2 minutes");
    echoTest(SW18AB_6B);
     Serial.print ("Echo test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
    #endif
  #ifdef TEST_SW8B
   Serial.println ("Starting SW8B ECHO Test.  This test takes about 2 minutes");
    echoTest(SW8B_68); 
    Serial.print ("Echo test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
    #endif
  #ifdef TEST_SW4B  
   Serial.println ("Starting SW4B ECHO Test.  This test takes about 2 minutes");
    echoTest(SW4B_6C);
    Serial.print ("Echo test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
    #endif
 
#endif

#ifdef UNIT_TEST_SERVO
#ifdef TEST_SW18AB
  Serial.println ("Starting SW18AB Servo Test.  This test takes about 1 hour 15 minutes.");
  resetAll();
  servoTest(SW18AB_6B,0,19); 
  Serial.print ("Servo test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif
#ifdef TEST_SW8B
  Serial.println ("Starting SW8B Servo Test.  This test takes about 1 hour 15 minutes.");
  resetAll();
  servoTest(SW8B_68,0,7);
  Serial.print ("Servo test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif




#endif

#ifdef UNIT_TEST_QUEUE
#ifdef TEST_SW18AB
Serial.println ("Starting SW18AB Queue Test.  This test takes about 50 minutes.");
  resetAll();
  queueTest(SW18AB_6B); 
  Serial.print ("Queue test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef TEST_SW8B
Serial.println ("Starting SW8B Queue Test.  This test takes about 6 minutes.");
  resetAll();
  queueTest(SW8B_68); 
  Serial.print ("Queue test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#endif

#ifdef UNIT_TEST_PWM
#ifdef TEST_SW18AB
  Serial.println ("Starting SW18AB Pwm Test.  This test takes about 1 hour 15 minutes.");
  resetAll();
  pwmTest(SW18AB_6B,18,19);   //TODO try without TRM, expand pin range
  Serial.print ("PWM test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif
#ifdef TEST_SW8B
  Serial.println ("Starting SW8B PWM Test.  This test takes about 1 hour 15 minutes.");
  resetAll();
  pwmTest(SW8B_68,6,6);  //TODO expand pin range
  Serial.print ("PWM test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif
#endif

#ifdef UNIT_TEST_QUAD_ENC
#ifdef TEST_SW18AB
  Serial.println ("Starting SW18AB Quadrature Encoder Test.  This test takes about 7 minutes.");
  resetAll();
  QuadEncTest(SW18AB_6B);
  Serial.print ("Quadrature Encoder test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif

  #ifdef TEST_SW8B
  if (SW8B_68.isPinModeSupported(PIN_MODE_QUADRATUREENCODER))
  {
  Serial.println ("Starting SW8B Quadrature Encoder Test.  This test takes about 7 minutes.");
  resetAll();
  QuadEncTest(SW8B_68);
  Serial.print ("Quadrature Encoder test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  }
  else
  {
    Serial.println ("Quadrature Encoder Pin Mode Not Available in SW8B");
  }
  #endif
#endif

#ifdef UNIT_TEST_HW_UART
#ifdef TEST_SW18AB
  Serial.println ("Starting 18AB HW UART Test.  This test takes about 31 minutes.");
  resetAll();
  uartHWTest(SW18AB_6B,9,7,19,17);
  Serial.print ("UART test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif
   #ifdef TEST_SW8B
  if (SW8B_68.isPinModeSupported(PIN_MODE_UART_RX_TX))
  {
  Serial.println ("Starting SW8B HW UART Encoder Test.  This test takes about 31 minutes.");
  resetAll();
  SW18AB_6B.readPublicData(0); //TODO remove
  uartHWTest(SW8B_68,5,4,255,255);
  Serial.print ("UART test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  }
  else
  {
    Serial.println ("HW UART Pin Mode Not Available in SW8B");
  }
  #endif
#endif

#ifdef UNIT_TEST_SW_UART
  Serial.println ("Starting SW UART Test.  This test takes about 25 minutes.");
  resetAll();
  uartSWTest();
  Serial.print ("UART SW test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef UNIT_TEST_DEBOUNCED_INPUT
#ifdef TEST_SW18AB
  Serial.println ("Starting 18AB Debounced Input Test.  This test takes less than a minute");
  resetAll();
  debounceTest(SW18AB_6B);
  Serial.print (": Debounce test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif
#ifdef TEST_SW8B
  Serial.println ("Starting 8B Debounced Input Test.  This test takes less than a minute");
  resetAll();
  debounceTest(SW8B_68);
  Serial.print ("Debounce test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif
#ifdef TEST_SW4B
  Serial.println ("Starting 4B Debounced Input Test.  This test takes less than a minute");
  resetAll();
  debounceTest(SW4B_6C);
  Serial.print (": Debounce test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif
#endif
#ifdef UNIT_TEST_RESISTANCE_INPUT
#ifdef TEST_SW18AB
  Serial.println ("Starting ResistanceInput Test.  This test takes less than a minute");
  resetAll();
  resistanceInputTest();
  Serial.print (": ResistanceInput test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#endif
  /*
    Serial.println ("Starting Watchdog Test.  This test takes less than a minute");
    watchdogTest();
    Serial.print(millis() / 1000); Serial.print (": Watchdog test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);

  */
#ifdef UNIT_TEST_PULSE_TIMER
  #ifdef TEST_SW18AB
  Serial.println ("Starting 18AB PulseTimer Test.  This test takes about 3 minutes");
  resetAll();
  pulseTimerTest(SW18AB_6B,0,19);
  Serial.print ("Pulse Timer test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif
  #ifdef TEST_SW8B
  if (SW8B_68.isPinModeSupported(PIN_MODE_PULSETIMER))
  {
     Serial.println ("Starting 8B PulseTimer Test.  This test takes about 3 minutes");
  resetAll();
  pulseTimerTest(SW8B_68,0,7);
  Serial.print ("Pulse Timer test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  }
  #endif
#endif

#ifdef UNIT_TEST_PROTECTED_OUTPUT
  Serial.println ("Starting Protected Output Test.  This test takes about 1 minute");
  resetAll();
  protectedOutputTest18AB();
  Serial.print ("Protected Output test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef UNIT_TEST_PULSE_ON_CHANGE
#ifdef TEST_SW8B
  if (SW8B_68.isPinModeSupported(PIN_MODE_PULSE_ON_CHANGE))
  {  Serial.println ("Starting Pulse On Change Test SW8B.  This test takes about 1 minute");
  resetAll();
  pulseOnChangeTest(SW8B_68);
  Serial.print ("Pulse On Change test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  }
  else
  {
    Serial.println ("Pulse On Change not supported in this build of SW8B");
  }
#endif
#ifdef TEST_SW18AB
  Serial.println ("Starting Pulse On Change Test SW18AB.  This test takes about 1 minute");
  resetAll();
  pulseOnChangeTest(SW18AB_6B);
  Serial.print ("Pulse On Change test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#endif

#ifdef UNIT_TEST_WATCHDOG
  Serial.println ("Starting WATCHDOG Test.  This test takes about 1 minute");
  resetAll();
  watchdogTest18AB();
  Serial.print ("WATCHDOG test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef UNIT_TEST_SEQUENCE_TEST
  Serial.println ("Starting Sequence Test.  This test takes less than a minute");
  resetAll();
  SequenceTest();
  Serial.print ("Sequence test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#ifdef UNIT_TEST_PUBLIC_DATA
#ifdef TEST_SW4B
  Serial.println ("Starting 4B Public Data Test.  This test takes less than 2 minutes");
  resetAll();
  publicDataTest(SW4B_6C );
  Serial.print(millis() / 1000); Serial.print (": Public Data test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#ifdef TEST_SW8B
  Serial.println ("Starting 8B Public Data Test.  This test takes less than 2 minutes");
  resetAll();
  publicDataTest(SW8B_68 );
  Serial.print(millis() / 1000); Serial.print (": Public Data test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#ifdef TEST_SW18AB
  Serial.println ("Starting 18AB Public Data Test.  This test takes less than 2 minutes");
  resetAll();
  publicDataTest(SW18AB_6B );
  Serial.print(millis() / 1000); Serial.print (": Public Data test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#endif
#ifdef UNIT_TEST_ANALOG_INPUT
#ifdef TEST_SW18AB
  Serial.println ("Starting 18AB Analog Input Test.  This test takes less than 2 minutes");
  resetAll();
  analogInputTest(SW18AB_6B);
  Serial.print ("18AB Analog Input test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif
#ifdef TEST_SW4B
  Serial.println ("Starting 4B Analog Input Test.  This test takes less than 2 minutes");
  resetAll();
  analogInputTest(SW4B_6C);
  Serial.print ("4B Analog Input test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#ifdef TEST_SW8B
  Serial.println ("Starting 8B Analog Input Test.  This test takes less than 2 minutes");
  resetAll();
  analogInputTest(SW8B_68);
  Serial.print ("8B Analog Input test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
  
#endif

#ifdef UNIT_TEST_BLINK
#ifdef TEST_SW18AB
  Serial.println ("Starting 18AB Blink Test.  This test takes less than 2 minutes");
  resetAll();
  blinkTest(SW18AB_6B,6,5);
  Serial.print ("18AB Blink  test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif
#ifdef TEST_SW8B
  Serial.println ("Starting 8B Blink  Test.  This test takes less than 2 minutes");
  resetAll();
  blinkTest(SW8B_68,6,5);
  Serial.print ("8B Blink test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
  
#endif


#ifdef UNIT_TEST_SCALING
  Serial.println ("Starting Scaling Test.  This test takes less than 7 minutes");
  resetAll();
  scalingTest();
  Serial.print ("Scaling complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef UNIT_TEST_INPUT_PROCESSOR
#ifdef TEST_SW18AB
  Serial.println ("Starting 18AB Input Processor Test.  This test takes less than 5 minutes");
  resetAll();
  inputProcessorTest(SW18AB_6B, 19);
  Serial.print ("Input Processor Test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif
#ifdef TEST_SW8B
  Serial.println ("Starting 8B Input Processor Test.  This test takes less than 5 minutes");
  resetAll();
  inputProcessorTest(SW8B_68,4);
  Serial.print ("Input Processor Test complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  #endif


#endif

#ifdef UNIT_TEST_HSCLOCK
#ifdef TEST_SW18AB
  Serial.println ("Starting High Speed Clock Test 18AB.  This test takes less than 1 minute");
  resetAll();
  hsClockTest();
  Serial.print ("HSClock complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#endif

#ifdef UNIT_TEST_HSCOUNTER
#ifdef TEST_SW18AB
  Serial.println ("Starting High Speed Counter Test 18AB.  This test takes less than 2 minutes");
  resetAll();
  hsCounterTest();
  Serial.print ("HSCounter complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#endif

#ifdef UNIT_TEST_FRAME_TIMER
  Serial.println ("Starting Frame Timer Test.  This test takes less than 1 minute");
  resetAll();
  frameTimerTest();
  Serial.print ("FrameTimer complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef UNIT_TEST_USDSENSOR
#ifdef TEST_SW18AB
  Serial.println ("Starting Ultrasonic Distance Sensor 18AB Test.  This test takes less than 1 minute");
  usdSensorTest(10);
  Serial.print ("Ultrasonic Distance Sensor Test   complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#endif

#ifdef UNIT_TEST_FREQUENCY_OUTPUT
#ifdef TEST_SW18AB
  Serial.println ("Starting 18AB Frequency Output Test.  This test takes less than 1 minute");
  frequencyOutputTest(SW18AB_6B,15);  //TODO add non TRM pin
  Serial.print ("Frequency Output   complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif

#ifdef TEST_SW8B
  Serial.println ("Starting 8B Frequency Output Test.  This test takes less than 1 minute");
  frequencyOutputTest(SW8B_68,4);  
  Serial.print ("Frequency Output   complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#endif

#ifdef UNIT_TEST_HBRIDGE
#ifdef TEST_SW18AB
  Serial.println ("Starting 18AB H Bridge  Test.  This test takes less than 13 minutes");
  hBridgeTest(SW18AB_6B,5,6);
  Serial.print ("H Bridge Test   complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
#endif
#ifdef TEST_SW8B
 if (SW8B_68.isPinModeSupported(PIN_MODE_HBRIDGE))
  {
  Serial.println ("Starting 8B H Bridge  Test.  This test takes less than 13 minutes");
  hBridgeTest(SW8B_68,2,3);
  Serial.print ("H Bridge Test   complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);
  }
  else
  {
    Serial.println ("Pulse On Change not supported in this build of SW8B");
  }
  #endif

#endif

#ifdef UNIT_TEST_QUEUED_PULSE_OUTPUT
  Serial.println ("Starting Queued Pulse Output  Test.  This test takes less than XX minutes");
  queuedPulseOutputTest();
  Serial.print ("Queued Pulse Output Test   complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);

#endif

#ifdef UNIT_TEST_DATALOGGER
  Serial.println ("Starting DataLogger  Test.  This test takes less than XX minutes");
  dataLoggerTest();
  Serial.print ("Queued DataLogger Test   complete.  Pass: "); Serial.print(passCount); Serial.print(" Fail: "); Serial.println(failCount);

#endif

}

void disablePPS(SerialWombatChip& sw)
{
  byte b[] = {219, 1, 16, 0x55, 0x55, 0x55, 0x55, 0x55};
  sw.sendPacket(b);
  b[1] = 2;
  sw.sendPacket(b);
  b[1] = 3;
  sw.sendPacket(b);

  sw.pinMode(1, INPUT);
  sw.pinMode(2, INPUT);
  sw.pinMode(3, INPUT);
}

void resetAll()
{
  static bool versionChecked = false;
  analogShutdown();

  SW8B_68.registerErrorHandler(NULL);
  SW8B_68.begin(Wire, 0x68);   
  while (!SW8B_68.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x68 did not respond to version query");
    SW8B_68.begin(Wire, 0x68);
  }
  if (!versionChecked && !SW8B_68.isLatestFirmware())
  {
    Serial.println("Serial Wombat chip at 0x68 is not latest firmware");
  }



  SW18AB_6B.registerErrorHandler(NULL);
  SW18AB_6B.begin(Wire, 0x6B);
  while (!SW18AB_6B.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6B did not respond to version query");
    SW18AB_6B.begin(Wire, 0x6B, false);
  }
 if (!versionChecked && !SW18AB_6B.isLatestFirmware())
  {
    Serial.println("Serial Wombat chip at 0x6B is not latest firmware");
  }


  SW4B_6C.begin(Wire, 0x6C);
  while (!SW4B_6C.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6C did not respond to version query");
    SW4B_6C.begin(Wire, 0x6C);
  }
  disablePPS(SW4B_6C);

  SW4B_6D.begin(Wire, 0x6D);
  while (!SW4B_6D.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6D did not respond to version query");
    SW4B_6D.begin(Wire, 0x6D);
  }
  disablePPS(SW4B_6D);

  SW4B_6E.begin(Wire, 0x6E);
  while (!SW4B_6E.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6E did not respond to version query");
    SW4B_6E.begin(Wire, 0x6E);
  }
  disablePPS(SW4B_6E);

  SW4B_6F.begin(Wire, 0x6F);
  while (!SW4B_6F.queryVersion())
  {
    Serial.println("Serial Wombat chip at 0x6F did not respond to version query");
    SW4B_6F.begin(Wire, 0x6F);
  }
  disablePPS(SW4B_6F);

  #ifdef TEST_SW18AB
  SW18AB_6B.registerErrorHandler(SerialWombatSerialErrorHandlerBrief);
  #endif
  versionChecked = true;

}



SerialWombatPulseTimer SW18AB_PT00(SW4B_6D);
SerialWombatPulseTimer SW18AB_PT05(SW4B_6D);
SerialWombatPulseTimer SW18AB_PT06(SW4B_6D);
SerialWombatPulseTimer SW18AB_PT07(SW4B_6C);
SerialWombatPulseTimer SW18AB_PT08(SW4B_6D);
SerialWombatPulseTimer SW18AB_PT09(SW4B_6C);
SerialWombatPulseTimer SW18AB_PT10(SW4B_6C);
SerialWombatPulseTimer SW18AB_PT11(SW4B_6C);
SerialWombatPulseTimer SW18AB_PT12(SW4B_6E);
SerialWombatPulseTimer SW18AB_PT13(SW4B_6F);
SerialWombatPulseTimer SW18AB_PT14(SW4B_6F);
SerialWombatPulseTimer SW18AB_PT15(SW4B_6F);
SerialWombatPulseTimer SW18AB_PT16(SW4B_6F);
SerialWombatPulseTimer SW18AB_PT17(SW4B_6E);
SerialWombatPulseTimer SW18AB_PT18(SW4B_6E);
SerialWombatPulseTimer SW18AB_PT19(SW4B_6E);

SerialWombatPulseTimer SW8B_PT00 = SW18AB_PT06;
SerialWombatPulseTimer SW8B_PT01 = SW18AB_PT00;
SerialWombatPulseTimer SW8B_PT02 = SW18AB_PT16;
SerialWombatPulseTimer SW8B_PT03 = SW18AB_PT17;
SerialWombatPulseTimer SW8B_PT04 = SW18AB_PT09;
SerialWombatPulseTimer SW8B_PT05 = SW18AB_PT07;
SerialWombatPulseTimer SW8B_PT06 = SW18AB_PT18;  
SerialWombatPulseTimer SW8B_PT07 = SW18AB_PT19;




SerialWombatPulseTimer* PulseTimerArray18AB[NUM_TEST_PINS] =
{
  &SW18AB_PT00, //0
  NULL, // 1
  NULL, //2
  NULL, //3
  NULL, //4
  &SW18AB_PT05, //5
  &SW18AB_PT06, //6
  &SW18AB_PT07, //7
  &SW18AB_PT08, //8
  &SW18AB_PT09, //9
  &SW18AB_PT10,//10
  &SW18AB_PT11,//11
  &SW18AB_PT12,//12
  &SW18AB_PT13,//13
  &SW18AB_PT14,//14
  &SW18AB_PT15,//15
  &SW18AB_PT16,//16
  &SW18AB_PT17,//17
  &SW18AB_PT18,//18
  &SW18AB_PT19//19
};

SerialWombatPulseTimer* PulseTimerArray08B[NUM_TEST_PINS] =
{
  &SW8B_PT00, //0
  &SW8B_PT01, // 1
  &SW8B_PT02, //2
  &SW8B_PT03, //3
  &SW8B_PT04, //4  
  &SW8B_PT05, //5
  &SW8B_PT06, //6
  &SW8B_PT07, //7
  NULL, //8
  NULL, //9
  NULL,//10
  NULL,//11
  NULL,//12
  NULL,//13
  NULL,//14
  NULL,//15
  NULL,//16
  NULL,//17
  NULL,//18
  NULL//19
};


void initializePulseReaduS(SerialWombatChip& sw, int pin)
{
  if (&sw == &SW18AB_6B)
  {
    if (pin < NUM_TEST_PINS)
    {
      if (PulseTimerArray18AB[pin] != NULL)
      {
        PulseTimerArray18AB[pin]->begin(SW18ABPinTo4BPin(pin));
     
      }
      else
      {
       test("TEST ERROR:  NULL PIN in initializePulseReaduS",0);
      }
    }
    else
    {
      test("TEST ERROR:  INVALID PIN in initializePulseReaduS",0);
    }
  }
  else if (&sw == &SW8B_68)
  {
    if (pin < NUM_TEST_PINS)
    {
      if (PulseTimerArray08B[pin] != NULL)
      {
        PulseTimerArray08B[pin]->begin(SW8BPinTo4BPin(pin)); 
      }
      else
      {
        test("TEST ERROR:  NULL PIN in initializePulseReaduS",0);
      }
    }
    else
    {
      test("TEST ERROR:  INVALID PIN in initializePulseReaduS",0);
    }
  }
  else
  {
    test("TEST ERROR:  INVALID CHIP in initializePulseReaduS",0);
  }
  
}

uint16_t dutyCycleRead(SerialWombatChip& sw,int pin)
{
  if (&sw == &SW18AB_6B)
  {
    if (pin < NUM_TEST_PINS)
    {
      if (PulseTimerArray18AB[pin] != NULL)
      {
        
       
        PulseTimerArray18AB[pin]->refreshHighCountsLowCounts();
      uint32_t result = 65536*PulseTimerArray18AB[pin]->HighCounts;
      result /= (PulseTimerArray18AB[pin]->HighCounts + PulseTimerArray18AB[pin]->LowCounts);
      return result;
      }
      else
      {
        test("TEST ERROR:  NULL PIN in pulseRead",0);
        return (0);
      }
    }
    else
    {
      test("TEST ERROR:  INVALID PIN in pulseRead",0);
      return 0;
    }
  }
  else if (&sw == &SW8B_68)
  {
    if (pin < NUM_TEST_PINS)
    {
      if (PulseTimerArray08B[pin] != NULL)
      {
         
        PulseTimerArray08B[pin]->refreshHighCountsLowCounts();
      uint32_t result = 65536*PulseTimerArray08B[pin]->HighCounts;
      result /= (PulseTimerArray18AB[pin]->HighCounts + PulseTimerArray08B[pin]->LowCounts);
      return result;

      }
      else
      {
        test("TEST ERROR:  NULL PIN in pulseRead",0);
        return (0);
      }
    }
    else
    {
      test("TEST ERROR:  INVALID PIN in pulseRead",0);
      return 0;
    }
  }
  else
  {
    test("TEST ERROR:  INVALID CHIP in pulseRead",0);
    return 0;
  }
 
}

uint16_t pulseRead(SerialWombatChip& sw,int pin)
{
  if (&sw == &SW18AB_6B)
  {
    if (pin < NUM_TEST_PINS)
    {
      if (PulseTimerArray18AB[pin] != NULL)
      {
        return PulseTimerArray18AB[pin]->readHighCounts();
      }
      else
      {
        test("TEST ERROR:  NULL PIN in pulseRead",0);
        return (0);
      }
    }
    else
    {
      test("TEST ERROR:  INVALID PIN in pulseRead",0);
      return 0;
    }
  }
  else if (&sw == &SW8B_68)
  {
    if (pin < NUM_TEST_PINS)
    {
      if (PulseTimerArray08B[pin] != NULL)
      {
        return PulseTimerArray08B[pin]->readHighCounts();
      }
      else
      {
        test("TEST ERROR:  NULL PIN in pulseRead",0);
        return (0);
      }
    }
    else
    {
      test("TEST ERROR:  INVALID PIN in pulseRead",0);
      return 0;
    }
  }
  else
  {
    test("TEST ERROR:  INVALID CHIP in pulseRead",0);
    return 0;
  }
  #if 0
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
  #endif
}




uint16_t pulseCounts(SerialWombatChip &sw,int pin)
{
  if (&sw == &SW18AB_6B)
  {
    if (pin < NUM_TEST_PINS)
    {
      if (PulseTimerArray18AB[pin] != NULL)
      {
        return PulseTimerArray18AB[pin]->readPulses();
      }
      else
      {
        test("TEST ERROR:  NULL PIN in pulseCounts",0);
        return (0);
      }
    }
    else
    {
      test("TEST ERROR:  INVALID PIN in pulseCounts",0);
      return 0;
    }
  }
  else if (&sw == &SW8B_68)
  {
    if (pin < NUM_TEST_PINS)
    {
      if (PulseTimerArray08B[pin] != NULL)
      {
        return PulseTimerArray08B[pin]->readPulses();
      }
      else
      {
        test("TEST ERROR:  NULL PIN in pulseCounts",0);
        return (0);
      }
    }
    else
    {
      test("TEST ERROR:  INVALID PIN in pulseCounts",0);
      return 0;
    }
  }
  else
  {
    test("TEST ERROR:  INVALID CHIP in pulseCounts",0);
    return 0;
  }
  
  #if 0
  if (pin < NUM_TEST_PINS)
  {
    if (PulseTimerArray[pin] != NULL)
    {
      return PulseTimerArray[pin]->readPulses();
    }
    else
    {
      Serial.println("TEST ERROR:  NULL PIN in pulseCounts");
      return (0);
    }
  }
  else
  {
    Serial.println("TEST ERROR:  INVALID PIN in pulseCounts");
    return 0;
  }
  #endif
}

bool withinRange(uint16_t value, uint16_t expected, uint8_t sixtyFourths, uint16_t counts)
{
  int32_t x32 = expected;
  if ((value > x32 + counts) && (value > x32 * (64 + sixtyFourths) / 64))
  {
    return (false);
  }
  if ((value < x32 - counts) && (value < x32 * (64 - sixtyFourths) / 64))
  {
    return (false);
  }
  return (true);
}

void test(const char* designator, uint16_t value, uint16_t expected , uint16_t counts , uint16_t sixtyFourths)
{
  if (withinRange(value, expected, sixtyFourths, counts))
  {
    pass(1);
  }
  else
  {
    fail(1);
    uint8_t failPacket[] = {0x40,0,SW_LE16(expected) , SW_LE16(value),0x55,0x55};
    SW18AB_6B.sendPacket(failPacket);
    Serial.print(designator);  Serial.print(" V: "); Serial.print(value); Serial.print(" X:"); Serial.println(expected);
  }
}
