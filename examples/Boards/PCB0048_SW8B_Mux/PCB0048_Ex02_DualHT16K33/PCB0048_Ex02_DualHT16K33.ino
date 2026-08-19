/*  This example shows how to use the Serial Wombat Mux board to control two I2C devices with   
 *   the same address by separating them onto separate segments on the I2C bus controlled by mux chips
 *   from a Serial Wombat 8B chip.
 *   
 *   This example assumes that four HT16K33 4 character alphanumeric displays are attached to 
 *   a MUX board.  all have address 0x70.  
 *   
 *   The Example intializes one display after turning on MUX segment 2 to show SERI
 *   It then switches to MUX segment 3 and displays AL W on the second display, then
 *   OMBA on segment 1 and T 8B on segment 7.  The result will be "SERIAL WOMBAT 8B" across 4 displays.
 *   This order is designed to make the physical layout of the displays convenient based on the location
 *   of the bus segments on the PCB0048 board.
 *   
 *   This example assumes the Adafruit LED Backpack and supporting libraries have been
 *   installed.
 *   
 *   Video on the PCB0048 MUX:
 *   
 *   TODO coming soon
 * 
 *   PCB0048 Mux documentation:
 *   
 *   https://serwom.com/p48
 */

#include <SerialWombat.h>

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

PCB0048_Mux swMux;
#define MUX_I2C_ADDRESS 0x60
Adafruit_AlphaNum4 alpha4_1 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha4_2 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha4_3 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha4_7 = Adafruit_AlphaNum4();

void setup() {
  // put your setup code here, to run once:
  Wire.begin(); //Initialize I2C
  Serial.begin(115200);  //Initialize Serial
  delay(200);
  swMux.begin(MUX_I2C_ADDRESS);

 

 swMux.enableBus2Only();
    alpha4_2.begin(0x70);  // pass in the address
  alpha4_2.setBrightness(0);
  alpha4_2.writeDigitAscii(0,'S');
  alpha4_2.writeDigitAscii(1,'E');
  alpha4_2.writeDigitAscii(2,'R');
  alpha4_2.writeDigitAscii(3,'I');
  alpha4_2.writeDisplay();

swMux.enableBus3Only();
    alpha4_3.begin(0x70);  // pass in the address
  alpha4_3.setBrightness(0);
  alpha4_3.writeDigitAscii(0,'A');
  alpha4_3.writeDigitAscii(1,'L');
  alpha4_3.writeDigitAscii(2,' ');
  alpha4_3.writeDigitAscii(3,'W');
  alpha4_3.writeDisplay();
  
  swMux.enableBus1Only();
  alpha4_1.begin(0x70);  // pass in the address
  alpha4_1.setBrightness(0);
  alpha4_1.writeDigitAscii(0,'O');
  alpha4_1.writeDigitAscii(1,'M');
  alpha4_1.writeDigitAscii(2,'B');
  alpha4_1.writeDigitAscii(3,'A');
  alpha4_1.writeDisplay();
  
  swMux.enableBus7Only();
    alpha4_7.begin(0x70);  // pass in the address
  alpha4_7.setBrightness(0);
  alpha4_7.writeDigitAscii(0,'T');
  alpha4_7.writeDigitAscii(1,' ');
  alpha4_7.writeDigitAscii(2,'8');
  alpha4_7.writeDigitAscii(3,'B');
  alpha4_7.writeDisplay();
}

void loop() {
 
  

}
