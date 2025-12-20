#include "SerialWombat.h"

SerialWombatPWM_18AB SW18AB_PWM0(SW18AB_6B);
//SerialWombatPWM_18AB SW18AB_PWM1(SW18AB_6B);
//SerialWombatPWM_18AB SW18AB_PWM2(SW18AB_6B);
//SerialWombatPWM_18AB SW18AB_PWM3(SW18AB_6B);
//SerialWombatPWM_18AB SW18AB_PWM4(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM5(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM6(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM7(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM8(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM9(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM10(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM11(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM12(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM13(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM14(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM15(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM16(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM17(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM18(SW18AB_6B);
SerialWombatPWM_18AB SW18AB_PWM19(SW18AB_6B);

SerialWombatPWM_18AB* SW18ABPWMs[] = {
  &SW18AB_PWM0,
  NULL,
  NULL,
  NULL,
  NULL,
  &SW18AB_PWM5,
  &SW18AB_PWM6,
  &SW18AB_PWM7,
  &SW18AB_PWM8,
  &SW18AB_PWM9,
  &SW18AB_PWM10,
  &SW18AB_PWM11,
  &SW18AB_PWM12,
  &SW18AB_PWM13,
  &SW18AB_PWM14,
  &SW18AB_PWM15,
  &SW18AB_PWM16,
  &SW18AB_PWM17,
  &SW18AB_PWM18,
  &SW18AB_PWM19
};

SerialWombatPWM_18AB SW8B_PWM0(SW8B_68);
SerialWombatPWM_18AB SW8B_PWM1(SW8B_68);
SerialWombatPWM_18AB SW8B_PWM2(SW8B_68);
SerialWombatPWM_18AB SW8B_PWM3(SW8B_68);
SerialWombatPWM_18AB SW8B_PWM4(SW8B_68);
SerialWombatPWM_18AB SW8B_PWM5(SW8B_68);
SerialWombatPWM_18AB SW8B_PWM6(SW8B_68);
SerialWombatPWM_18AB SW8B_PWM7(SW8B_68);

SerialWombatPWM_18AB* SW8BPWMs[] = {
  &SW8B_PWM0,
  &SW8B_PWM1,
  &SW8B_PWM2,
  &SW8B_PWM3,
  &SW8B_PWM4,
  &SW8B_PWM5,
  &SW8B_PWM6,
  &SW8B_PWM7
};


SerialWombatPWM SW4B_PWM1(SW4B_6C);
SerialWombatPWM SW4B_PWM2(SW4B_6C);
SerialWombatPWM SW4B_PWM3(SW4B_6C);

SerialWombatPWM* SW4BPWMs[] = {
  &SW4B_PWM1,  // This should never be used
  &SW4B_PWM1,
  &SW4B_PWM2,
  &SW4B_PWM3
};



#define PWM_TEST_INCREMENTS 100
void pwmTest(SerialWombatChip &sw , uint8_t startPin, uint8_t endPin)
{

  resetAll();
  for (int pin = startPin; pin <= endPin; ++ pin)
  {

    if (!test_pinCanBeOutput(sw, pin))
    {
      continue;
    }

    initializePulseReaduS(sw, pin);
  }

  SerialWombatPWM_18AB** PWMArray = NULL;
  if (&sw == &SW18AB_6B)
  {
    PWMArray =  SW18ABPWMs;
  }
  else if (&sw == &SW8B_68)
  {
    PWMArray = SW8BPWMs;
  }
  /*
    else if (&sw == &SW4B_6C)
    {
    PWMArray = SW4BPWMs;
    }
  */
  else
  {
    Serial.println("Invalid chip for pwm test");
    return;
  }

  for (uint32_t pwmPeriod_uS = 122; pwmPeriod_uS <= 50000; pwmPeriod_uS *= 2)
  {
    for (int pin = startPin; pin <= endPin; ++ pin)
    {

      if (!test_pinCanBeOutput(sw, pin))
      {
        continue;
      }
      PWMArray[pin]-> begin(pin, 0 );
      PWMArray[pin]->writePeriod_uS(pwmPeriod_uS);
    }
    for (uint16_t duty = 0x0100; duty <= 0xFE00; duty += 0x80)
    {
      for (int pin = startPin; pin <= endPin; ++ pin)
      {
        if (!test_pinCanBeOutput(sw, pin))
        {
          continue;
        }


        uint16_t pinDuty = duty + 0x1000 * pin;
        if (pinDuty == 0)
        {
          pinDuty = 0x1000;
        }
        PWMArray[pin]-> writePublicData( pinDuty );

      }
      delay(10 * pwmPeriod_uS / 1000);
      for (int pin = startPin; pin <= endPin; ++ pin)
      {

        if (!test_pinCanBeOutput(sw, pin))
        {
          continue;
        }


        uint16_t pinDuty = duty + 0x1000 * pin;
        uint16_t result = pulseRead(sw, pin);
        uint32_t setting = (((uint32_t)pwmPeriod_uS) * (pinDuty)) >> 16;
        if (setting < 50)
        {
          continue;
        }
        /*TODO if (reverse)
          {
          setting = (65535 - setting);
          }
        */
        char s[70];
        sprintf(s, "PWM 01 Pin: %d Period: %lu Duty cycle: %d ", pin, pwmPeriod_uS, pinDuty);
        test(s, result, setting, 30, 5);
      }
    }
  }
}
