SerialWombatIRRx irRx18(SW18AB_6B), irRx8(SW8B_68);
SerialWombatIRTx irTx18(SW18AB_6B), irTx8(SW8B_68);
void irTxRxTest()
{
  // Basic test using stream functions
  {
    irRx18.begin(19,
                 0, //Mode
                 true, //Use Repeat
                 SW_HIGH //Active
                );
    irTx8.begin(SW18ABPinTo8BPin(19), 0x1234);

    irTx18.begin(18,
                 0x2345 //Address
                );
    irRx8.begin(SW18ABPinTo8BPin(18),
                0, //Mode
                true, //Use Repeat
                SW_HIGH //Active
               );

    // Send a stream of commands to each and verify that they were received

    for (int i = 0; i < 9; ++i)
    {
      irTx8.write((uint8_t)(i + 80));
      irTx18.write((uint8_t)(i + 180));
    }
    delay(3000);
    for (int i = 0; i < 9; ++i)
    {
      test("irRx8 basic stream", irRx8.read(), i + 180);
      test("irRx18 basic stream", irRx18.read(), i + 80);
    }
  }

  //Test Repeat
  { 
    irRx18.begin(19,
                 0, //Mode
                 true, //Use Repeat
                 SW_HIGH //Active
                );
    irTx8.begin(SW18ABPinTo8BPin(19), 0x1234);

    irTx18.begin(18,
                 0x2345 //Address
                );
    irRx8.begin(SW18ABPinTo8BPin(18),
                0, //Mode
                true, //Use Repeat
                SW_HIGH //Active
               );

    // Send a command and 14 repeat commands.  Verify that the right number (15) are received


    irTx8.sendMessage(80, 0x5678, 14);
    irTx18.sendMessage(180, 0x6789, 14);

    delay(10000);
    for (int i = 0; i < 20; ++i)
    {
      if (i < 15)
      {
        test("irRx8 repeat", irRx8.read(), 180);
        test("irRx18 repeat", irRx18.read(), 80);
      }
      else
      {
        test("irRx8 repeat end", irRx8.read(), 65535);
        test("irRx18 repeat end", irRx18.read(), 65535);

      }
    }


  }

  //Test Addressing
  { 
    irRx18.begin(19,
                 0, //Mode
                 true, //Use Repeat
                 SW_HIGH, //Active
                 1000, //Timeout period
                 0x65535, // timeoutValue
                 true, //Use Address Filter
                 0x1234 //Address
                );
    irTx8.begin(SW18ABPinTo8BPin(19), 0x1234);

    irTx18.begin(18,
                 0x2345 //Address
                );
    irRx8.begin(SW18ABPinTo8BPin(18),
                 0, //Mode
                 true, //Use Repeat
                 SW_HIGH, //Active
                 1000, //Timeout period
                 0x65535, // timeoutValue
                 true, //Use Address Filter
                 0x2345 //Address
               );

    // Send a command and 14 repeat commands, and then a message with wrong address.  Verify that the right number (15) are received


    irTx8.sendMessage(80, 0x1234, 14);
    irTx18.sendMessage(180, 0x2345, 14);
    irTx8.sendMessage(80, 0x89AB, 14);
    irTx18.sendMessage(180, 0x89AB, 14);

    delay(10000);
    for (int i = 0; i < 20; ++i)
    {
      if (i < 15)
      {
        test("irRx8 addressed repeat", irRx8.read(), 180);
        test("irRx18 addressed repeat", irRx18.read(), 80);
      }
      else
      {
        test("irRx8 addressed repeat end", irRx8.read(), 65535);
        test("irRx18 addressedrepeat end", irRx18.read(), 65535);

      }
    }
    test ("irRx8 addressed repeat public data count",irRx8.readPublicData(),15);
    test ("irRx18 addressed repeat public data count",irRx18.readPublicData(),15);


  }
  //Test  public data Address
  { 
    irRx18.begin(19,
                 0, //Mode
                 true, //Use Repeat
                 SW_HIGH, //Active
                 1000, //Timeout period
                 0x65535, // timeoutValue
                 true, //Use Address Filter
                 0x1234, //Address
                 SerialWombatIRRx::publicDataOutput::ADDRESS
                );
    irTx8.begin(SW18ABPinTo8BPin(19), 0x1234);

    irTx18.begin(18,
                 0x2345 //Address
                );
    irRx8.begin(SW18ABPinTo8BPin(18),
                 0, //Mode
                 true, //Use Repeat
                 SW_HIGH, //Active
                 1000, //Timeout period
                 0x65535, // timeoutValue
                 true, //Use Address Filter
                 0x2345 //Address
               );

    // Send a command and 14 repeat commands, and then a message with wrong address.  Verify that the right number (15) are received


    irTx8.sendMessage(80, 0x1234, 14);
    irTx18.sendMessage(180, 0x2345, 14);
    irTx8.sendMessage(80, 0x89AB, 14);
    irTx18.sendMessage(180, 0x89AB, 14);

    delay(10000);
    for (int i = 0; i < 20; ++i)
    {
      if (i < 15)
      {
        test("irRx8 addressed repeat B", irRx8.read(), 180);
        test("irRx18 addressed repeat B", irRx18.read(), 80);
      }
      else
      {
        test("irRx8 addressed repeat end B", irRx8.read(), 65535);
        test("irRx18 addressedrepeat end B", irRx18.read(), 65535);

      }
    }
    test ("irRx8 addressed repeat public data address ",irRx8.readPublicData(),0x2345);
    test ("irRx18 addressed repeat public data address ",irRx18.readPublicData(),0x1234);


  }

  //Test  public data Address
  { 
    irRx18.begin(19,
                 0, //Mode
                 true, //Use Repeat
                 SW_HIGH, //Active
                 1000, //Timeout period
                 0x65535, // timeoutValue
                 true, //Use Address Filter
                 0x1234, //Address
                 SerialWombatIRRx::publicDataOutput::COMMAND
                );
    irTx8.begin(SW18ABPinTo8BPin(19), 0x1234);

    irTx18.begin(18,
                 0x2345 //Address
                );
    irRx8.begin(SW18ABPinTo8BPin(18),
                 0, //Mode
                 true, //Use Repeat
                 SW_HIGH, //Active
                 1000, //Timeout period
                 0x65535, // timeoutValue
                 true, //Use Address Filter
                 0x2345 //Address
               );

    // Send a command and 14 repeat commands, and then a message with wrong address.  Verify that the right number (15) are received


    irTx8.sendMessage(80, 0x1234, 14);
    irTx18.sendMessage(180, 0x2345, 14);
    irTx8.sendMessage(80, 0x89AB, 14);
    irTx18.sendMessage(180, 0x89AB, 14);

    delay(10000);
    for (int i = 0; i < 20; ++i)
    {
      if (i < 15)
      {
        test("irRx8 addressed repeat C", irRx8.read(), 180);
        test("irRx18 addressed repeat C", irRx18.read(), 80);
      }
      else
      {
        test("irRx8 addressed repeat end C", irRx8.read(), 65535);
        test("irRx18 addressedrepeat end C", irRx18.read(), 65535);

      }
    }
    test ("irRx8 addressed repeat public data address ",irRx8.readPublicData(),180);
    test ("irRx18 addressed repeat public data address ",irRx18.readPublicData(),80);


  }
}
