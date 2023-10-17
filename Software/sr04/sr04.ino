int trigPin = 19;
int echoPin = 18;
float pingTravelTime;
float distance;

void setup() {
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(9600);
}

void loop() {
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
  distance = (pingTravelTime/2) * 0.0343;
  Serial.print(distance);
  Serial.println("  cm");
  delay(200);

}
