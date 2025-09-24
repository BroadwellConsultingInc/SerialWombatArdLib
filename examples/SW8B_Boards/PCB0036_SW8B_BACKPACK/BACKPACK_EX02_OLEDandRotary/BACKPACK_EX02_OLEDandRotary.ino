#include <SerialWombat.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

SerialWombatChip sw;
SerialWombatQuadEnc_18AB Pin6QuadEnc(sw);
SerialWombatDebouncedInput conf(sw);
SerialWombatDebouncedInput back(sw);
SerialWombatDebouncedInput push(sw);
void redraw(void);

void setup() {
 
  //Wire.begin(); // This is in Adafruit's display.begin, so we comment it out.  Call the display begin before other I2C devices
  
  Serial.begin(115200);
  delay(500);
  
  display.begin(0x3C, true);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.write("Test");
  display.display();


  delay(500);

  sw.begin(Wire, sw.find(true));
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial

  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }



  Pin6QuadEnc.begin(6, //1st Pin
                    5, //2nd Pin
                    0, //DebouceTime in mS
                    true, //pull ups
                    QE_READ_MODE_t ::QE_ONHIGH_POLL //Mode
                   );
  Pin6QuadEnc.writeMinMaxIncrementTargetPin(0, 16); // Min and max

  conf.begin(1,  //Pin
             20, //Debounce Ms
             true, // Invert
             true); // Pull Up enabled
  back.begin(7,  //Pin
             20, //Debounce Ms
             true, // Invert
             true); // Pull Up enabled

  push.begin(4,  //Pin
             20, //Debounce Ms
             true, // Invert
             true); // Pull Up enabled

  redraw();
}

int boxY = 0; // Value from 0 to 7
uint16_t lastX = 0;
bool boxOutline = true;



void loop() {
  if (conf.readTransitionsState() ==  false && conf.transitions > 0)
  {
    if (boxY < 7)
    {
      ++boxY;
      redraw();
    }
  }

  if (back.readTransitionsState() ==  false && back.transitions > 0)
  {
    if (boxY > 0)
    {
      --boxY;
      redraw();
    }
  }

  if (push.readTransitionsState() ==  false && push.transitions > 0)
  {
    boxOutline = !boxOutline;
    redraw();
  }

  uint16_t newX = Pin6QuadEnc.readPublicData();

  if (newX != lastX)
  {
    lastX = newX;
    redraw();
  }

}


void redraw()
{
  display.clearDisplay();
  if (boxOutline)
  {
    display.drawRect(lastX * 8, boxY * 8, 8, 8, SH110X_WHITE);
  }
  else
  {
    display.fillRect(lastX * 8, boxY * 8, 8, 8, SH110X_WHITE);
  }
  display.display();
}
