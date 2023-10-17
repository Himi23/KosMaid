int trigPin = 19;
int echoPin = 18;
int p0 = A0;
int p1 = A1;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, OUTPUT);
  pinMode(p0, INPUT);
  pinMode(p1, INPUT);
  Serial.begin(9600);
  
}

void loop() {

  digitalWrite(trigPin, LOW);
  delay(100);
  digitalWrite(trigPin, HIGH);
  delay(100);
  digitalWrite(echoPin, LOW);
  delay(100);
  digitalWrite(echoPin, HIGH);
  delay(100);

  int p0value = analogRead(A0);
  int p1value = analogRead(A1);

  Serial.println(p0value);
  Serial.println(p1value);

}
