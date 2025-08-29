#include <SerialWombat.h>

#define GRIP_I2C_ADDRESS 0x60
PCB0031_Grip grip;


void setup() {
#ifdef ARDUINO_ESP8266_GENERIC
  Wire.begin(2, 0); // ESP-01 - SDA GPIO2, SCL GPIO0
#else
  Wire.begin();
#endif

  Serial.begin(115200);

  delay(500);

  grip.begin(0x60, true);
  grip.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if (!grip.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }



  // Output Scaling configurations for this pin
  grip.gs0.writeScalingEnabled(true, //Enabled
                               grip.gs0.pin()); //DataSource
  //put this line in setup.
  grip.gs0.writeRateControl(SerialWombatAbstractScaledOutput::Period::PERIOD_16mS, // Sampling Period
                            (uint16_t)1000, //Maximum Increase Rate
                            (uint16_t)1000); //Maximum Decrease Rate

}

void loop() {
  grip.gs0.writePublicData(0);
  delay(2000);
  grip.gs0.writePublicData(65535);
  delay(2000);
}
