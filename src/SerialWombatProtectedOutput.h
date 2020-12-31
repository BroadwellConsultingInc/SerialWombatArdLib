#pragma once
#include "SerialWombat.h"
typedef enum
{
    PO_FAULT_IF_NOT_EQUAL = 0,
    PO_FAULT_IF_FEEDBACK_LESS_THAN_EXPECTED= 1,
    PO_FAULT_IF_FEEDBACK_GREATER_THAN_EXPECTED = 2,
} PO_COMPARE_t;
class SerialWombatProtectedOutput
{
public:
    SerialWombatProtectedOutput(SerialWombat& serialWombat);
    void begin(uint8_t pin, uint8_t monitoredPin);
    void configure(PO_COMPARE_t compareMode, uint16_t compareValue, uint8_t debounceTime, SerialWombatPinState_t activeState, SerialWombatPinState_t safeState);
    bool isInSafeState();
    void makeInput();
    void digitalWrite(uint8_t state);

private:
    SerialWombat& _sw;
    uint8_t _pin;
    uint8_t _monitoredPin = 255;
    PO_COMPARE_t _compareMode = PO_FAULT_IF_NOT_EQUAL;
    uint8_t _debounceTime = 0;
    SerialWombatPinState_t _safeState = SW_INPUT;
    SerialWombatPinState_t _activeState = SW_INPUT;
    uint16_t _compareValue;
};

