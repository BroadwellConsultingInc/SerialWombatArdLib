/*  This example shows how to use the Serial Wombat IR Receive (IRRx) pin mode.  It assumes an NEC compatible IR Transmitter
 *   (most inexpensive Arduino kit remotes are compatible) and a 38kHz receiver module that goes low when a modulated IR signal is
 *   present.
 *   
 *   This example is compatible with the Serial Wombat 18AB and 8B chips.
 *   
  *   
 *   In this example commands are received from the pin mode and printed to Serial, 
 *   
 *   By default the pin mode provides a command byte as its 16 bit public data value.  However, it can be reconfigured
 *   so that the 16 bit public data value increments each time a command (or a repeat code) is received.  A pulse on change
 *   pin mode can then read this value to chirp a buzzer or blink an LED when IR commands are received.  Note that this is
 *   more sophisticated than the visible LED on some IR Receivers.  The Serial Wombat Pulse on Change pin will only pulse
 *   when data is successfully received and decoded.  An LED on an IR Receivers blinks to indicate presense of an IR signal,
 *   not successful receiption of an IR packet.
 *   
 *   
 *   Video on IRRx pin mode:
 *   
 *   TODO coming soon
 * 
 *   SerialWombatIRRx pin mode documentation:
 *   
 *   TODO
 */
 
 #include <SerialWombat.h>

SerialWombatChip sw;
SerialWombatIRRx irrx(sw);
SerialWombatPulseOnChange poc(sw);

#define IRRX_PIN 7
#define PULSE_PIN 6


void setup() {
#ifdef ARDUINO_ESP8266_GENERIC
  Wire.begin(2, 0); // ESP-01 - SDA GPIO2, SCL GPIO0
#else
  Wire.begin();
#endif

  Serial.begin(115200);

  delay(500);

  sw.begin(Wire,sw.find(true));
 // sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}


  irrx.begin(IRRX_PIN,
              0,//Mode 0 : NEC
              true, // Use repeat
              SW_LOW,// Active Low
              1000, // 1000 ms Public Data Timeout
              0xFFFF, // Default public data
              false,   // Use Address Filtering
              0x0000, // Transmitter Address (not used)
              SerialWombatIRRx::publicDataOutput::DATACOUNT); // Make data count the output
              
//irrx.enablePullup(true);   //Comment in this line if your receiver is open drain type without pullup 


poc.begin  ( PULSE_PIN, SW_HIGH,SW_LOW, 100, 200);// Implied values active high, inactive low, 100mS pulse time, 200 mS pulse off, OR entries, No PWM
poc.setEntryOnChange(0, // first entry (valid values are 0-7)
IRRX_PIN);  // The data source we're monitoring for change
delay(1000);
}

void loop() {
  int receivedData = irrx.read();

  if (receivedData >=0)
  {
    Serial.print(irrx.readPublicData());
    Serial.print(":");
     Serial.println(receivedData);
  }
  
}
