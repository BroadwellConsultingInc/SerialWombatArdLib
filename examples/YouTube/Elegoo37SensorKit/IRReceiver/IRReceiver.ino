/* 
 */
 
 #include <SerialWombat.h>

 SerialWombatChip swc;
 

 SerialWombatPWM_18AB emitter(swc);
 
 void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  swc.begin(Wire,0x6b);

  
  Serial.begin(115200);
  emitter.begin(10,0);
  emitter.writeFrequency_Hz(37900);
  emitter.writeDutyCycle(0x8000);

}

void loop() {
  // put your main code here, to run repeatedly:
 
  Serial.println(swc.readPublicData(0));

}
