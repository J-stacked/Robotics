//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Original Code by Joey Hoffmann ////////////////////////////////////////////////
////////////////////////////////////////// Modified by:                   ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////  Wiring diagram:  ////////////////////////////////////////////////////////////
////////////  left sensor:  /////////////////////////////////////////////////  right sensor:  ////////////////////////////
////////////  trig: 9       //////////  Tx(Pin 1): S1 on controller  ////////  trig: 11       ////////////////////////////
////////////  echo: 10      /////////////////////////////////////////////////  echo: 12       ////////////////////////////
//////////////////////////////////////////  5V split devices:  ///////////////////////////////////////////////////////////
//////////////////////////////////////////  L/R sensors        ///////////////////////////////////////////////////////////
//////////////////////////////////////////  motor driver       ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////  DIP Switches:  ////////////////////////////////////////////////   5S LiPo 60C   ////////////////////////////
////////////  1: ON          ////////////////////////////////////////////////   18.4V         ////////////////////////////
////////////  2: OFF         ////////////////////////////////////////////////   4000mAh       ////////////////////////////
////////////  3: ON          /////////////////////////////////////////////////////////////////////////////////////////////
////////////  4: OFF         /////////////////////////////////////////////////////////////////////////////////////////////
////////////  5: ON          /////////////////////////////////////////////////////////////////////////////////////////////
////////////  6: ON          /////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////  Physical specifications  //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////  Maximum load on springed walls = 12.9 lb * 2 per side = 25.8 lbs per front/back wall                 ///////
////////////  Torque per motor = 120 oz*in = 7.5 lb*in = 7.5 lb*in * 4 motors = 30 lb*in per robot                 ///////
////////////      = 24 lbs of force (30lb*in/1.25in (radius))                                                      ///////
////////////  RPM of motor = 530 no load RPM = 7.85398 in wheel circumference * 530 rpm = 4162.61 in/min top speed ///////
////////////      = 69.3768 in/sec top speed = 5.78 ft/sec top speed                                               ///////
////////////  Above specs will vary due to voltage, amps, and the regenerative properties of motor controller      ///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <SabertoothSimplified.h>

SabertoothSimplified ST;  //library as object ST

int stop = 0;  //all stop cmd

/////////////////////////////////////Motor Stuff////////////////////////////////////////////////////////////////////////
int dir = 1; //direction of movement, 0 is forward, 1 is back
int currentL = 0;  //int at which speed is set for left motor
int currentR = 0;  //int at which speed is set for right motor
/////////////////////////// Always use these functions to start and stop robot to avoid motor damage ///////////////////
void accelerate(char side, int len, int maxspd);  //side to accelerate, time over which acceleration takes place, speed to accelerate to
void accelerateBoth(int len, int maxspd);
void decelerate(char side, int len);  //side to slow down, length of time to slow down over
int senseL();
int senseR();
int turning = 0;  //increase for wider radius when searching
int pulseWidthR = 0;  //the output variable for the right sensor                                                                                                                                                                                                                                              int pulseWidthR = 0;  //the output variable for the right sensor
int pulseWidthL = 0;  //the output variable for the left sensor
///////////////////////////////////Ultrasonic Sensors///////////////////////////////////////////////////////////////////
#define sensorTrigPinR    11    ///Define pins
#define sensorEchoPinR    12
#define sensorTrigPinL    10
#define sensorEchoPinL    9
//////////////////////////////////setup stuff///////////////////////////////////////////////////////////////////////////
void setup() {
   // put your setup code here, to run once:
   pinMode(sensorTrigPinR, OUTPUT);  //set the different pins as input/output
   pinMode(sensorEchoPinR, INPUT);
   pinMode(sensorTrigPinL, OUTPUT);
   pinMode(sensorEchoPinL, INPUT);
   Serial.begin(9600);  //don't actually use this unless you unplug the pin from pin 1, as it could lead to weird results
   SabertoothTXPinSerial.begin(9600);  //and this is why, because it also uses pin 1 to output motor commands
   ST.motor(1, stop);  //halts motor 1 just to make sure
   ST.motor(2, stop);  //halts motor 2
   pulseWidthR = senseR();  //does a bit of math after reading in through the "ear" of the sensor to convert to cm (roughly)
   pulseWidthL = senseL();  //do the math to find the range
   delay(4800);  //delay five seconds.  Took into account the actual power on delay and code running beforehand.
   //accelerateBoth(6, 128);  //immediately accelerates motors.  Left stays on pretty much the entire time
   //delay(50);  // run both motors for a second, just in case robot is in front, or to get out of the way if otherwise
}
//////////////////////////main program/////////////////////////////////////////////////////////
void loop() {
  // put your main code here, to run repeatedly:
  
  /*Serial.print(" Pulse WidthR: ");  //test sensors
  Serial.print(pulseWidthR);
  Serial.print(" Pulse WidthL: ");
  Serial.print(pulseWidthL);*/

  delay(9);  //delay 9ms to avoid rapid sound pulses and acceleration.  
  if(pulseWidthL < 80  or pulseWidthR < 80){  //it sees something through one of the sensors
    if(dir == 0){  //if turning already
      turning = 0;  //variable to control a search routine (basic, just go in slightly bigger circles after more time has passed after seeing something)
      dir = 1;  //set direction to forward
      accelerateBoth(1, 128);  //accelerate left, though it should be full speed already
    }
  }else{   //the robot doesn't see anything
    if(turning > 2){  //if going straight
      if(dir == 1){
        dir = 0;  //sets direction to turning
        decelerate('L', 1);  //slow down so the robot doesn't spin into the 6th dimension
        decelerate('R', 1);  //slows down right motor
        accelerate('L', 2, 80);
      }
      ST.motor(2, map(turning, 0, 500, 0, 127));  //maps the values from turning (0-1000) to values allowable to the motor controller (-127 - 127, but in this case 0-127).  decrease 1000 if you want it to make bigger circles faster.  
    }
    turning+=1;  //increments the turn radius
  }   

  pulseWidthR = senseR();  //does a bit of math after reading in through the "ear" of the sensor to convert to cm (roughly)
  pulseWidthL = senseL();  //do the math to find the range

  //test dem motors
  /*accelerate('R', 20);
  delay(20000);
  decelerate('R', 20);
  accelerate('L', 20);
  delay(20000);
  decelerate('L', 20);*/
  
}

