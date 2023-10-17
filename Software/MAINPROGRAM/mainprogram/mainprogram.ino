int powerButton = 13;
int speedpotentioPin = A0;
int steeringpotentioPin = A1;
int speedPinA = 9;
int speedPinB = 10;
int directionPin1A = 4;
int directionPin2A = 5;
int directionPin1B = 6;
int directionPin2B = 7;
int trigPin = 19;
int echoPin = 18;

unsigned long currentTime;
int speedRate;
int speedRateCalibration;
int steerRate;
int steerRateReverseCalibration;
int turningRate;
int turningDegree;
int powerButtonState;
int prevpowerButtonState;
int toggleSwitch;
int turnMemory; //1 is time to turn left, 0 is time to turn right. to achieve zigzag movement(left-right-left-right)//

float pingTravelTime;
float distance;
float speedofSound = 343;



float shaftDiameter = 17.0; //in cm//
float wheelDiameter = 6.5; //in cm
float motorRps = 232.0/60.0;
float chassisWidth = 12.5; // in cm//
float wheelCircumference = (wheelDiameter/2.0) * (wheelDiameter/2.0) * 3.1428;
float spincircumference = (shaftDiameter/2.0) * (shaftDiameter/2.0) * 3.1428;
float speedResultant; //in cms//

float perpendicularTurnInterval = (spincircumference/4.0) / speedResultant;
float gridInterval = chassisWidth/speedResultant;  

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
  
