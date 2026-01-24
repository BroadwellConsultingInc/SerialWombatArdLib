
#define mnC0 16
#define mnCs0 17
#define mnDb0 17
#define mnD0 18
#define mnDs0 19
#define mnEb0 19
#define mnE0 20
#define mnF0 21
#define mnFs0 23
#define mnGb0 23
#define mnG0 24
#define mnGs0 25
#define mnAb0 25
#define mnA0 27
#define mnAs0 29
#define mnBb0 29
#define mnB0 30
#define mnC1 32
#define mnCs1 34
#define mnDb1 34
#define mnD1 36
#define mnDs1 38
#define mnEb1 38
#define mnE1 41
#define mnF1 43
#define mnFs1 46
#define mnGb1 46
#define mnG1 49
#define mnGs1 51
#define mnAb1 51
#define mnA1 55
#define mnAs1 58
#define mnBb1 58
#define mnB1 61
#define mnC2 65
#define mnCs2 69
#define mnDb2 69
#define mnD2 73
#define mnDs2 77
#define mnEb2 77
#define mnE2 82
#define mnF2 87
#define mnFs2 92
#define mnGb2 92
#define mnG2 98
#define mnGs2 103
#define mnAb2 103
#define mnA2 110
#define mnAs2 116
#define mnBb2 116
#define mnB2 123
#define mnC3 130
#define mnCs3 138
#define mnDb3 138
#define mnD3 146
#define mnDs3 155
#define mnEb3 155
#define mnE3 164
#define mnF3 174
#define mnFs3 185
#define mnGb3 185
#define mnG3 196
#define mnGs3 207
#define mnAb3 207
#define mnA3 220
#define mnAs3 233
#define mnBb3 233
#define mnB3 246
#define mnC4 261
#define mnCs4 277
#define mnDb4 277
#define mnD4 293
#define mnDs4 311
#define mnEb4 311
#define mnE4 329
#define mnF4 349
#define mnFs4 369
#define mnGb4 369
#define mnG4 392
#define mnGs4 415
#define mnAb4 415
#define mnA4 440
#define mnAs4 466
#define mnBb4 466
#define mnB4 493
#define mnC5 523
#define mnCs5 554
#define mnDb5 554
#define mnD5 587
#define mnDs5 622
#define mnEb5 622
#define mnE5 659
#define mnF5 698
#define mnFs5 739
#define mnGb5 739
#define mnG5 783
#define mnGs5 830
#define mnAb5 830
#define mnA5 880
#define mnAs5 932
#define mnBb5 932
#define mnB5 987
#define mnC6 1046
#define mnCs6 1108
#define mnDb6 1108
#define mnD6 1174
#define mnDs6 1244
#define mnEb6 1244
#define mnE6 1318
#define mnF6 1396
#define mnFs6 1479
#define mnGb6 1479
#define mnG6 1567
#define mnGs6 1661
#define mnAb6 1661
#define mnA6 1760
#define mnAs6 1864
#define mnBb6 1864
#define mnB6 1975
#define mnC7 2093
#define mnCs7 2217
#define mnDb7 2217
#define mnD7 2349
#define mnDs7 2489
#define mnEb7 2489
#define mnE7 2637
#define mnF7 2793
#define mnFs7 2959
#define mnGb7 2959
#define mnG7 3135
#define mnGs7 3322
#define mnAb7 3322
#define mnA7 3520
#define mnAs7 3729
#define mnBb7 3729
#define mnB7 3951
#define mnC8 4186
#define mnCs8 4434
#define mnDb8 4434
#define mnD8 4698
#define mnDs8 4978
#define mnEb8 4978

#define qn 2  //Quarter Note
#define dqn 3  // Dotted Half Note
#define hn 4  // Half Note
#define en 1  // Eighth Note

