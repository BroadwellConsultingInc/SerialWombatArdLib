#include <SerialWombat.h>



SerialWombatChip sw;
SerialWombatFrequencyOutput_18AB buzz4(sw);
SerialWombatFrequencyOutput_18AB buzz5(sw);
SerialWombatFrequencyOutput_18AB buzz6(sw);
SerialWombatFrequencyOutput_18AB buzz7(sw);

#define NUMBER_OF_BUZZERS 4
#define NUMBER_OF_NOTES_MAX 65

extern uint16_t otj [NUMBER_OF_BUZZERS][NUMBER_OF_NOTES_MAX][2];    // otj = OdeToJoy






#define TIME_MULTIPLIER 200
#define NOTE_CUTTOFF 25

uint32_t noteEndMillis[NUMBER_OF_BUZZERS];
int noteCurrentEntry[NUMBER_OF_BUZZERS];


void setup() {
#ifdef ARDUINO_ESP8266_GENERIC
  Wire.begin(2, 0); // ESP-01 - SDA GPIO2, SCL GPIO0
#else
  Wire.begin();
#endif

  Serial.begin(115200);

  delay(100);

  sw.begin(Wire, sw.find(true));
  buzz4.begin(4);
  buzz5.begin(5);
  buzz6.begin(6);
  buzz7.begin(7);
  
  {
    int i;
    uint32_t starttime = millis();
    for (i = 0; i < NUMBER_OF_BUZZERS; ++i)
    {
      noteEndMillis[i] = starttime + otj[i][0][1] * TIME_MULTIPLIER;
      noteCurrentEntry[i] = 0;
      sw.writePublicData(i+ 4, otj[i][0][0]); // Start first note
    }
  }
 
}

void loop() {

  for (int i = 0; i < NUMBER_OF_BUZZERS; ++i)
  {
    if (millis() > noteEndMillis[i]    && otj[i][noteCurrentEntry[i]][1] != 0)
    {
 
      ++noteCurrentEntry[i];
      sw.writePublicData(i+4,otj[i][noteCurrentEntry[i]][0]);
      noteEndMillis[i] += otj[i][noteCurrentEntry[i]][1] * TIME_MULTIPLIER;
     

    }
    else if (millis()  + NOTE_CUTTOFF > noteEndMillis[i]  && otj[i][noteCurrentEntry[i]][1] != 0)
    {
      sw.writePublicData(i+4,0);
    }
  }
  /*
  {
    if (millis() > note1StartMillis + otj_1[note1CurrentEntry][1]* TIME_MULTIPLIER   && otj_1[note1CurrentEntry][1] != 0)
    {
     
      ++note1CurrentEntry;
      buzz5.writePublicData(otj_1[note1CurrentEntry][0]);
      note1StartMillis = millis();

    }
    else if (millis() > note1StartMillis + otj_1[note1CurrentEntry][1]* TIME_MULTIPLIER  - NOTE_CUTTOFF  && otj_1[note1CurrentEntry][1] != 0)
    {
      buzz5.writePublicData(0);
    }
  }

  {
    if (millis() > note2StartMillis + otj_2[note2CurrentEntry][1]* TIME_MULTIPLIER   && otj_2[note2CurrentEntry][1] != 0)
    {
      char s[50];
      sprintf(s, "%u %u %u", note2CurrentEntry, otj_2[note2CurrentEntry][0], otj_2[note2CurrentEntry][1]);
      Serial.println(s);
      ++note2CurrentEntry;
      buzz6.writePublicData(otj_2[note2CurrentEntry][0]);
      note2StartMillis = millis();

    }
    else if (millis() > note2StartMillis + otj_2[note2CurrentEntry][1]* TIME_MULTIPLIER  - NOTE_CUTTOFF  && otj_2[note2CurrentEntry][1] != 0)
    {
      buzz6.writePublicData(0);
    }
  }

   {
    if (millis() > note3StartMillis + otj_3[note3CurrentEntry][1]* TIME_MULTIPLIER   && otj_3[note3CurrentEntry][1] != 0)
    {
      char s[50];
      sprintf(s, "%u %u %u", note3CurrentEntry, otj_3[note3CurrentEntry][0], otj_2[note3CurrentEntry][1]);
      Serial.println(s);
      ++note3CurrentEntry;
      buzz7.writePublicData(otj_3[note3CurrentEntry][0]);
      note3StartMillis = millis();

    }
    else if (millis() > note3StartMillis + otj_3[note3CurrentEntry][1]* TIME_MULTIPLIER  - NOTE_CUTTOFF  && otj_3[note3CurrentEntry][1] != 0)
    {
      buzz7.writePublicData(0);
    }
  }
  */
}
