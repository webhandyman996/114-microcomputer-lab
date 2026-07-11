const int ledPin1 = 3;
const int ledPin2 = 4;
const int ledPin3 = 5;
const int buzzerPin = 6;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  WaveLED();
}

void WaveLED() {
  ChangeAllLedStatus(LOW);

  digitalWrite(ledPin1, HIGH);
  tone(buzzerPin, 262);  // D0 = 262 Hz (Do)
  delay(1000);
  noTone(buzzerPin);

  digitalWrite(ledPin2, HIGH);
  tone(buzzerPin, 294);  // Ra = 294 Hz (Re)
  delay(1000);
  noTone(buzzerPin);

  digitalWrite(ledPin3, HIGH);
  tone(buzzerPin, 330);  // Mi = 330 Hz
  delay(1000);
  noTone(buzzerPin);

  digitalWrite(ledPin3, LOW);
  delay(1000);

  digitalWrite(ledPin2, LOW);
  delay(1000);

  digitalWrite(ledPin1, LOW);
  delay(1000);
}

void ChangeAllLedStatus(int status) {
  digitalWrite(ledPin1, status);
  digitalWrite(ledPin2, status);
  digitalWrite(ledPin3, status);
}
