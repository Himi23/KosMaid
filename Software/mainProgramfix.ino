//US Sensor Inpput//
#define trigPin 19
#define echoPin 18
unsigned long startTime = 0;
bool functionRunning = false;
bool normalMotion;
float distance, duration;
//motion input//
int speedpotentioPin = A0, steeringpotentioPin = A1, speedPinA = 9, speedPinB = 10, directionPin1A = 4, directionPin2A = 5, directionPin1B = 6, directionPin2B = 7;

unsigned long currentTime;
//data to process//
int powerButtonState, prevpowerButtonState, toggleSwitch;
bool left1 = true, gridchange = true, left2 = true, turnMemory = 1; //turn around left//

//motion data//
float shaftDiameter = 20.0; //in cm//
float wheelDiameter = 6.5; //in cm
float motorRps = 232.0/60.0;
float chassisWidth = 18.0; // in cm//
float wheelCircumference = wheelDiameter * 3.1428;
float spinCircumference = shaftDiameter * 3.1428;
float speedResultant; //in cms//
float perpendicularTurnInterval;
float gridInterval; 

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
void gridChangeLeft() {
  unsigned long currentTime = millis();
  normalMotion = false;
  int speedpotentioValue = analogRead(speedpotentioPin);
  int speedRate = map(speedpotentioValue, 0,1023,0,255);
  speedResultant = (speedRate/255.0) * motorRps * wheelCircumference;

  //Steering potentio module//
  int steeringpotentioValue = analogRead(steeringpotentioPin);
  int steerRate = map(steeringpotentioValue, 0, 1023,0,255);
  int steerRateReverseCalibration = map(steerRate,129,255,128,0);
  float perpendicularTurnInterval = (spinCircumference/4.0) / speedResultant;
  float gridInterval = chassisWidth/speedResultant;

  if (functionRunning == false & left1 == true) {
    // indicate that the function is running //
    startTime = currentTime;
    functionRunning = true;
    Serial.println("  Function1 started...");
    stop (directionPin1A, directionPin2A);
    stop (directionPin1B, directionPin2B);
    delay (500);
  }

  // code to execute function for certain duration //
  if (currentTime - startTime <= perpendicularTurnInterval * 1100.0 + 500 & left1 == true) {
    // 90 degree left function //
    Serial.println("Turning Left 1...");
    forward (directionPin1A, directionPin2A);
    backward(directionPin1B, directionPin2B);
    analogWrite(speedPinA, speedRate);
    analogWrite(speedPinB, speedRate);

  } else {
    left1 = false;
    // Recycling the function //
    if (functionRunning == true & gridchange == true ) {
      // Set the start time and indicate that the function is running //
      startTime = currentTime;
      functionRunning = false;
      Serial.println("  Function2 started...");
      stop (directionPin1A, directionPin2A);
      stop (directionPin1B, directionPin2B);
      delay (500);
    }
    if (currentTime - startTime < gridInterval * 1280.0 + 500 & gridchange == true){
      //grid change code for certain duration //
      Serial.println("Changing grid...");
      forward (directionPin1B, speedRate);
      forward (directionPin1A, speedRate);     
    } else {
      gridchange = false;
      if (functionRunning == false & left2 == true) {
      //indicate that the function is running  //
        startTime = currentTime;
        functionRunning = true;
        Serial.println("  Function3 started...");
        stop (directionPin1A, directionPin2A);
        stop (directionPin1B, directionPin2B);
        delay (500);
      }

      if (currentTime - startTime < perpendicularTurnInterval * 1000.0 + 500  & left2 == true){
        Serial.println("Turning Left2");
        forward (directionPin1A, directionPin2A);
        backward (directionPin1B, directionPin2B);
        analogWrite(speedPinA, speedRate);
        analogWrite(speedPinB, speedRate);

      }else {
        // enabling mormal motion and disabling 
        Serial.println("done");
        left1 = true;
        gridchange = true;
        left2 = true;
        functionRunning = false;
        normalMotion = true;
        turnMemory = 0;
      }
    }
  }
}

