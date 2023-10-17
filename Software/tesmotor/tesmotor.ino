#define enA 9
#define enB 10
#define in1 4
#define in2 5
#define in3 6
#define in4 7

int speed = 255;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

}

void loop() {

  analogWrite(enA, speed);
  analogWrite(enB, speed);


}
