/*
This example shows how to tune the Serial Wombat 18AB chip's internal FRC oscillator against 
the host's millis() function to reduce the error in the FRC from as much as +/- 1.5% at room
temperature to less than 0.1% .

This sketch runs for 1 minute to profile and display the nominal error in the FRC vs. millis()
then begins calling the update function of SerialWombat18ABOscillatorTuner.  The system then displays
the improvement in accurary as the oscillator is tuned.

A video demonstrating the use of the SerialWombat18ABOscillatorTuner class on the Serial Wombat 18AB chip is available at:
https://youtu.be/T2uBQM3s_JM
Documentation for the SerialWombat18ABOscillatorTuner Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat18_a_b_oscillator_tuner.html
*/

#include <SerialWombat.h>

SerialWombatChip sw;

uint32_t millisStart = 0;
uint32_t framesStart = 0;
uint32_t nextUpdate = 0;
SerialWombat18ABOscillatorTuner oscTun(sw);
void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("High Speed Counter Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
  if (!sw.isSW18())
  {
    Serial.println("This Example is not supported on the Serial Wombat 4B or 8B chip.  An  18AB chip is required.");
    while (1) {
      delay(100);
    }
  }
  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  uint32_t framesStartmsb = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW);
  uint16_t framesStartlsb = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_LSW);
  if (framesStartmsb != sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW))
  {
   framesStartlsb = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_LSW);
   framesStartmsb = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW); 
  }
  framesStart = framesStartmsb <<16;
  framesStart += framesStartlsb;
  millisStart = millis();
  nextUpdate = millis() + 60000;

  Serial.println("System will test Serial Wombat frame count for 1 minute, then start tuning algorithm, and show results every minute.");

}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t m = millis();
  if (m > nextUpdate)
  {
    uint16_t frameslsb = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_LSW);
  uint32_t frames = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW);
  if (frames != sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW))
  {
   frameslsb = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_LSW);
   frames = sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW); 
  }
  frames <<= 16;
  frames += frameslsb;
    Serial.print("millis elapsed: ");  Serial.print (m - millisStart); Serial.print(" frames run: "); Serial.print(frames-framesStart);
    Serial.print(" d: "); Serial.print((int32_t)(m - millisStart) - (int32_t)(frames-framesStart));
    Serial.print(" % (+ is SW too slow): "); Serial.println((float)((int32_t)(m - millisStart) - (int32_t)(frames-framesStart))/ (m - millisStart) * 100);
    nextUpdate = millis() + 60000;
    framesStart = frames;
    millisStart = m;
  }
  if (millis() > 70000)
  {
  oscTun.update();  // Start tuning after the first minute.
  }

}
