#pragma once
#include <stdint.h>
#include "SerialWombat.h"
/*! \file SerialWombatPWM.h
*/
class SerialWombat;

typedef enum 
{
    SW4AB_PWMFrequency_1_Hz = 0x76,
    SW4AB_PWMFrequency_2_Hz = 0x66,
    SW4AB_PWMFrequency_4_Hz = 0x56,
    SW4AB_PWMFrequency_8_Hz = 0x46,
    SW4AB_PWMFrequency_16_Hz = 0x75,
    SW4AB_PWMFrequency_32_Hz = 0x65,
    SW4AB_PWMFrequency_63_Hz = 0x55,
    SW4AB_PWMFrequency_125_Hz = 0x45,
    SW4AB_PWMFrequency_244_Hz = 0x71,
    SW4AB_PWMFrequency_488_Hz = 0x61,
    SW4AB_PWMFrequency_976_Hz = 0x51,
    SW4AB_PWMFrequency_1952_Hz = 0x41,
    SW4AB_PWMFrequency_3900_Hz = 0x31,
    SW4AB_PWMFrequency_7800_Hz = 0x21,
    SW4AB_PWMFrequency_15625_Hz = 0x11,
    SW4AB_PWMFrequency_31250_Hz = 0x01,
}Wombat4A_B_PWMFrequencyValues_t;

/// \brief A class representing a Serial Wombat PWM output
/// 
/// An instance of this class should be declared for each pin
/// to be used as a Serial Wombat PWM.  
/// 
/// PWMs are initialized to a frequency of 31250 Hz at startup.
/// This frequency can be changed using the setFrequency_SW4AB method.
/// All PWM outputs use the same clock divider, so a change in frequency
/// to one PWM output will affect other outputs.
/// 
/// PWM inputs are either 8 or 10 bit resolution, depending on frequency
/// selection.  The duty cycle parameter of methods that set duty cycle
/// take a 16 bit value ranging from 0 to 65535 as an input regardless of
/// resolution, with 0 being
/// always low, and 65535 being always high.
/// 

class SerialWombatPWM
{
public:
    /// \brief Constructor for SerialWombatPWM class
    /// \param serialWombat SerialWombat on which the PWM will run
	SerialWombatPWM(SerialWombat& serialWombat );

    /// \brief Initialize a pin that has been declared as PWM.  Starts with 0 duty cycle
    /// \param pin The pin to become a PWM.  Valid values for SW4A: 0-3  SW4B: 1-3 
	void begin(uint8_t pin);

    /// \brief Initialize a pin that has been declared as PWM. 
   /// \param pin The pin to become a PWM.  Valid values for SW4A: 0-3  SW4B: 1-3 
   /// \param dutyCycle A value from 0 to 65535 representing duty cycle
	void begin(uint8_t pin, uint16_t dutyCycle);

    /// \brief Initialize a pin that has been declared as PWM. 
   /// \param pin The pin to become a PWM.  Valid values for SW4A: 0-3  SW4B: 1-3 
   /// \param dutyCycle A value from 0 to 65535 representing duty cycle
   /// \param invert if true, internally adjust duty cycle to 65535-duty cycle
    void begin(uint8_t pin, uint16_t dutyCycle,bool invert);

    /// \brief Set PWM duty cycle
    /// \param dutyCycle A value from 0 to 65535 representing duty cycle
	void writeDutyCycle(uint16_t dutyCycle);

    /// \brief Set PWM Frequency (Adjusts all PWM outputs' frequency on a SerialWombat)
    /// \param frequency  A value of the #Wombat4A_B_PWMFrequencyValues_t enumeration
    /// 
    /// This function changes the Serial Wombat 4A and 4B PWM output frequncy by adjusting
    /// the clock divisor for the PWM generation hardware.  By default the value is 31250Hz.
    /// Changing the frequency may reduce PWM resolution from 10 bits to 8 bits for some
    /// frequencies.  However, the input value for duty cycle for methods of this class
    /// continue to be 0 to 65535 and are scaled accordingly.
    /// 
    /// \warning This function will likely not be compatible with other models in the Serial Wombat
    /// family based on other hardware that are released in the future because it is tightly coupled to the
    /// PIC16F15214 hardware.
    void setFrequency_SW4AB(Wombat4A_B_PWMFrequencyValues_t frequency);


    void setFrequency_SW18AB_Hz(uint16_t frequency_Hz);
    void setPeriod_SW18AB_uS(uint16_t period_uS);

	~SerialWombatPWM();

private:
	SerialWombat* _sw;
	uint8_t _pin = 255;
};

