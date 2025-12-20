// Note: this uses the SimpleQueue Library by msilveus
#include <SimpleQueue.h>

#define QUEUETEST_MAX_QUEUE_SIZE 513
uint32_t ardQData [QUEUETEST_MAX_QUEUE_SIZE];
uint8_t queuetempData [QUEUETEST_MAX_QUEUE_SIZE];
    uint8_t swqTempData[QUEUETEST_MAX_QUEUE_SIZE];

#define QUEUE_TEST_NUMBER_ITERATIONS 1000
void queueTest(SerialWombatChip& sw)
{

  int queueSizeMax = QUEUETEST_MAX_QUEUE_SIZE;
  int queueSizeMin = 4;
  if (&sw == &SW8B_68)
  {
    queueSizeMax = 80;
  }
  for (int queueSize = queueSizeMin; queueSize < queueSizeMax; ++queueSize)
  {
   
     char s[80];
    SerialWombatQueue swq(sw);
    SimpleQueue q(ardQData,1,queueSize);


    uint32_t lfsrSeed = queueSize;
    uint16_t queueOffset = wrandom(&lfsrSeed);
    queueOffset &= 0x3FE; // Up to 1022, even numbers
    if (&sw == &SW8B_68)
    {
         queueOffset &= 0xE; // Up to 14
    }
    swq.begin(queueOffset, queueSize) ;
    swq.setTimeout(0);
    for (int iteration = 0; iteration < QUEUE_TEST_NUMBER_ITERATIONS; ++iteration)
    {
      uint16_t action = wrandom(&lfsrSeed);
      switch (action >> 30)
      {
        case 0:  //Add to queue
          {
            int count = action % queueSize;

            int aqSuccess = 0;
            int swqSuccess = 0;
            for (int x = 0; x < count; ++x)
            {
              queuetempData[x] = (uint8_t) wrandom(&lfsrSeed);
              
              if (q.push(&queuetempData[x]))
              {
                /*
                Serial.print("s: ");
                Serial.print(x);
                Serial.print(" c: ");
                Serial.println(q.count());
                */
                aqSuccess++;
                
              }
              else
              {
                /*
                                Serial.print("f: ");
                Serial.print(x);
                Serial.print(" c: ");
                Serial.println(q.count());
                */

              }
            }

            swqSuccess = swq.write((const uint8_t*) queuetempData, count);

           
            sprintf(s, "Q write: it: %d, count: %d, aq: %d, sw: %d", iteration, count,  aqSuccess, swqSuccess);
            test(s, swqSuccess, aqSuccess);
             if (swqSuccess == aqSuccess)
            {
              
            }
            else
            {
              iteration = QUEUE_TEST_NUMBER_ITERATIONS;
            }

          }
          break;
        case 1: //Remove from queue
            {
            int count = action % queueSize;

            int aqSuccess = 0;
            int swqSuccess = 0;
            for (int x = 0; x < count; ++x)
            {
             
              if (q.pop(&queuetempData[aqSuccess]))
              {
                aqSuccess++;
              }
            }
        
            swqSuccess = swq.readBytes((char*) swqTempData, aqSuccess);

            
            sprintf(s, "Q read: it: %d, count: %d, aq: %d, sw: %d", iteration, count, swqSuccess, aqSuccess);
            test(s, swqSuccess, aqSuccess);
            if (swqSuccess == aqSuccess)
            {
              for (int x = 0; x < aqSuccess; ++x)
              {
                   sprintf(s, "Q read: it: %d, count: %d, aq: %d, sw: %d, x: %d", iteration, count, swqSuccess, aqSuccess,x);
                   test(s, swqTempData[x], queuetempData[x]);
              }
            }
            else
            {
              iteration = QUEUE_TEST_NUMBER_ITERATIONS;
            }

          }
          break;


        case 2:  // Check filled bytes
          {
              sprintf(s, "Q count: it: %d ", iteration);
                   test(s, swq.available(), q.count());
          }
          break;
        case 3: // Check peek
          {
             sprintf(s, "Q peek: it: %d ", iteration);
             uint8_t value = *(uint8_t*)q.peek(0);
             test(s,swq.peek(),value);

          }
          break;

      }

    }
  }

}
