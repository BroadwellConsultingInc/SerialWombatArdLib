#include <SerialWombat.h>


SerialWombatChip sw;
SerialWombatAnalogInput swAn0(sw);
SerialWombatServo swServo(sw);
void setup() {
  Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial


  swAn0.begin(0);
  swServo.attach(1,1000,15000);

}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t pot = swAn0.readCounts();
  Serial.println(pot);
  swServo.write16bit(pot);
  delay(100);
}
