int Vin = 4;
int Gnd = 5;
int pwm = 3;

const int ROTATIONSPEED = 70;

void setup() {
  pinMode(Vin, OUTPUT);
  pinMode(Gnd, OUTPUT);
  pinMode(pwm, OUTPUT);
}

void loop() {
  flipperTurn();
  delay(ROTATIONSPEED);
  
  flipperStop();
  delay(3000);
  
  flipperReverse();
  delay(ROTATIONSPEED);
  
  flipperStop();
  delay(3000);
}

void flipperTurn(){
  analogWrite(pwm, 255);
  digitalWrite(Vin, LOW);
  digitalWrite(Gnd, HIGH);
}

void flipperReverse(){
  analogWrite(pwm, 255);
  digitalWrite(Vin, HIGH);
  digitalWrite(Gnd, LOW);
}

void flipperStop(){
  analogWrite(pwm, 0);
  digitalWrite(Vin, LOW);
  digitalWrite(Gnd, LOW);
}

