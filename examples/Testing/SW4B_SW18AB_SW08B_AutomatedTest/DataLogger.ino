SerialWombat18ABDataLogger swdl(SW18AB_6B);
SerialWombatQueue dlQueue(SW18AB_6B);
#define DLQUEUELENGTH 512
#define DLQUEUEADDR 300
void dataLoggerTest()
{
  SerialWombatPWM p0(SW18AB_6B), p8(SW18AB_6B),p10(SW18AB_6B), p17(SW18AB_6B); // Set to PWMs to allow config of public data
  resetAll();
     p0.begin(0);
    p8.begin(8);
    p10.begin(10);
    p17.begin(17);

{
 dlQueue.begin(DLQUEUEADDR,DLQUEUELENGTH);
  swdl.begin(DLQUEUEADDR,DLQUEUELENGTH,true, // Queue Frame Index
    false, // QueueOnChange
    DataLoggerPeriod::PERIOD_128mS);

    swdl.configurePin(0,true,true);
    swdl.configurePin(8,true,true);
    swdl.configurePin(10,true,true);
    swdl.configurePin(17,true,true);
    swdl.enable(true);
    delay(130);
    p0.writePublicData(1000);
    delay(130);
    p8.writePublicData(2000);
    delay(130);
    p10.writePublicData(3000);
    delay(130);
    p17.writePublicData(4000);
    delay(130);
    swdl.enable(false);

    // Should be at least 5 * 2 * 5 = 50 data bytes available, no more than 60
    {
      int i = dlQueue.available();
      if (i >= 50 && i <=60)
      {
        i = 50;
      }
      test("DataLogger_00", i, 50);
      
    }
    uint16_t d0[5];
    uint16_t d1[5];
    {
      // Test that first entry is all 0's
      
      dlQueue.readUInt16(d0,5);
      dlQueue.readUInt16(d1,5);

      for (int i = 1; i < 5; ++i)
      {
        char st[20];
        sprintf(st,"Datalogger_01_%d",i);
        test(st, d0[i], 0);
      }
    }
    {
      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
        dlQueue.readUInt16(d1,5);
      }
      else
      {
        test("Datalogger_02",d1[0]-d0[0],128);
      }
      test("Datalogger_03", d1[1], 1000);
      test("Datalogger_04", d1[2], 0);
      test("Datalogger_05", d1[3], 0);
      test("Datalogger_06", d1[4], 0);
    }
    {
      memcpy(d0,d1,10);
      dlQueue.readUInt16(d1,5);
      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
        dlQueue.readUInt16(d1,5);
      }
      else
      {
        test("Datalogger_07",d1[0]-d0[0],128);
      }
         test("Datalogger_08", d1[1], 1000);
      test("Datalogger_09", d1[2], 2000);
      test("Datalogger_10", d1[3], 0);
      test("Datalogger_11", d1[4], 0);
    }
    {
      memcpy(d0,d1,10);
      dlQueue.readUInt16(d1,5);
      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
        dlQueue.readUInt16(d1,5);
      }
      else
      {
        test("Datalogger_12",d1[0]-d0[0],128);
      }
         test("Datalogger_13", d1[1], 1000);
      test("Datalogger_14", d1[2], 2000);
      test("Datalogger_15", d1[3], 3000);
      test("Datalogger_16", d1[4], 0);
    }
    {
      memcpy(d0,d1,10);
      dlQueue.readUInt16(d1,5);
      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
        dlQueue.readUInt16(d1,5);
      }
      else
      {
        test("Datalogger_17",d1[0]-d0[0],128);
      }
         test("Datalogger_18", d1[1], 1000);
      test("Datalogger_19", d1[2], 2000);
      test("Datalogger_20", d1[3], 3000);
      test("Datalogger_21", d1[4], 4000);
    }
}   

