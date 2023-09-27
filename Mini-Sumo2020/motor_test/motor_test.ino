#include <SabertoothSimplified.h>

SabertoothSimplified ST;

//all stop cmd
int stop = 0;
int currentL = 0;
int currentR = 0;
void accelerate(char side, int len, int maxspd);  //always use this and decelerate to avoid sudden stops which would harm the gears
void decelerate(char side, int len);

void setup()
{
  Serial.begin(9600);  //don't actually use this unless you unplug the pin from pin 1, as it could lead to weird results
   SabertoothTXPinSerial.begin(9600);  //and this is why, because it also uses pin 1 to output motor commands
   ST.motor(1, stop);  //halts motor 1 just to make sure
   ST.motor(2, stop);  //halts motor 2
   accelerate('L', 6, 128);  //immediately accelerates motors.  Left stays on the entire time pretty much
   accelerate('R', 6, 128); 
}

void loop()
{
  //stop
  accelerate('R',10, 128);
  delay(2000);
  decelerate('R', 10);
  accelerate('L', 10, 128);
  delay(2000);
  decelerate('L', 10);

}

void accelerate(char side, int len, int maxspd){  //function for accelerating motors.
  if(side=='L'){
     for(int i = currentL; i < maxspd; i +=1){
       ST.motor(1, i);
       currentL = i;
       delay(len);
     }
   }
   if(side=='R'){
     for(int i = currentR; i < maxspd; i +=1){
       ST.motor(2, i);
       currentR = i;
       delay(len);
     }
   }
}

void decelerate(char side, int len){  //function for slowing down motors
  if(side=='L'){
     for(int i = currentL; i >= 0; i -=1){
       ST.motor(1, i);
       currentL = i;
       delay(len);
     }
   }
   if(side=='R'){
     for(int i = currentR; i >= 0; i -=1){
       ST.motor(2, i);
       currentR = i;
       delay(len);
     }
   }
}
