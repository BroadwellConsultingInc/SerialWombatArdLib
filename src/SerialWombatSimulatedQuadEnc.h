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
/*! /brief Class for a Serial Wombat chip.  Each Serial Wombat chip on a project should have its own instance.

This class describes the capabilties of a Serial Wombat Chip that are not Pin Mode functionalities

*/
class SerialWombatSimulatedQuadEnc
{
public:
	SerialWombatSimulatedQuadEnc(SerialWombatChip& serialWombat0, SerialWombatChip& serialWombat1, uint8_t pin0, uint8_t pin1, bool openDrain, bool doubleTransition);

	void bothPinsHigh();
	void togglePin0();
	void togglePin1();
	void pin0High();
	void pin1High();
	void pin0Low();
	void pin1Low();
	bool service();
	void initialize();


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