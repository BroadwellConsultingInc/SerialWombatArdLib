#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip
SerialWombatPulseTimer steering(sw);
SerialWombatPulseTimer throttle(sw);
SerialWombatPulseTimer button(sw);
SerialWombatPulseTimer thumbSwitch(sw);

#define STEERING_PIN 0
#define THROTTLE_PIN 1
#define BUTTON_PIN 2
#define THUMBSWITCH_PIN 3
// This example is explained in a video tutorial at: https://youtu.be/YtQWUub9gYw

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Pulse Timer Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:

  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  if (sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_PULSETIMER))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  
  steering.begin(STEERING_PIN);
  throttle.begin(THROTTLE_PIN);
  button.begin(BUTTON_PIN);
  thumbSwitch.begin(THUMBSWITCH_PIN); // Change this to a different pin on 18AB, since 3 is SDA


}

void clearTerminal()
{
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
}

int i;
void loop() {
  clearTerminal();
  Serial.println(steering.readHighCounts());
  Serial.println(throttle.readHighCounts());
  Serial.println(button.readHighCounts());
  Serial.println(thumbSwitch.readHighCounts());

  delay(50);

}
