#include <SerialWombat.h>

SerialWombat sw;    //Declare a Serial Wombat
SerialWombatUART SWUart(sw);  // Declare a Serial Wombat UART  Only one UART can be assigned on the SerialWombat 4B.  This pin mode does not work on Serial Wombat 4A.

// There is a video tutorial to go with this example at:  https://youtu.be/C1FjcaiBYZs
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);    
  Serial.println("initialize Wire...");
  {//I2C Initialization
    Wire.begin();
    Wire.setClock(400000);
    Serial.println("begin sw...");
    sw.begin(Wire,0x6C,false);  //Initialize the Serial Wombat library to use the primary I2C port, This SerialWombat's address is 6C.
    delay(100);
  }
  Serial.println("Start SWUart...");
  SWUart.begin(115200,1,0,1); 
    Serial.println("Initialization Complete...");
}

//32,7,5,3,2,1
#define MASK 0x80000057
uint16_t random(uint32_t *seed)
{

  uint16_t output = 0;
  for (int util_local_i = 0; util_local_i < 16; ++util_local_i)
  {
    if (*seed & 0x00000001)
    {
      *seed = (((*seed ^ MASK) >> 1) | 0x80000000);
    }
    else
    {
      *seed >>= 1;
    }
    output <<= 1;
    output |= *seed & 0x01;
  }
  return((output));
}

uint32_t random1 = 6;
uint32_t random2 = 6;
uint32_t iteration = 0;
uint32_t fails = 0;
uint32_t passes = 0;
#define BUFFER_SIZE 128

uint8_t tx[BUFFER_SIZE];
uint8_t rx[BUFFER_SIZE];
    
void loop() {
  // put your main code here, to run repeatedly:

    
    while (SWUart.read() != -1){Serial.println("Flush");} // Flush rx


    // Fill the TX buffer with random data
    int txcount = random(&random1) % BUFFER_SIZE;
    for (int x = 0; x < txcount; ++x)
    {     
      tx[x] = (uint8_t)random(&random1);
    }

    SWUart.write(tx, txcount);



    // read the bytes back in and compare them against the random # generator
    int rxcount = random(&random2) % BUFFER_SIZE;
    int c = SWUart.readBytes((char*)rx,rxcount);
    bool mismatch = false;
    for (int x = 0; x < rxcount; ++x)
    {
      byte b;
      b = (byte)random(&random2);
      if (b != rx[x])
      {
        mismatch = true;
        ++fails;
        Serial.write("Mismatch: ");
        Serial.print(iteration);
        Serial.write(' ');
        Serial.print(x);
        Serial.write(' ');
        Serial.print(b);
        Serial.write(' ');
        Serial.println(rx[x]);
      } 

    }
    if (!mismatch)
    {
      ++passes;
      Serial.write("P  ");
      Serial.print(passes);
      Serial.write(" F ");
      Serial.println(fails);
    }
    ++iteration;
    

}
