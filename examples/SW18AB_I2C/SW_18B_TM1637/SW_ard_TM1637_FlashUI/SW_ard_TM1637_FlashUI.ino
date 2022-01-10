#include "SerialWombat.h"

#define PENNY_PIN 16  //Must be an Analog capable pin:  0,1,2,3,4,16,17,18,19
#define QUARTER_PIN 17 //Must be an Analog capable pin:  0,1,2,3,4,16,17,18,19


/*
  This example shows how to configure two Serial Wombat 18AB pins to Touch input and use the
  SerialWombat18CapTouchCounter class to implement a two touch sensor interface to increment
  a counter at various speeds by two different increments.

  The example was created using a Serial Wombat 18AB chip in I2C mode with a Node MCU clone Arduino
  and a penny and quarter both covered with electrial tape wired to pins WP16 and WP17.

  When the penny is touched briefly the total will increment by 1 cent.  When the quarter is touched
  the total will increment by 25 cents.  If a finger is held on them then they will increment slowly, then
  more quickly, then very quickly.  This type of interface could be easily integrated into a complete solution
  for user configuration of parameters.

A video demonstrating the use of the TM1637 pin mode on the Serial Wombat 18AB chip is available at:
https://youtu.be/AwW12n6o_T0

Documentation for the SerialWombatTM1637 Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_t_m1637.html#details
*/


SerialWombat sw;
SerialWombat18CapTouch penny(sw);
SerialWombat18CapTouch quarter(sw);
SerialWombatTM1637 myDisplay(sw);
SerialWombat18CapTouchCounter quarterCounter(&quarter);



long int digitChange = 0;
uint8_t displayString[7] = "000000";
uint8_t currentDigit = 6; // 6 means none, 0-5 are the displayed digits

void setup() {
  Wire.begin();  // Initialize I2C
 Serial.begin(115200); //Initialize the UART

  delay(1000);
  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) {showNotFoundError();}
  sw.begin(Wire,i2cAddress,false);

  // Initialize the Penny sensor
  //9000 based on previous calibration of this penny on this pin with this wire using the Calibration example
  penny.begin(PENNY_PIN, 9000, 0);

  // Initialize the Penny sensor
  //9250 based on previous calibration of this quarter on this pin with this wire using the Calibration example
  quarter.begin(QUARTER_PIN, 9250, 0);

  delay(500);

  penny.makeDigital(53985, 57620, 1, 0, 0, 0); //Low and High limits based on previous calibration of this penny on this pin with this wire
  quarter.makeDigital(54349, 57792, 1, 0, 0, 0); //Low and High limits based on previous calibration of this quarter on this pin with this wire
  delay(250);

  quarterCounter.begin(&digitChange,
                       1,  //Increment by 1
                       500,//Every 500 ms
                       2000, // for 2000ms, then...
                       1,  // by 1
                       250, // every 250ms
                       5000, // for 5000 ms, then
                       1,    // by 1
                       100); // every 100ms

  myDisplay.begin(19,  //Clk Pin
                  18, // Data Pin
                  6, // Number of digits
                  tm1637CharArray, // Mode enumeration
                  0x55, // Source pin Not used in tm1637CharArray mode
                  4);   // Brightness
  myDisplay.writeDigitOrder(2, 1, 0, 5, 4, 3);
  myDisplay.writeArray(displayString);
}





void loop() {


  if (penny.readTransitionsState() && penny.transitions > 0)
  {
    // The penny was touched.  Move to next digit
    nextDigit();
  }

  quarterCounter.update();
  
  if (currentDigit < 6 && digitChange != 0)
  {
    displayString[currentDigit] += digitChange;
    if (displayString[currentDigit] > 'z')
    {
      displayString[currentDigit] = ' ';
    }

    if (displayString[currentDigit] < ' ')
    {
      displayString[currentDigit] = 'z';
    }

    digitChange = 0;
    myDisplay.writeArray(displayString);
  }

}

int nextDigit()
{
  digitChange = 0;
  ++currentDigit;
  if (currentDigit > 6)
  {
    currentDigit = 0;
  }

  switch (currentDigit)
  {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    {
      myDisplay.writeBlinkBitmap(0x01 << currentDigit); // Update which digit blinks
      
    }
    break;

    case 6:
    {
      myDisplay.writeBlinkBitmap(0); // Turn off blinking.
    }
    break;

    default:
    {
      currentDigit = 6;
      myDisplay.writeBlinkBitmap(0); // Turn off blinking.      
    }
    break;
   
  }

  return(0);
}
