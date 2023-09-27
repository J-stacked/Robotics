#include <DualVNH5019MotorShield.h>  

DualVNH5019MotorShield md;

void setup() {
  // put your setup code here, to run once:
  md.init();
  delay(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(true){
    md.setM1Speed(-400);
    md.setM2Speed(-400);
    Serial.println("Loop2");
  }
  Serial.println("Loop");
}
