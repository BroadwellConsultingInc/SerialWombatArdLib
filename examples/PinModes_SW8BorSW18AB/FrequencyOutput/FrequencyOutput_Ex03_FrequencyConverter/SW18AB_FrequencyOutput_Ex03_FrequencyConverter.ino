#include "SerialWombat.h"
/*
  This example shows how to add an offset or multiplier to an incoming frequency and output a modified frequency.
  This functionality can be very useful as "glue hardware" connecting two pieces of hardware that have different
  scaling for what frequency means.  For instance, changing the diameter of a tire can cause a speedometer to
  display an incorrect speed.  This functionalty could be used inbetween the wheel sensor and the speedometer to
  correct the output.  In the example below, we will increase the frequency by 5%.
  

 
  In this example pins 0 and 1 should be connected together.
  Pin 0 will use the frequency output mode.  We will use this pin to simulate an incoming waveform.
  
  Pin 1 will be in Pulse Measurement mode, configured to measure frequency, with an mx+b modification
  provided using the processed input functionality available on may numerical input pins.  This pin would be attached
  to an actual input in a non-example situation.
  
  Pin 2 will be in frequency output mode, and will output the frequency measured on pin 1 using the scaled output mode.
  
  A video demonstrating the use of the Freiquency Output  is available at:
  TODO

*/

SerialWombatChip sw;
SerialWombatFrequencyOutput_18AB freqOutput(sw);
SerialWombatFrequencyOutput_18AB freqSim(sw);

SerialWombatPulseTimer_18AB pulseTimerIn(sw); // Your serial wombat chip may be named something else than sw

#define FREQUENCY_OUTPUT_PIN 2
#define FREQUENCY_INPUT_PIN 1
#define FREQUENCY_INPUT_SIMULATION 0  // Connect this to pin 1 if you want something to measure


void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
   Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port
 if (i2cAddress == 0) {showNotFoundError();} // see showErrorNotFound tab
  sw.begin(Wire,i2cAddress); 
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  
 sw.queryVersion();
  Serial.println();
  Serial.print("Version "); Serial.println((char*)sw.fwVersion);
  versionCheck(); //see showErrorNotFound tab
  Serial.println("SW18AB Found.");


  // Set up the simulated input and output frequency pins, set the low freqeuency option
  freqOutput.begin(FREQUENCY_OUTPUT_PIN, 150, true);  
  freqSim.begin(FREQUENCY_INPUT_SIMULATION, 150, true);


  // Set up the input.  Since we're using slow (hundreds of Hz or less) frequencies, we'll measure in 
  // mS rather than uS.
  
  pulseTimerIn.begin(FREQUENCY_INPUT_PIN,  //Pin
                           SW_PULSETIMER_mS, //Units
                           false); // Pull Up enabled

  pulseTimerIn.configurePublicDataOutput( SerialWombatPulseTimer_18AB::publicDataOutput::FREQUENCY_ON_LTH_TRANSITION);
  // Configure the pulse timer to output frequency rather than pulse high time.                           
  pulseTimerIn.configureOutputValue(SerialWombatAbstractProcessedInput::OutputValue::RAW);

  //Enable the Processed Input functionality on the pulse timer, and set the mx+b to a 10% increase
  pulseTimerIn.writeProcessedInputEnable(true);
  pulseTimerIn.writeTransformLinearMXB(282, // m in 1/256ths, = to a 10% increase.
      0);

  //Set up pin 19 to output a frequency based on pin 18's scaled output
    freqOutput.writeScalingEnabled(true, //Enabled
                                 18); //DataSource

}


uint16_t simulatedFrequency = 50;
void loop() {
  freqSim.writePublicData(simulatedFrequency);
  delay(2000);
  Serial.print("In frequency: "); Serial.print(simulatedFrequency); Serial.print(" Out frequency: ");
  Serial.println(pulseTimerIn.readPublicData());

  simulatedFrequency += 5;
  if (simulatedFrequency > 100)
  {
    simulatedFrequency = 10;
  }
}
