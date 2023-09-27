#define sensorTrigPinR    11
#define sensorEchoPinR    12
#define sensorTrigPinL    9
#define sensorEchoPinL    10

int turning = 0;
int turnint = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(sensorTrigPinR, OUTPUT);
    pinMode(sensorEchoPinR, INPUT);
    pinMode(sensorTrigPinL, OUTPUT);
    pinMode(sensorEchoPinL, INPUT);
}

void loop()
{
    int pulseWidthR = 0;
    int pulseWidthL = 0;
    

    digitalWrite(sensorTrigPinR, HIGH);
    delayMicroseconds(8);
    digitalWrite(sensorTrigPinR, LOW);
    pulseWidthR = pulseIn(sensorEchoPinR, HIGH)  * 0.034/2;
    
    digitalWrite(sensorTrigPinL, HIGH);
    delayMicroseconds(8);
    digitalWrite(sensorTrigPinL, LOW);
    pulseWidthL = pulseIn(sensorEchoPinL, HIGH)  * 0.034/2;

    //Serial.println(constrain(map(turning, 0, 400, 0, 127), 0, 127));
    if(pulseWidthR < 90){
      Serial.print(" Pulse WidthR: ");
      Serial.println(pulseWidthR);
    }if(pulseWidthL < 90){
      Serial.print(" Pulse WidthL: ");
      Serial.println(pulseWidthL);
    }
    delay(9);
}
