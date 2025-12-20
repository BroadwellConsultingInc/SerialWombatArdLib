#include "SerialWombat.h"
#define SERVO_DEFAULT_BASE 544
#define SERVO_DEFAULT_VARIABLE 1856

#define PRINT_FAILURES



SerialWombatServo_18AB SW18AB_Servo0(SW18AB_6B);
//SerialWombatServo_18AB SW18AB_Servo1(SW18AB_6B); 
//SerialWombatServo_18AB SW18AB_Servo2(SW18AB_6B);
//SerialWombatServo_18AB SW18AB_Servo3(SW18AB_6B);
//SerialWombatServo_18AB SW18AB_Servo4(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo5(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo6(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo7(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo8(SW18AB_6B);   
SerialWombatServo_18AB SW18AB_Servo9(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo10(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo11(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo12(SW18AB_6B);   
SerialWombatServo_18AB SW18AB_Servo13(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo14(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo15(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo16(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo17(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo18(SW18AB_6B);
SerialWombatServo_18AB SW18AB_Servo19(SW18AB_6B);

SerialWombatServo_18AB* SW18ABServos[] = {
  &SW18AB_Servo0,
 NULL,
  NULL,
   NULL,
    NULL,
  &SW18AB_Servo5,
  &SW18AB_Servo6,
  &SW18AB_Servo7,
  &SW18AB_Servo8,   
  &SW18AB_Servo9,
  &SW18AB_Servo10,
  &SW18AB_Servo11,
  &SW18AB_Servo12,   
  &SW18AB_Servo13,
  &SW18AB_Servo14,
  &SW18AB_Servo15,
  &SW18AB_Servo16,
  &SW18AB_Servo17,
  &SW18AB_Servo18,
  &SW18AB_Servo19
};

SerialWombatServo_18AB SW8B_Servo0(SW8B_68);
SerialWombatServo_18AB SW8B_Servo1(SW8B_68); 
SerialWombatServo_18AB SW8B_Servo2(SW8B_68);
SerialWombatServo_18AB SW8B_Servo3(SW8B_68);
SerialWombatServo_18AB SW8B_Servo4(SW8B_68);
SerialWombatServo_18AB SW8B_Servo5(SW8B_68);
SerialWombatServo_18AB SW8B_Servo6(SW8B_68);
SerialWombatServo_18AB SW8B_Servo7(SW8B_68);

SerialWombatServo_18AB* SW8BServos[] = {
  &SW8B_Servo0,
  &SW8B_Servo1,
  &SW8B_Servo2,
  &SW8B_Servo3,
  &SW8B_Servo4,
  &SW8B_Servo5,
  &SW8B_Servo6,
  &SW8B_Servo7
};


SerialWombatServo SW4B_Servo1(SW4B_6C); 
SerialWombatServo SW4B_Servo2(SW4B_6C);
SerialWombatServo SW4B_Servo3(SW4B_6C); 

SerialWombatServo* SW4BServos[] = {
  &SW4B_Servo1,  // This should never be used
  &SW4B_Servo1,
  &SW4B_Servo2,
  &SW4B_Servo3
};


#if 0
SerialWombatHighFrequencyServo SWHFServo10(SW18AB_6B);
SerialWombatHighFrequencyServo SWHFServo11(SW18AB_6B);
SerialWombatHighFrequencyServo SWHFServo12(SW18AB_6B);
SerialWombatHighFrequencyServo SWHFServo13(SW18AB_6B);
SerialWombatHighFrequencyServo SWHFServo14(SW18AB_6B);
SerialWombatHighFrequencyServo SWHFServo15(SW18AB_6B);
#endif

#define SERVO_TEST_INCREMENTS 100
void servoTest(SerialWombatChip &sw , uint8_t startPin, uint8_t endPin)
{

  resetAll();
  for (int pin = startPin; pin <= endPin; ++ pin)  
  {
    
    if (!test_pinCanBeOutput(sw,pin))
    {
      continue;
    }
   
    initializePulseReaduS(sw,pin);
  }

  SerialWombatServo** ServoArray = NULL;
  if (&sw == &SW18AB_6B)
  {
    ServoArray = (SerialWombatServo**) SW18ABServos;
  }
  else if (&sw == &SW8B_68)
  {
    ServoArray = (SerialWombatServo**) SW8BServos;
  }
  else if (&sw == &SW4B_6C)
  {
    ServoArray = SW4BServos;
  }
  else
  {
    Serial.println("Invalid chip for servo test");
    return;
  }

  for (uint16_t variable = 800; variable <= 2000; variable += 100)
  {
    Serial.print(" iteration "); Serial.print((variable - 800) / 100); Serial.println(" of 13");
    for (uint16_t base = 500; base <= 1200; base += 100)
    {
      for (int reverse = 0; reverse < 2; ++ reverse)
      {
        for (int i = 0; i < SERVO_TEST_INCREMENTS; ++i)
        {

          for (int pin = startPin; pin <= endPin; ++ pin)
          {
          
            if (!test_pinCanBeOutput(sw,pin))
            {
              continue;
            }
           


            ServoArray[pin]-> attach(pin, base, base + variable, reverse);
            ServoArray[pin]->write16bit((i * 65535 ) / SERVO_TEST_INCREMENTS + (pin * 65535 / NUM_TEST_PINS));
          }
          delay(100);
          for (int pin = startPin; pin <= endPin; ++ pin)
          {
         
            if (!test_pinCanBeOutput(sw,pin))
            {
              continue;
            }
            


            uint16_t result = pulseRead(sw,pin);
            uint16_t setting = (i * 65535 ) / SERVO_TEST_INCREMENTS + (pin * 65535 / NUM_TEST_PINS);
            if (reverse)
            {
              setting = (65535 - setting);
            }
            int32_t expected = (variable) * (uint32_t)setting / 65536 + base;

            if ((result < (expected + (expected / 20)) + 20) && (result > (expected - (expected / 20) - 20))) // 2% clock error on receiver, 2% error on sender, worst case 4% either way plus a little round off error, +/- 20 for quant. error in DMA.
            {
              /*
                Serial.print(pin);
                Serial.print(" ");
                Serial.print(result);
                Serial.print(" ");
                Serial.print(expected);
                Serial.print(" ");
                Serial.print ("Pass");
                Serial.println();
              */
              pass(i);
            }
            else
            {
              #ifdef PRINT_FAILURES
              Serial.print(pin);Serial.print(" ");Serial.print(result);
              Serial.print(" ");Serial.print(expected);Serial.print(" ");
              Serial.print ("Fail");Serial.println();
              #endif
              
              fail(i);
            }


          }
        }
      }
    }
  }
#ifdef TODO
  testHSServo();
#endif
}
#if 0

void testHSServo()
{
  delay(10000);
  resetAll();

  initializePulseReaduS(10);
  SWHFServo10.attach(10,400,900,false);
  SWHFServo10.writeFrequency_Hz(700);

  initializePulseReaduS(11);
  SWHFServo11.attach(11,200,1000,false);
  SWHFServo11.writeFrequency_Hz(700);

  initializePulseReaduS(12);
  SWHFServo12.attach(12,200,500,false);
  SWHFServo12.writeFrequency_Hz(1100);

  initializePulseReaduS(13);
  SWHFServo13.attach(13,500,2500,false);
  SWHFServo13.writeFrequency_Hz(300);

  initializePulseReaduS(14);
  SWHFServo14.attach(14,500,1500,false);
  SWHFServo14.writeFrequency_Hz(300);

  initializePulseReaduS(15);
  SWHFServo15.attach(15,100,300,false);
  SWHFServo15.writeFrequency_Hz(1500);


  
  SWHFServo10.write16bit(0);
  SWHFServo11.write16bit(0);
  SWHFServo12.write16bit(0);
  SWHFServo13.write16bit(0);
  SWHFServo14.write16bit(0);
  SWHFServo15.write16bit(0);
  delay(10);
  uint16_t result = pulseRead(10);
  test("HSSERVO_0_10", result, 400,0,3);
  result = pulseRead(11);
  test("HSSERVO_0_11", result, 200,0,3);
  result = pulseRead(12);
  test("HSSERVO_0_12", result, 200,0,3);
  result = pulseRead(13);
  test("HSSERVO_0_13", result, 500,0,3);
  result = pulseRead(14);
  test("HSSERVO_0_14", result, 500,0,3);
  result = pulseRead(15);
  test("HSSERVO_0_16", result, 100,0,3);


  
  SWHFServo10.write16bit(0x8000);
   SWHFServo11.write16bit(0x8000);
    SWHFServo12.write16bit(0x8000);
     SWHFServo13.write16bit(0x8000);
      SWHFServo14.write16bit(0x8000);
       SWHFServo15.write16bit(0x8000);
  delay(10);
  result = pulseRead(10);
  test("HSSERVO_1_10", result, 650,0,3);
  result = pulseRead(11);
  test("HSSERVO_1_11", result, 600,0,3);
  result = pulseRead(12);
  test("HSSERVO_1_12", result, 350,0,3);
  result = pulseRead(13);
  test("HSSERVO_1_13", result, 1500,0,3);
  result = pulseRead(14);
  test("HSSERVO_1_14", result, 1000,0,3);
  result = pulseRead(15);
  test("HSSERVO_1_15", result, 200,0,3);

  SWHFServo10.write16bit(0xFFFF);
  SWHFServo11.write16bit(0xFFFF);
  SWHFServo12.write16bit(0xFFFF);
  SWHFServo13.write16bit(0xFFFF);
  SWHFServo14.write16bit(0xFFFF);
  SWHFServo15.write16bit(0xFFFF);
  delay(10);
  
   result = pulseRead(10);

  test("HSSERVO_2_10", result, 900,0,3);
  result = pulseRead(11);
  test("HSSERVO_2_11", result, 1000,0,3);
  result = pulseRead(12);
  test("HSSERVO_2_12", result, 500,0,3);
  result = pulseRead(13);
  test("HSSERVO_2_13", result, 2500,0,3);
  result = pulseRead(14);
  test("HSSERVO_2_14", result, 1500,0,3);
  result = pulseRead(15);
  test("HSSERVO_2_15", result, 300,0,3);


#if 0  
  uint16_t second = 0;
  uint16_t expected = 0;
  while (second < 10000)
  {
    Serial.println(second);
  uint16_t first = PulseTimerArray[10]->readPulses();
  delay(3000);
   second = PulseTimerArray[10]->readPulses();

   expected = first + 3*700;
  }
  test("HSSERVO_3", second, expected,0,3);
  Serial.println("A");
  second = 0;
   while (second < 10000)
  {
    Serial.println(second);
  uint16_t first = PulseTimerArray[11]->readPulses();
  delay(3000);
   second = PulseTimerArray[11]->readPulses();

   expected = first + 3*700;
  }
  test("HSSERVO_3_11", second, expected,0,3);
Serial.println("B");
  second = 0;
   while (second < 10000)
  {
    Serial.println(second);
  uint16_t first = PulseTimerArray[12]->readPulses();
  delay(3000);
   second = PulseTimerArray[12]->readPulses();

   expected = first + 3*1100;
  }
  test("HSSERVO_3_12", second, expected,0,3);
Serial.println("C");
    second = 0;
   while (second < 10000)
  {
    Serial.println(second);
  uint16_t first = PulseTimerArray[13]->readPulses();
  delay(3000);
   second = PulseTimerArray[13]->readPulses();

   expected = first + 3*300;
  }
  test("HSSERVO_3_13", second, expected,0,3);
Serial.println("D");
    second = 0;
   while (second < 10000)
  {
    Serial.println(second);
  uint16_t first = PulseTimerArray[14]->readPulses();
  delay(3000);
   second = PulseTimerArray[14]->readPulses();

   expected = first + 3*300;
  }
  test("HSSERVO_3_14", second, expected,0,3);
Serial.println("E");
    second = 0;
   while (second < 10000)
  {
    Serial.println(second);
  uint16_t first = PulseTimerArray[15]->readPulses();
  delay(3000);
   second = PulseTimerArray[15]->readPulses();

   expected = first + 3*1500;
  }
  test("HSSERVO_3_15", second, expected,0,3);
  Serial.println("F");
  #endif
}
#endif
