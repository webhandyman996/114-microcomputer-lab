const int Greenled = 2;
const int Yellowled = 3;
const int Redled = 4;

void setup() {
  
  pinMode(Greenled, OUTPUT);
  pinMode(Yellowled, OUTPUT);
  pinMode(Redled, OUTPUT);
}

void loop() {
  digitalWrite(Greenled, HIGH);  
  digitalWrite(Yellowled, LOW);  
  digitalWrite(Redled, LOW);   
  delay(3000);                      // wait for 3 second
  digitalWrite(Greenled, LOW);  
  digitalWrite(Yellowled, HIGH);  
  digitalWrite(Redled, LOW);     
  delay(1000);                      // wait for a second
  digitalWrite(Greenled, LOW);  
  digitalWrite(Yellowled, LOW);  
  digitalWrite(Redled, HIGH);   
  delay(3000);                      // wait for 3 second
}
