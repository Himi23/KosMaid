int speedpotentioPin = A0;
int steeringpotentioPin = A1;
int speedPinA = 9;
int speedPinB = 10;
int directionPin1A = 4;
int directionPin2A = 5;
int directionPin1B = 6;
int directionPin2B = 7;

void forward(int dirpin1, int dirpin2){
  digitalWrite(dirpin1, HIGH);
  digitalWrite(dirpin2,LOW);
}
void backward(int dirpin1, int dirpin2){
  digitalWrite(dirpin1, LOW);
  digitalWrite(dirpin2,HIGH);
}
void stop (int dirpin1, int dirpin2){
  digitalWrite(dirpin1, LOW);
  digitalWrite(dirpin2, LOW);
}
void brake (int dirpin1, int dirpin2){
  digitalWrite(dirpin1,HIGH);
  digitalWrite(dirpin2, HIGH);
}
void setup() {
  pinMode(speedPinA, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  pinMode(directionPin1A, OUTPUT);
  pinMode(directionPin2A, OUTPUT);
  pinMode(directionPin1B, OUTPUT);
  pinMode(directionPin2B,OUTPUT);
  pinMode(speedpotentioPin, INPUT);
  pinMode(steeringpotentioPin, INPUT);
  Serial.begin(9600);

}

void loop() {
  //speed function//
  int speedpotentioValue = analogRead(speedpotentioPin);
  int speedRate = map(speedpotentioValue, 0,1023,0,255);
  //end of speed function//
  
  //Steering potentio module//
  int steeringpotentioValue = analogRead(steeringpotentioPin);
  int steerRate = map(steeringpotentioValue, 0, 1023,0,255);
  int steerRateReverseCalibration = map(steerRate,129,255,128,0);
  Serial.print(speedRate);
  Serial.print("    ");
  Serial.println(steerRate);
  
  if (steerRate == 128) {
    analogWrite(speedPinA, speedRate);
    analogWrite(speedPinB, speedRate);
  }
  if (steerRate < 128) {
    int turningRate = (steerRate*speedRate)/128;
  	analogWrite(speedPinA, speedRate);
    analogWrite(speedPinB, turningRate);
  }
  if (steerRate > 128) {
  	int turningRate = (steerRateReverseCalibration*speedRate)/128;
    analogWrite(speedPinB, speedRate);
    analogWrite(speedPinA, turningRate);
  }
}