void setup()
{
  pinMode(powerButton, INPUT);
  pinMode(speedpotentioPin, INPUT);
  pinMode(steeringpotentioPin, INPUT);
  pinMode(speedPinA, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  pinMode(directionPin1A, OUTPUT);
  pinMode(directionPin2A, OUTPUT);
  pinMode(directionPin1B, OUTPUT);
  pinMode(directionPin2B,OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  stop(directionPin1A, directionPin2A);
  stop(directionPin1B, directionPin2B);
  prevpowerButtonState = 1;
  toggleSwitch = 0;
  Serial.begin (9600);
  turnMemory = 1;
}

void loop(){
  currentTime = millis();
  
  //manual pulse generation by trigpin//
  digitalWrite(trigPin,LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  delayMicroseconds(10);
  //catching the times required for the ping traveled from trigpin through echopin using pulseIn//
  //pulseIn function measure the time while the echoPin is high
  pingTravelTime = pulseIn (echoPin,HIGH);
  //distance variable in centimeter//
  distance = (speedofSound * pingTravelTime) / (2.0*10000.0);

  
  //pushbutton as toggle switch controlling power of the car//
  powerButtonState = digitalRead (powerButton);
  //because of pull up resistor, powerButtonState = 0 when the button is pushed//
  //toggleswitch = 1 is on//
  if (toggleSwitch == 1 && powerButtonState == 0 && prevpowerButtonState ==1){
    toggleSwitch = 0;
    delay(300);
  }
  if (toggleSwitch == 0 && powerButtonState == 0 && prevpowerButtonState == 0 ){
    toggleSwitch = 1 ;
  }
  prevpowerButtonState = powerButtonState;
  //end toggle//
  
  //POWER function//
  if (toggleSwitch == 1) {
    forward(directionPin1A, directionPin2A);
    forward(directionPin1B, directionPin2B);
  }
  if (toggleSwitch == 0) {
  	stop(directionPin1A, directionPin2A);
    stop(directionPin1B, directionPin2B);
  }
  //end of direction function//
  
  //speed function//
  int speedpotentioValue = analogRead(speedpotentioPin);
  speedRate = map(speedpotentioValue, 0,1023,0,255);
  
  speedResultant = (speedRate/255) * motorRps * wheelCircumference;
  //end of speed function//
  
  //Steering potentio module//
  int steeringpotentioValue = analogRead(steeringpotentioPin);
  steerRate = map(steeringpotentioValue, 0, 1023,0,255);
  steerRateReverseCalibration = map(steerRate,129,255,128,0);
  
  if (steerRate == 128) {
    analogWrite(speedPinA, speedRate);
    analogWrite(speedPinB, speedRate);
  }
  if (steerRate < 128) {
    turningRate = (steerRate*speedRate)/128;
  	analogWrite(speedPinA, speedRate);
    analogWrite(speedPinB, turningRate);
  }
  if (steerRate > 128) {
  	turningRate = (steerRateReverseCalibration*speedRate)/128;
    analogWrite(speedPinB, speedRate);
    analogWrite(speedPinA, turningRate);
  }
  
  //to reach all of the area of the square surface, we then divide the area into grids that has the width area of the robot or the the width of the vacuum sucker. and the robot has to travel through all of the grid with zigzag movement//
  //zigzag movement//
  if (distance <= 3 && turnMemory == 1){
	//defining the interval needed to achieve 90 degree turn//
    stop(directionPin1A, directionPin2A);
    stop(directionPin1B, directionPin2B);
    delay(25);
    unsigned long functionStartTime = currentTime;
    //turn left 90 degree//
    while (currentTime - functionStartTime <= perpendicularTurnInterval * 1000){   
      forward (directionPin1B, directionPin2B);
      backward(directionPin1A, directionPin2A);
      analogWrite(speedPinA, speedRate);
      analogWrite(speedPinB, speedRate);
    }
    stop(directionPin1A,directionPin2A);
    stop(directionPin1B,directionPin2B);
    delay(25);
    //manual pulse generation by trigpin//
    digitalWrite(trigPin,LOW);
    delayMicroseconds(10);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    delayMicroseconds(10);
  
    pingTravelTime = pulseIn (echoPin,HIGH);
    int speedofSound = 343;
    distance = (speedofSound * pingTravelTime) / (2*10000);;
    
    //check if there is obstacle upfront that have distance of less than the width of the robot. 
    	//it means that the robot cannot go forward and has to immediately turn around
    if (distance <=3){
      unsigned long functionStartTime = currentTime;
      //turn left 90 degree//
      while (currentTime - functionStartTime <= perpendicularTurnInterval * 1000){   	
      	forward(directionPin1B, directionPin2B);
      	backward(directionPin1A, directionPin2A);
      	analogWrite(speedPinA, speedRate);
      	analogWrite(speedPinB, speedRate);
      }
    }    //if there are not any disruptive obstacle upfront, 
    	//move forward as far as the width of the robot, 
    	//then  90degree to the right(using while interval again//
    else {
      unsigned long functionStartTime = currentTime;
      //changing grid//
      while (currentTime - functionStartTime <= gridInterval * 1000){
      	forward(directionPin1A, directionPin2A);
        forward(directionPin1B, directionPin2B);
        analogWrite(speedPinA, speedRate);
        analogWrite(speedPinB, speedRate);
      }
      stop(directionPin1A,directionPin2A);
      stop(directionPin1B,directionPin2B);
      delay(25);
      //turn left 90 degree//
      while (currentTime - functionStartTime <= perpendicularTurnInterval * 1000){   
        forward (directionPin1B, directionPin2B);
        backward(directionPin1A, directionPin2A);
        analogWrite(speedPinA, speedRate);
        analogWrite(speedPinB, speedRate);
      }
    }
	stop(directionPin1A,directionPin2A);
    stop(directionPin1B,directionPin2B);
    delay(25);
    turnMemory = 0;
  }
  if (distance <=3 && turnMemory ==0) {
	//defining the interval needed to achieve 90 degree turn//
    stop(directionPin1A, directionPin2A);
    stop(directionPin1B, directionPin2B);
    delay(25);
    unsigned long functionStartTime = currentTime;
    //turn right 90 degree//
    while (currentTime - functionStartTime <= perpendicularTurnInterval * 1000){   
      forward (directionPin1A, directionPin2A);
      backward(directionPin1B, directionPin2B);
      analogWrite(speedPinA, speedRate);
      analogWrite(speedPinB, speedRate);
    }
    stop(directionPin1A,directionPin2A);
    stop(directionPin1B,directionPin2B);
    delay(25);
    //manual pulse generation by trigpin//
    digitalWrite(trigPin,LOW);
    delayMicroseconds(10);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    delayMicroseconds(10);
    
    pingTravelTime = pulseIn (echoPin,HIGH);
    //distance variable in centimeter//
    int speedofSound = 343;
    distance = (speedofSound * pingTravelTime) / (2*10000);
    //check if there is obstacle upfront that have distance of less than the width of the robot. 
    	//it means that the robot cannot go forward and has to immediately turn around
    if (distance <=3){
      unsigned long functionStartTime = currentTime;
      while (currentTime - functionStartTime <= perpendicularTurnInterval * 1000){   
      	//turn right 90 degree//
      	forward(directionPin1A, directionPin2A);
      	backward(directionPin1B, directionPin2B);
      	analogWrite(speedPinA, speedRate);
      	analogWrite(speedPinB, speedRate);
      }
    }    //if there are not any disruptive obstacle upfront, 
    	//move forward as far as the width of the robot, 
    	//then  90degree to the right(using while interval again//
    else {
      unsigned long functionStartTime = currentTime;
      while (currentTime - functionStartTime <= gridInterval * 1000){
      	forward(directionPin1A, directionPin2A);
        forward(directionPin1B, directionPin2B);
        analogWrite(speedPinA, speedRate);
        analogWrite(speedPinB, speedRate);
      }
      stop(directionPin1A,directionPin2A);
      stop(directionPin1B,directionPin2B);
      delay(25);
      //turn right 90 degree//
      while (currentTime - functionStartTime <= perpendicularTurnInterval * 1000){       
        forward (directionPin1A, directionPin2A);
        backward(directionPin1B, directionPin2B);
        analogWrite(speedPinA, speedRate);
        analogWrite(speedPinB, speedRate);
      }
    }
	stop(directionPin1A,directionPin2A);
    stop(directionPin1B,directionPin2B);
    delay(25);
    turnMemory = 1;
  }
  
}
