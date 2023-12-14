#pragma once
/// \file SerialWombatSimulatedQuadEnc.h

#include <stdint.h>
#include "SerialWombat.h"

typedef enum {
	SIMQE_IDLE,
	SIMQE_1ST_TRANSITION_1ST_PIN_COMPLETED,
	SIMQE_1ST_TRANSITION_2ND_PIN_COMPLETED,
	SIMQE_2ND_TRANSITION_1ST_PIN_COMPLETED,
	SIMQE_2ND_TRANSITION_2ND_PIN_COMPLETED,
	SIMQE_2ND_TRANSITION_COMPLETED
}SIMQUE_STATE_t;
/*! \brief A class for testing SerialWombatQuadEnc inputs.

This class uses digital IO calls to simulate the output of a quadrature encoder.  This is used to unit test
the SerialWombatQuadEnc class.  All processing occurs on the Arduino side.  This class does not currently
wrap a pin mode on the Serial Wombat chip.  See the Unit Test example sketch for usage.

*/
class SerialWombatSimulatedQuadEnc
{
public:
	SerialWombatSimulatedQuadEnc(SerialWombatChip& serialWombat0, SerialWombatChip& serialWombat1, uint8_t _pin0, uint8_t _pin1, bool openDrain, bool doubleTransition): sw0(serialWombat0), sw1(serialWombat1)
	{
		pin0State = true;
		pin1State = true;
		pin0 = _pin0;
		pin1 = _pin1;
		this->openDrain = openDrain;

		initialize();
	}

	void bothPinsHigh()
	{
		pin0High();
		pin1High();
	}
	void togglePin0()
	{
		if (pin0State)
		{
			pin0Low();
		}
		else
		{
			pin0High();
		}
	}

	void togglePin1()
	{
		if (pin1State)
		{
			pin1Low();
		}
		else
		{
			pin1High();
		}
	}
	void pin0High()
	{
		sw0.digitalWrite(pin0, HIGH);
		pin0State = true;
	}

	void pin1High()
	{
		sw1.digitalWrite(pin1, HIGH);
		pin1State = true;
	}


	void pin0Low()
	{
		sw0.digitalWrite(pin0, LOW);
		pin0State = false;
	}

	void pin1Low()
	{
		sw1.digitalWrite(pin1, LOW);
		pin1State = false;
	}

	bool service()
	{
		if (currentPosition != targetPosition)
		{
			switch (state)
			{
			case SIMQE_IDLE:
			{
				if (currentPosition != targetPosition)
				{
					lastTransitionTime_millis = millis();
					if (currentPosition < targetPosition)
					{
						togglePin0();
					}
					else 
					{
						togglePin1();
					}
					
					state = SIMQE_1ST_TRANSITION_1ST_PIN_COMPLETED;
				}

			}
			break;

			case SIMQE_1ST_TRANSITION_1ST_PIN_COMPLETED:
			{
				if (millis() > lastTransitionTime_millis + delayAfterFirstPinFirstTransition_mS)
				{
					lastTransitionTime_millis = millis();
					if (currentPosition < targetPosition)
					{
						togglePin1();
						if (!doubleTransition)
						{
							++currentPosition;
						}
					}
					else
					{
						togglePin0();
						if (!doubleTransition)
						{
							--currentPosition;
						}

					}
					
					
					state = SIMQE_1ST_TRANSITION_2ND_PIN_COMPLETED;
				}

			}
			break;

			case SIMQE_1ST_TRANSITION_2ND_PIN_COMPLETED:
			{
				if (millis() > lastTransitionTime_millis + delayAfterFirstPinSecondTransition_mS)
				{
					if (doubleTransition)
					{
						lastTransitionTime_millis = millis();
						if (currentPosition < targetPosition)
						{
							togglePin0();
						}
						else
						{
							togglePin1();
						}
						
						state = SIMQE_2ND_TRANSITION_2ND_PIN_COMPLETED;
					}
					else
					{
						state = SIMQE_IDLE;
					}
				}

			}
			break;
			case SIMQE_2ND_TRANSITION_1ST_PIN_COMPLETED:
			{
				if (millis() > lastTransitionTime_millis + delayAfterSecondPinFirstTransition_mS)
				{
					lastTransitionTime_millis = millis();
					if (currentPosition < targetPosition)
					{
						togglePin1();
						++currentPosition;

					}
					else
					{
						togglePin0();
						--currentPosition;

					}
					state = SIMQE_1ST_TRANSITION_2ND_PIN_COMPLETED;
				}

			}
			break;

			case SIMQE_2ND_TRANSITION_2ND_PIN_COMPLETED:
			{
				if (millis() > lastTransitionTime_millis + delayAfterFirstPinSecondTransition_mS)
				{
					
						state = SIMQE_IDLE;
				}

			}
			break;
			}
		}
		return state == SIMQE_IDLE;
	}
	void initialize()
	{
		sw0.pinMode(pin0, OUTPUT, false, openDrain);
		sw1.pinMode(pin1, OUTPUT, false, openDrain);
		bothPinsHigh();
		currentPosition = 0;
		targetPosition = 0;
		lastTransitionTime_millis = 0;
	}


	bool doubleTransition = false;
	SIMQUE_STATE_t state = SIMQE_IDLE;

	int32_t currentPosition = 0;
	int32_t targetPosition = 0;
	uint32_t delayAfterFirstPinFirstTransition_mS = 20;
	uint32_t delayAfterSecondPinFirstTransition_mS = 20;
	uint32_t delayAfterFirstPinSecondTransition_mS = 20;
	uint32_t delayAfterSecondPinSecondTransition_mS = 20;
	bool openDrain = true;
	uint32_t lastTransitionTime_millis = 0;

private:
	bool pin0State = true;
	bool pin1State = true;
	SerialWombatChip& sw0;
	SerialWombatChip& sw1;
	uint8_t pin0;
	uint8_t pin1;



};
