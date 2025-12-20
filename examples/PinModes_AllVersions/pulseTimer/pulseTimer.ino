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
	Serial.begin(115200);
	Wire.begin();
	delay(500);

	uint8_t i2cAddress = sw.find();

	sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port

	// Check for proper Serial Wombat version and pin mode, set error handler.  Not necessary, but useful for debugging problems
	errorChecking();  

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
