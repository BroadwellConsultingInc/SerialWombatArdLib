#include <SerialWombat.h>

SerialWombatChip sw;


void setup() {
#ifdef ARDUINO_ESP8266_GENERIC
  Wire.begin(2, 0); // ESP-01 - SDA GPIO2, SCL GPIO0
#else
  Wire.begin();
#endif

  Serial.begin(115200);

  delay(100);

  sw.begin(Wire, sw.find(true));
}

void loop() {
  
}