void accelerate(char side, int len, int maxspd){  //function for accelerating motors.  Self explanatory code.
  if(side=='L'){  //left side
     for(int i = currentL; i < maxspd; i +=1){  //go from current motor speed to max speed given in function args
       ST.motor(1, i);  //motor 1 is the left side, set speed to i
       currentL = i;  //set current speed for left motor to i
       delay(len/4);  //delay the length given in function args
     }
   }
   if(side=='R'){  //right side
     for(int i = currentR; i < maxspd; i +=1){  //go from current motor speed to max speed given in function args
       ST.motor(2, i);  //motor 2 is the right side, set speed to i
       currentR = i;  //set current speed for right motor to i
       delay(len/4);  //delay the length given in function args
     }
   }
}

void accelerateBoth(int len, int maxspd){  //function for accelerating motors.  Self explanatory code.
   for(int i = currentL; i < maxspd; i +=1){  //go from current motor speed to max speed given in function args
     ST.motor(1, i);  //motor 1 is the left side, set speed to i
     currentL = i;  //set current speed for left motor to i
     ST.motor(2, i);  //motor 2 is the right side, set speed to i
     currentR = i;  //set current speed for right motor to i
     delay(len/4);  //delay the length given in function args
   }
}

void decelerate(char side, int len){  //function for slowing down motors
  if(side=='L'){  //left side
     for(int i = currentL; i >= 0; i -=1){  //loop from current position to resting position
       ST.motor(1, i);  //1 = left  move 1 to i   i eventually = 0
       currentL = i;  //set the variable recording current left motor speed to i
       delay(len);   //delay the length as provided in args
     }
   }
   if(side=='R'){
     for(int i = currentR; i >= 0; i -=1){
       ST.motor(2, i); //2 = left  move 2 to i   i eventually = 0
       currentR = i;  //set the variable recording current right motor speed to i
       delay(len); //delay the length as provided in args
     }
   }
}

int senseL(){
  digitalWrite(sensorTrigPinL, HIGH); //sends out sound pulse through left sensor
  delayMicroseconds(8);  //8ms delay
  digitalWrite(sensorTrigPinL, LOW);  //stop sending sound pulse
  return (pulseIn(sensorEchoPinL, HIGH) * 0.034/2);
}

int senseR(){
  digitalWrite(sensorTrigPinR, HIGH);  //sends out a sound pulse
  delayMicroseconds(8);  //through experimentation, 8ms gets the furthest and most reliable read
  digitalWrite(sensorTrigPinR, LOW);  //stops sending out the sound pulse
  return (pulseIn(sensorEchoPinR, HIGH) * 0.034/2);
}
