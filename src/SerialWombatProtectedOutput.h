#pragma once
#include "SerialWombat.h"
/*! \file SerialWombatProtectedOutput.h
*/


typedef enum
{
    PO_FAULT_IF_NOT_EQUAL = 0,
    PO_FAULT_IF_FEEDBACK_LESS_THAN_EXPECTED= 1,
    PO_FAULT_IF_FEEDBACK_GREATER_THAN_EXPECTED = 2,
} PO_COMPARE_t;


/*! \brief Combine and input and output pin for a protected output

The SerialWombtProtectedOutput class is assigned to a Serial Wombat
output.  It monitors another previously configured pin's public data,
such as a digital I/O value or an Analog input.  If the monitored value
does not meet expectations, then the protected pin changes values to
a configured state.   This allows the Serial Wombat to constantly
verify a condition without the need for constant polling from the 
host device.

\warning The SerialWombatProtectedOutput is intended to help prevent
accidental damage to hobby circuitry.  The Serial Wombat and its associated
libraries are not designed for use in Safety Critical applications.  The 
Serial Wombat should not be used in situations where a malfunction or design
defect could result in damage to property, economic loss,
or harm to living people or creatures.

The period of time that a mismatch must occur before going to the safeState
is configurable.

Video Tutorial

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/p8CO04C1q_Y" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/p8CO04C1q_Y
*/
class SerialWombatProtectedOutput
{
public:
    /// \brief Constructor for SerialWombatProtectedOutput
    /// \param serialWombat The serial Wombat on which the Protected Output runs
    SerialWombatProtectedOutput(SerialWombat& serialWombat);
    ///  Initialize the Protected Output.  No pin state changes happen with this function
    /// \param pin The output pin of the Protected output   Valid values for SW4A: 0-3  SW4B: 1-3 
    /// \param monitoredPin The input pin for the Protected Output.  Must be initialized before this call
    void begin(uint8_t pin, uint8_t monitoredPin);
    /// Configure a protected output.  This function will change the pin output to activeState unless fault conditions are met
    /// \param compareMode  Enumerated type of PO_COMPARE_t .
    /// \param compareValue  A 16 bit unsigned value that will be compared with the public data from the monitored pin and compared based on compareMode
    /// \param debounceTime How long, in mS the compareMode fault condition must exist before transitioning to safeState
    /// \param activeState The initial, pre-fault state for the output (SW_LOW, SW_HIGH, SW_INPUT).  Do not use Arduino HIGH or LOW or INPUT constants.
    /// \param safeState The safe, post-fault state for the output (SW_LOW, SW_HIGH, SW_INPUT).  Do not use Arduino HIGH or LOW or INPUT constants.
    void configure(PO_COMPARE_t compareMode, uint16_t compareValue, uint8_t debounceTime, SerialWombatPinState_t activeState, SerialWombatPinState_t safeState);
    ///  \brief Queries the Serial Wombat to see if the protected output has entered safe mode due to a fault
    /// \return True if in safe state, false if in openState
    bool isInSafeState();
    /// \brief Turn off protection features and make the protected pin an input
    void makeInput();
    /// \brief Turn off the protection features and make the protected pin an unprotected output 
    /// 
    /// This function is compatible with the Arduino digital write, and therefore uses HIGH and LOW
    /// as parameters rather than SW_HIGH or SW_LOW
    /// /param state HIGH or LOW
    void digitalWrite(uint8_t state);

private:
    SerialWombat& _sw;
    uint8_t _pin = 255;
    uint8_t _monitoredPin = 255;
    PO_COMPARE_t _compareMode = PO_FAULT_IF_NOT_EQUAL;
    uint8_t _debounceTime = 0;
    SerialWombatPinState_t _safeState = SW_INPUT;
    SerialWombatPinState_t _activeState = SW_INPUT;
    uint16_t _compareValue;
};

