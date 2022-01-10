#include "SerialWombat.h"

SerialWombat sw;
SerialWombatTM1637 redDisplay(sw);
SerialWombatTM1637 yellowDisplay(sw);
SerialWombatTM1637 dummyDisplay(sw);
SerialWombatPWM pwm(sw);


#define SEG_A 0x1
#define SEG_B 0x2
#define SEG_C 0x4
#define SEG_D 0x8
#define SEG_E 0x10
#define SEG_F 0x20
#define SEG_G 0x40
#define SEG_POINT 0x80
#define OFF__ 0

#define ANIMATION_ARRAY snake1

#define VERTRIGHT (SEG_B | SEG_C)
#define VERTLEFT  (SEG_E | SEG_F)
#define TOP (SEG_A)
#define MID (SEG_G)
#define BOT (SEG_D)

uint8_t LeftToRight[18][6] = {
  {VERTLEFT, 0,0,0,0,0},
  {VERTRIGHT, 0,0,0,0,0},
  {0,VERTLEFT, 0,0,0,0},
  {0,VERTRIGHT, 0,0,0,0},
  {0,0,VERTLEFT, 0,0,0},
  {0,0,VERTRIGHT, 0,0,0},
  {0,0,0,VERTLEFT, 0,0},
  {0,0,0,VERTRIGHT, 0,0},
  {0,0,0,0,VERTLEFT, 0},
  {0,0,0,0,VERTRIGHT, 0},
  {0,0,0,0,0,VERTLEFT },
  {0,0,0,0,0,VERTRIGHT},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
   
};
uint8_t snake1[28][6] = {
  
  {TOP,0,0,0,0,0},
  {0,TOP,0,0,0,0},
  {0,0,TOP,0,0,0},
  {0,0,0,TOP,0,0},
  {0,0,0,0,TOP,0},
  {0,0,0,0,0,TOP},

  {0,0,0,0,0,SEG_B},

  {0,0,0,0,0,MID},
  {0,0,0,0,MID,0},
  {0,0,0,MID,0,0},
  {0,0,MID,0,0,0},
  {0,MID,0,0,0,0},
  {MID,0,0,0,0,0},

  {SEG_E,0,0,0,0,0},

  {BOT,0,0,0,0,0},
  {0,BOT,0,0,0,0},
  {0,0,BOT,0,0,0},
  {0,0,0,BOT,0,0},
  {0,0,0,0,BOT,0},
  {0,0,0,0,0,BOT},

  {0,0,0,0,0,SEG_B},

  {0,0,0,0,0,MID},
  {0,0,0,0,MID,0},
  {0,0,0,MID,0,0},
  {0,0,MID,0,0,0},
  {0,MID,0,0,0,0},
  {MID,0,0,0,0,0},

   {SEG_E,0,0,0,0,0},
};
/*
uint8_t Animation1[18][6] = {
  {SEG_A, SEG_B,SEG_C,SEG_D,SEG_E,SEG_F},
  {SEG_B, SEG_C,SEG_D,SEG_E,SEG_F,SEG_G},
  {SEG_C, SEG_D,SEG_E,SEG_F,SEG_G,SEG_A},
  {SEG_D, SEG_E,SEG_F,SEG_G,SEG_A,SEG_B},
  {SEG_E, SEG_F,SEG_G,SEG_A,SEG_B,SEG_C},
  {SEG_F, SEG_G,SEG_A,SEG_B,SEG_C,SEG_D},
  {SEG_G, SEG_A,SEG_B,SEG_C,SEG_D,SEG_E},
  {0,0,0,0,0,0},
    {0,0,0,0,0,0},
      {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
          {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
   
};
*/
void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  sw.begin(Wire,0x69,false);

  Serial.begin(115200);
  /*
  uint8_t tx6[8] = {200,19,11,18,6,0,67,0x55};
    sw.sendPacket(tx6);
    uint8_t tx6_201[8] = {201,19,11,2,1,0,5,4};
    sw.sendPacket(tx6_201);
   uint8_t tx6_202[8] = {202,19,11,3,1,2,3,4};
    sw.sendPacket(tx6_202);
*/
  sw.setThroughputPin(14);

  if(0)
  {
  redDisplay.begin(19,18,6,tm1637Decimal16,67,4);
  redDisplay.orderDigits(2,1,0,5,4,3);
  }
  {
  //redDisplay.begin(19,18,6,tm1637CharArray,67,4);
  //redDisplay.orderDigits(2,1,0,5,4,3);


  //uint8_t test[7] = "TEST`'";
  //redDisplay.setArray(test);
  }

  
  if(1)
  {
    // Average FrameTime
  yellowDisplay.begin(17,16,4,tm1637Hex16,74,4);
  yellowDisplay.orderDigits(2,3,4,5,4,5);
  }
  if(0)
  {
    //Frame counter
   dummyDisplay.begin(15,14,6,tm1637Decimal16,67,4);
   dummyDisplay.orderDigits(2,1,0,5,4,3);
  }
  /*
  uint8_t tx4[8] = {200,17,11,16,4,1,74,0x55};
    sw.sendPacket(tx4);

    uint8_t tx4_201[8] = {201,17,11,2,3,4,5,4};
    sw.sendPacket(tx4_201);
    uint8_t tx4_202[8] = {202,17,11,5,0,0,1,2};
    sw.sendPacket(tx4_202); 
    */
/*
    {
      //Raw Output
    uint8_t tx[8] = {205,17,11,0,3,0x55,0x55,0x55};
    sw.sendPacket(tx);
    uint8_t tx2[8] = {204,17,11,0,0,0,0,0};
    sw.sendPacket(tx2);

  }
  */
  
    
}
  uint16_t data = 12345;

void loop() {

  
 
  // put your main code here, to run repeatedly:
  ++data;
  Serial.println(data);
//sw.writePublicData(19,data);
if(1)
  {
   // sw.writeUserBuffer(0,(uint8_t*)ANIMATION_ARRAY,sizeof(ANIMATION_ARRAY));
  redDisplay.begin(19,18,6,tm1637Animation,0,4);
  redDisplay.orderDigits(2,1,0,5,4,3);
/*
  uint8_t test[6] = {SW_LE16(0),
                    SW_LE16(100),
                    sizeof(ANIMATION_ARRAY)/6,
                    0};
  redDisplay.setArray(test);
  */
  redDisplay.setAnimation(0x180,20,sizeof(ANIMATION_ARRAY)/6,ANIMATION_ARRAY);
    
  }
  delay(180000);
  
  
  

}
