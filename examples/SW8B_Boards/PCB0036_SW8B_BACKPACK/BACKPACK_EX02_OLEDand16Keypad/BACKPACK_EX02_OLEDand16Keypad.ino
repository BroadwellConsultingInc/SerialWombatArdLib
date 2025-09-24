
#include <SerialWombat.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

SerialWombatChip sw;


                SerialWombatMatrixKeypad Pin0Keypad(sw); // Your serial wombat chip may be named something else than sw

void setup() {
  //Wire.begin();
  Serial.begin(115200);

  delay(500);
  Serial.println("Starting Display");
  delay(500);
display.begin(0x3C, false);

  display.clearDisplay();
  display.setTextSize(3);      // Normal 1:1 pixel scale
  display.setTextColor(SH110X_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.write("READY");
  display.display();
 



  sw.begin(Wire, sw.find(true));
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}

  
               
              
                                Pin0Keypad.begin(0, //Control pin
                4, //Row 0 pin
                5, // Row 1 pin
                6, // Row 2 pin
                7, // Row 3 pin
                0, // Col 0 pin
                1, // Col 1 pin
                2, // Col 2 pin
                    3, // Col 3 pin
                    1,  //Public data mode
                    1, //Queue Mode
                    5); // Row Delay in mS
                
           Pin0Keypad.writeAsciiTable(0, (uint8_t)'1');
           Pin0Keypad.writeAsciiTable(1, (uint8_t)'2');
           Pin0Keypad.writeAsciiTable(2, (uint8_t)'3');
           Pin0Keypad.writeAsciiTable(3, (uint8_t)'A');
           Pin0Keypad.writeAsciiTable(4, (uint8_t)'4');
           Pin0Keypad.writeAsciiTable(5, (uint8_t)'5');
           Pin0Keypad.writeAsciiTable(6, (uint8_t)'6');
           Pin0Keypad.writeAsciiTable(7, (uint8_t)'B');
           Pin0Keypad.writeAsciiTable(8, (uint8_t)'7');
           Pin0Keypad.writeAsciiTable(9, (uint8_t)'8');
           Pin0Keypad.writeAsciiTable(10, (uint8_t)'9');
           Pin0Keypad.writeAsciiTable(11, (uint8_t)'C');
           Pin0Keypad.writeAsciiTable(12, (uint8_t)'*');
           Pin0Keypad.writeAsciiTable(13, (uint8_t)'0');
           Pin0Keypad.writeAsciiTable(14, (uint8_t)'#');
           Pin0Keypad.writeAsciiTable(15, (uint8_t)'D');

}

void loop() {

  int x = Pin0Keypad.read();
  if (x >=0)
  {
    Serial.print((char)x);
    display.clearDisplay();
    display.setTextSize(7);      // Normal 1:1 pixel scale
  display.setTextColor(SH110X_WHITE); // Draw white text
  display.setCursor(40, 10);     // Start at top-left corner
  display.write((char)x);
  display.display();
  
    
  }
  
  
}