uint16_t otj [NUMBER_OF_BUZZERS][NUMBER_OF_NOTES_MAX][2] =    // otj = OdeToJoy
{
{//Highest note
  {mnB5 , qn}, //1
  {mnB5 , qn},
  {mnC6 , qn},
  {mnD6 , qn},

  {mnD6 , qn}, //2
  {mnC6 , qn},
  {mnB5 , qn},
  {mnA5 , qn},

  {mnG5 , qn}, //3
  {mnG5 , qn},
  {mnA5 , qn},
  {mnB5 , qn},

  {mnB5 , dqn}, //4
  {mnA5 , en},
  {mnA5 , hn},



  {mnB5 , qn}, //5
  {mnB5 , qn},
  {mnC6 , qn},
  {mnD6 , qn},

  {mnD6 , qn}, //6
  {mnC6 , qn},
  {mnB5 , qn},
  {mnA5 , qn},

  {mnG5 , qn}, //7
  {mnG5 , qn},
  {mnA5 , qn},
  {mnB5 , qn},

  {mnA5 , dqn}, //8
  {mnG5 , en},
  {mnG5 , hn},



  
  {mnA5 , qn}, //9
  {mnA5 , qn},
  {mnB5 , qn},
  {mnG5 , qn},

  {mnA5 , qn}, //10
  {mnB5 , en},
  {mnC6 , en},
  {mnB5 , qn},
  {mnG5 , qn},

  {mnA5 , qn}, //11
  {mnB5 , en},
  {mnC6 , en},
  {mnB5 , qn},
  {mnA5 , qn},

  {mnG5 , qn}, //12
  {mnA5 , qn},
  {0 , qn},
  {mnB5 , qn},

{mnB5 , qn}, //13
  {mnB5 , qn},
  {mnC6 , qn},
  {mnD6 , qn},

  {mnD6 , qn}, //14
  {mnC6 , qn},
  {mnB5 , qn},
  {mnA5 , qn},

  {mnG5 , qn}, //15
  {mnG5 , qn},
  {mnA5 , qn},
  {mnB5 , qn},

  {mnA5 , dqn}, //16
  {mnG5 , en},
  {mnG5 , hn},

/*
  {mnB5 , qn},
  {mnB5 , qn},
  {mnC6 , qn},
  {mnD6 , qn},

  {mnD6 , qn},
  {mnC6 , qn},
  {mnB5 , qn},
  {mnA5 , qn},

  {mnG5 , qn},
  {mnG5 , qn},
  {mnA5 , qn},
  {mnB5 , qn},

  {mnA5 , dqn},
  {mnG5 , en},
  {mnG5 , hn},

*/



  
  {0, 0},
},

{     //2nd highest note
  {mnD5 , qn},
  {mnD5 , qn},
  {mnD5 , qn},
  {mnD5 , qn},

  {mnD5 , qn},
  {mnE5 , en},
  {mnFs5 , en},
  {mnG5 , qn},
  {mnD5 , qn},

  {mnD5 , qn},
  {mnD5 , qn},
  {mnFs5 , qn},
  {mnG5 , qn},

  {mnG5 , dqn},
  {mnFs5 , en},
  {mnFs5 , hn},



  {mnG5 , qn},
  {mnD5 , qn},
  {mnE5 , qn},
  {mnF5 , qn},

  {mnF5 , qn},
  {mnE5 , qn},
  {mnD5 , qn},
  {mnE5 , qn},

  {mnD5 , qn},
  {mnD5 , qn},
  {mnFs5 , qn},
  {mnG5 , qn},

  {mnFs5 , dqn},
  {0 , en},
  {0 , hn},

  {mnFs5 , qn}, //9
  {mnD5 , qn},
  {mnD5 , qn},
  {0 , qn},

  {mnFs5 , qn}, //10
  {mnG5 , en},
  {mnF5 , en},
  {mnG5 , qn},
  {0 , qn},

  {mnFs5 , qn}, //11
  {mnFs5 , qn},
  {mnFs5 , qn},
  {mnDs5 , qn},

  {mnE5 , en}, //12
  {mnD5 , en},
  {mnCs4 , qn},
  {mnD5 , qn},
   {mnD5 , qn},

  {mnD5 , qn}, //13
  {mnD5 , qn},
  {mnE5 , qn},
  {mnF5 , qn},

  {mnF5 , qn}, //14
  {mnE5 , qn},
  {mnD5 , qn},
  {mnE5 , qn},

  {mnD5 , qn}, //15
  {mnD5 , qn},
  {mnFs5 , qn},
  {mnG5 , qn},

  {mnFs5 , dqn}, //16
  {0 , en},
  {0 , hn},
  
  /*
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},




    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
  */
  {0, 0},
},

#if (NUMBER_OF_BUZZERS >= 3)
{
  {mnG4 , qn},
  {mnG4 , qn},
  {mnA4 , qn},
  {mnB4 , qn},

  {mnB4 , qn},
  {mnA4 , qn},
  {mnG4 , qn},
  {mnD4 , qn},

  {mnG4 , qn},
  {mnF4 , qn},
  {mnC5 , qn},
  {mnD5 , qn},

  {mnD5 , dqn},
  {mnD5 , en},
  {mnD5 , hn},



  {mnD5 , qn},
  {mnD5 , qn},
  {mnC5 , qn},
  {mnB4 , qn},

  {mnG4 , qn},
  {mnA4 , qn},
  {mnB4 , qn},
  {mnC5 , qn},

  {mnB4 , qn},
  {mnB4 , qn},
  {mnC5 , qn},
  {mnD5 , qn},

  {mnC5 , dqn},
  {mnB4 , en},
  {mnB4, hn},

  {mnD5 , qn},  //9
  {mnFs4 , qn},
  {mnG4 , qn},
  {mnB4 , qn},

  {mnC5 , qn},
  {mnB4 , en},
  {mnA4 , en},
  {mnG4 , qn},
  {mnB4 , qn},

  {mnC5 , qn},
  {mnB4 , en},
  {mnA4 , en},
  {mnB4 , qn},
  {mnB4 , qn},

  {mnB4 , qn},
  {mnA4 , en},
  {mnG4 , en},
  {mnFs4 , qn},
   {mnG4 , qn},


  {mnG4 , qn},  //12
  {mnD5 , qn},
  {mnC5 , qn},
  {mnB4 , qn},

  {mnG4 , qn},
  {mnA4 , qn},
  {mnB4 , qn},
  {mnC5 , qn},


  {mnB4 , qn},
  {mnB4 , qn},
  {mnC5 , qn},
  {mnD5 , qn},

  {mnC5 , dqn},
  {mnB4 , en},
  {mnB4 , hn},
  /*
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},




    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
  */
  {0, 0},
},
#endif
#if (NUMBER_OF_BUZZERS >= 4)

{
  {0 , qn},
  {0 , qn},
  {mnG4 , qn},
  {mnG4 , qn},

  {mnD4 , qn},
  {mnD4 , qn},
  {mnD4 , qn},
  {mnC3 , qn},

  {mnB3 , qn},
  {mnB3 , qn},
  {mnA3 , qn},
  {mnG3 , qn},

  {mnD4 , dqn},
  {mnD4 , en},
  {mnD4 , hn},



  {mnG4 , qn}, //5
  {mnG4 , qn},
  {mnG4 , qn},
  {mnG4 , qn},

  {mnC3 , qn},
  {mnC3 , qn},
  {mnC3 , qn},
  {mnC3 , qn},

  {mnD3 , qn},
  {mnD3 , qn},
  {mnD3 , qn},
  {mnD3 , qn},

  {mnD3 , dqn},
  {mnG4 , en},
  {mnG4, hn},



  {mnD4 , qn}, //9
  {mnD4 , qn},
  {mnD4 , qn},
  {mnD4 , qn},

  {mnD4 , qn},
  {mnD4 , qn},
  {mnD4 , qn},
  {mnD4 , qn},

  {mnD4 , qn},
  {mnD4 , qn},
  {mnDs4 , qn},
  {mnB3 , qn},

  {mnE4 , qn},
  {mnA3 , qn},
  {mnD4, qn},
  {0,qn},

  
  {mnG4 , qn},
  {mnG4 , qn},
  {mnG4 , qn},
  {mnG4 , qn},
  
  {mnC3 , qn},
  {mnC3 , qn},
  {mnC3 , qn},
  {mnC3 , qn},

  {mnD4 , qn},
  {mnD4 , qn},
  {mnD4 , qn},
  {mnD4 , qn},

  {mnD4 , dqn},
  {mnG4 , en},
  {mnG4, hn},

/*
{mnG4 , qn}, //13
  {mnG4 , qn},
  {mnG4 , qn},
  {mnG4 , qn},

  {mnC3 , qn},
  {mnC3 , qn},
  {mnC3 , qn},
  {mnC3 , qn},

  {mnD3 , qn},
  {mnD3 , qn},
  {mnD3 , qn},
  {mnD3 , qn},

  {mnD3 , dqn},
  {mnG4 , en},
  {mnG4, hn},

  */
  /*
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},




    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},

    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
    {mnx5 , qn},
  */
  {0, 0},
}
#endif
};
