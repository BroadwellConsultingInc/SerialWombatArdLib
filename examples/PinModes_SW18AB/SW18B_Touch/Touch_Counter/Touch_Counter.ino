#include "SerialWombat.h"


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

SerialWombat18CapTouch class documentation can be found here:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat18_cap_touch.html#details

A demonstration video of this class can be found here:
https://youtu.be/c4B0_DRVHs0

 */


#define I2C_ADDRESS 0x6B
#define PENNY_PIN 16  //Must be an Analog capable pin:  0,1,2,3,4,16,17,18,19
#define QUARTER_PIN 17 //Must be an Analog capable pin:  0,1,2,3,4,16,17,18,19

SerialWombat sw;
SerialWombat18CapTouch penny(sw);
SerialWombat18CapTouch quarter(sw);

SerialWombatButtonCounter quarterCounter(&quarter), pennyCounter(&penny);

long int moneyCount = 0;  //Place to keep track of total money count in pennies

void setup() {
  Wire.begin();  // Initialize I2C
  
  sw.begin(Wire,I2C_ADDRESS,false); //Initialize the Serial Wombat Chip
  
  Serial.begin(115200); //Initialize the UART

  delay(1000);
  
  VersionCheck();  //Check to ensure the Serial Wombat chip is responding (see other tab)

  // Initialize the Penny sensor
  //9000 based on previous calibration of this penny on this pin with this wire using the Calibration example
  penny.begin(PENNY_PIN,9000,0);  

  // Initialize the Penny sensor
  //9250 based on previous calibration of this quarter on this pin with this wire using the Calibration example
  quarter.begin(QUARTER_PIN,9250,0);
     
  delay(500);
  
  penny.makeDigital(53985,57620,1,0,0,0);//Low and High limits based on previous calibration of this penny on this pin with this wire
  quarter.makeDigital(54349,57792,1,0,0,0);//Low and High limits based on previous calibration of this quarter on this pin with this wire
  delay(250);

  pennyCounter.begin(&moneyCount, //moneyCount is the variable we want to increment.  
    1,  //Increment by 1
    500,//Every 500 ms 
    2000, // for 2000ms, then...
    1,  // by 1 
    250, // every 250ms 
    5000, // for 5000 ms, then
    1,    // by 1
    100); // every 100ms

    //Initialization of the quarter Counter is the same, but incrments by 25.
   quarterCounter.begin(&moneyCount, 25,500,2000,25,250,5000,25,100);

   Serial.println("Touch or hold the penny or the quarter:");

}


long int lastCount = -1;  // A copy of moneyCount so we can send a Serial update on changes.
void loop() {
  quarterCounter.update();  //Service the counter periodically
  pennyCounter.update();    //Serivce the counter periodically
  
  if (lastCount != moneyCount)  // Did the counter change the moneyCount variable?
  {
    //Yes, the counter changed
    lastCount = moneyCount;  //Make a copy for comparison

    //Then build a string and send it.
    char moneyCountStr[20];
    sprintf(moneyCountStr,"$%ld.%02ld",moneyCount / 100, moneyCount%100);
    Serial.println(moneyCountStr);
  }
}