void gridChangeRight() {
  unsigned long currentTime = millis();
  normalMotion = false;
  int speedpotentioValue = analogRead(speedpotentioPin);
  int speedRate = map(speedpotentioValue, 0,1023,0,255);
  speedResultant = (speedRate/255.0) * motorRps * wheelCircumference;
  //Steering potentio module//

  int steeringpotentioValue = analogRead(steeringpotentioPin);
  int steerRate = map(steeringpotentioValue, 0, 1023,0,255);
  int steerRateReverseCalibration = map(steerRate,129,255,128,0);
  float perpendicularTurnInterval = (spinCircumference/4.0) / speedResultant;
  float gridInterval = chassisWidth/speedResultant;

  if (functionRunning == false & left1 == true) {
    //indicate that the function is running //
    startTime = currentTime;
    functionRunning = true;
    Serial.println("  Function1 started...");
    stop (directionPin1A, directionPin2A);
    stop (directionPin1B, directionPin2B);
    delay (500);
  }

  if (currentTime - startTime <= perpendicularTurnInterval * 1200.0 + 500 & left1 == true) {
    // 90 degree right function //
    Serial.println("Turning Right 1");
    forward (directionPin1B, directionPin2B);
    backward(directionPin1A, directionPin2A);
    analogWrite(speedPinA, speedRate);
    analogWrite(speedPinB, speedRate);

  } else {
    left1 = false;
    // Recycling for next use //
    if (functionRunning == true & gridchange == true ) {
      // indicate that the function is running //
      startTime = currentTime;
      functionRunning = false;
      Serial.println("  Function2 started...");
      stop (directionPin1A, directionPin2A);
      stop (directionPin1B, directionPin2B);
      delay (500);
    }

    if (currentTime - startTime < gridInterval * 1350.0 + 500 & gridchange == true){
      Serial.println("Changing grid...");
      forward (directionPin1B, speedRate);
      forward (directionPin1A, speedRate);     
    } else {
      gridchange = false;
      if (functionRunning == false & left2 == true) {
      //indicate that the function is running
        startTime = currentTime;
        functionRunning = true;
        Serial.println("  Function3 started...");
        stop (directionPin1A, directionPin2A);
        stop (directionPin1B, directionPin2B);
        delay (500);
      }
      if (currentTime - startTime < perpendicularTurnInterval * 1000.0 + 500  & left2 == true){
        // grid change function //
        Serial.println("Turning Right 2");
        forward (directionPin1B, directionPin2B);
        backward (directionPin1A, directionPin2A);
        analogWrite(speedPinA, speedRate);
        analogWrite(speedPinB, speedRate);

      }else {
        // enabling normal motion and disable changing grid motion //
        Serial.println("done");
        left1 = true;
        gridchange = true;
        left2 = true;
        functionRunning = false;
        normalMotion = true;
        turnMemory = 1;
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(speedPinA, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  pinMode(directionPin1A, OUTPUT);
  pinMode(directionPin2A, OUTPUT);
  pinMode(directionPin1B, OUTPUT);
  pinMode(directionPin2B,OUTPUT);
  pinMode(speedpotentioPin, INPUT);
  pinMode(steeringpotentioPin, INPUT);
  normalMotion = true;
  stop (directionPin1A, directionPin2A);
  stop (directionPin1B, directionPin2B);
}

void loop() {
  if (normalMotion == true){
    //US Sensing//
    currentTime = millis();
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) * 0.0343;
    Serial.print(distance);
    Serial.print("  cm");
    Serial.print("   ");
    delay(25);
    //motion sensing//
    if (currentTime >= 0){
            //speed input//
      forward (directionPin1A, directionPin2A);
      forward (directionPin1B, directionPin2B);
      int speedpotentioValue = analogRead(speedpotentioPin);
      int speedRate = map(speedpotentioValue, 0,1023,0,255);
      speedResultant = (speedRate/255.0) * motorRps * wheelCircumference;
      //Steering potentio module//
      int steeringpotentioValue = analogRead(steeringpotentioPin);
      int steerRate = map(steeringpotentioValue, 0, 1023,0,255);
      int steerRateReverseCalibration = map(steerRate,129,255,128,0);
      float perpendicularTurnInterval = (spinCircumference/4.0) / speedResultant;
      float gridInterval = chassisWidth/speedResultant;

      Serial.print(speedRate);
      Serial.print("    ");
      Serial.print(steerRate);
      Serial.print("    ");
      Serial.print(speedResultant);
      Serial.print("    ");
      Serial.print(spinCircumference);
      Serial.print("    ");
      Serial.print(wheelCircumference);
      Serial.print("    ");
      Serial.print(perpendicularTurnInterval);
      Serial.print("    ");
      Serial.println(gridInterval);

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
          Serial.println("ngeeeng");
      }
    }
    
  }
  if ( distance <= 23 && turnMemory == 1) {
      gridChangeLeft();
  } else if (distance <= 23 && turnMemory == 0){
    gridChangeRight();
  }
}