{
  swdl.enable(false);
   p0.writePublicData(0);
    p8.writePublicData(0);
    p10.writePublicData(0);
    p17.writePublicData(0);
 dlQueue.begin(DLQUEUEADDR,DLQUEUELENGTH);
  swdl.begin(DLQUEUEADDR,DLQUEUELENGTH,true, // Queue Frame Index
    false, // QueueOnChange
    DataLoggerPeriod::PERIOD_128mS);

    swdl.configurePin(0,true,true);
    swdl.configurePin(8,true,true);
    swdl.configurePin(10,true,false);
    swdl.configurePin(17,false,true);
    swdl.enable(true);
    delay(130);
    p0.writePublicData(1000);
    delay(130);
    p8.writePublicData(2000);
    delay(130);
    p10.writePublicData(3000);
    delay(130);
    p17.writePublicData(4000);
    delay(130);
    swdl.enable(false);

    // Should be at least 40 data bytes available, no more than 48
    {
      int i = dlQueue.available();
      if (i >= 40 && i <=48)
      {
        i = 40;
      }
      test("DataLogger_22", i, 40);
      
    }
    uint16_t d0[5];
    uint16_t d1[5];
    {
      // Test that first entry is all 0's
      
      dlQueue.readUInt16(d0,3);
      d0[3] = dlQueue.read();
      d0[4] = dlQueue.read();
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();

      for (int i = 1; i < 5; ++i)
      {
        char st[20];
        sprintf(st,"Datalogger_21_%d",i);
        test(st, d0[i], 0);
      }
    }
    {
      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();

      }
      else
      {
        test("Datalogger_22",d1[0]-d0[0],128);
      }
      test("Datalogger_23", d1[1], 1000);
      test("Datalogger_24", d1[2], 0);
      test("Datalogger_25", d1[3], 0);
      test("Datalogger_26", d1[4], 0);
    }
    {
      memcpy(d0,d1,10);
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();
      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();
      }
      else
      {
        test("Datalogger_27",d1[0]-d0[0],128);
      }
         test("Datalogger_28", d1[1], 1000);
      test("Datalogger_29", d1[2], 2000);
      test("Datalogger_30", d1[3], 0);
      test("Datalogger_31", d1[4], 0);
    }
    {
      memcpy(d0,d1,10);
       dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();

      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();

      }
      else
      {
        test("Datalogger_32",d1[0]-d0[0],128);
      }
         test("Datalogger_33", d1[1], 1000);
      test("Datalogger_34", d1[2], 2000);
      test("Datalogger_35", d1[3], 3000 & 0xFF);
      test("Datalogger_36", d1[4], 0);
    }
    {
      memcpy(d0,d1,10);
            dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();

      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();
      }
      else
      {
        test("Datalogger_37",d1[0]-d0[0],128);
      }
         test("Datalogger_38", d1[1], 1000);
      test("Datalogger_39", d1[2], 2000);
      test("Datalogger_40", d1[3], 3000 & 0xFF);
      test("Datalogger_41", d1[4], 4000 >>8);
    }
}
  
{
  swdl.enable(false);
   p0.writePublicData(0);
    p8.writePublicData(0);
    p10.writePublicData(0);
    p17.writePublicData(0);
 dlQueue.begin(DLQUEUEADDR,DLQUEUELENGTH);
  swdl.begin(DLQUEUEADDR,DLQUEUELENGTH,true, // Queue Frame Index
    true // QueueOnChange
    );

    swdl.configurePin(0,true,true);
    swdl.configurePin(8,true,true);
    swdl.configurePin(10,true,false);
    swdl.configurePin(17,false,true);
    swdl.enable(true);
    delay(100);
    p0.writePublicData(1000);
    delay(200);
    p8.writePublicData(2000);
    delay(300);
    p10.writePublicData(3000);
    delay(400);
    p17.writePublicData(4000);
    delay(500);
    swdl.enable(false);

    // Should be at least 40 data bytes available, no more than 48
    {
      int i = dlQueue.available();
      if (i >= 40 && i <=48)
      {
        i = 40;
      }
      test("DataLogger_42", i, 40);
      
    }
    uint16_t d0[5];
    uint16_t d1[5];
    {
      // Test that first entry is all 0's
      
      dlQueue.readUInt16(d0,3);
      d0[3] = dlQueue.read();
      d0[4] = dlQueue.read();
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();

      for (int i = 1; i < 5; ++i)
      {
        char st[20];
        sprintf(st,"Datalogger_42_%d",i);
        test(st, d0[i], 0);
      }
    }
    {
      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();

      }
      else
      {
        if (d1[0] < d0[0])
        {
          d1[0] += 65536;
        }
        test("Datalogger_43",d1[0]-d0[0],100,10);
      }
      test("Datalogger_44", d1[1], 1000);
      test("Datalogger_45", d1[2], 0);
      test("Datalogger_46", d1[3], 0);
      test("Datalogger_47", d1[4], 0);
    }
    {
      memcpy(d0,d1,10);
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();
      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();
      }
      else
      {
        if (d1[0] < d0[0])
        {
          d1[0] += 65536;
        }
        test("Datalogger_48",d1[0]-d0[0],200,10);
      }
         test("Datalogger_49", d1[1], 1000);
      test("Datalogger_50", d1[2], 2000);
      test("Datalogger_51", d1[3], 0);
      test("Datalogger_52", d1[4], 0);
    }
    {
      memcpy(d0,d1,10);
       dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();

      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();

      }
      else
      {
                if (d1[0] < d0[0])
        {
          d1[0] += 65536;
        }

        test("Datalogger_53",d1[0]-d0[0],300,10);
      }
         test("Datalogger_54", d1[1], 1000);
      test("Datalogger_55", d1[2], 2000);
      test("Datalogger_56", d1[3], 3000 & 0xFF);
      test("Datalogger_57", d1[4], 0);
    }
    {
      memcpy(d0,d1,10);
            dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();

      if (d0[1] == d1[1] && d0[2] == d1[2] && d0[3] == d1[3] && d0[4] == d1[4])
      {
      dlQueue.readUInt16(d1,3);
      d1[3] = dlQueue.read();
      d1[4] = dlQueue.read();
      }
      else
      {
                if (d1[0] < d0[0])
        {
          d1[0] += 65536;
        }

        test("Datalogger_58",d1[0]-d0[0],400,10);
      }
         test("Datalogger_59", d1[1], 1000);
      test("Datalogger_60", d1[2], 2000);
      test("Datalogger_61", d1[3], 3000 & 0xFF);
      test("Datalogger_62", d1[4], 4000 >>8);
    }
}


}
