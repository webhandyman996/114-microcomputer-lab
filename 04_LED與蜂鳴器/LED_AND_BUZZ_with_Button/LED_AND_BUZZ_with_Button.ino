const int ledPin1 = 3;
const int ledPin2 = 4;
const int ledPin3 = 5;
const int buzzerPin = 6;
const int buttonPin = 7;

bool isRunning = false;
bool lastButtonState = HIGH;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  // 偵測按鈕從 HIGH → LOW（按下）
  if (lastButtonState == HIGH && buttonState == LOW) {
    isRunning = !isRunning;  // 切換狀態
    if (!isRunning) {
      ChangeAllLedStatus(LOW);
      noTone(buzzerPin);
    }
    delay(300);  // 去彈跳延遲
  }

  lastButtonState = buttonState;

  if (isRunning) {
    WaveLED();
  }
}

void WaveLED() {
  ChangeAllLedStatus(LOW);

  digitalWrite(ledPin1, HIGH);
  tone(buzzerPin, 262);  // Do
  delay(500);
  noTone(buzzerPin);

  digitalWrite(ledPin2, HIGH);
  tone(buzzerPin, 294);  // Re
  delay(500);
  noTone(buzzerPin);

  digitalWrite(ledPin3, HIGH);
  tone(buzzerPin, 330);  // Mi
  delay(500);
  noTone(buzzerPin);

  digitalWrite(ledPin3, LOW);
  delay(300);
  digitalWrite(ledPin2, LOW);
  delay(300);
  digitalWrite(ledPin1, LOW);
  delay(300);
}

void ChangeAllLedStatus(int status) {
  digitalWrite(ledPin1, status);
  digitalWrite(ledPin2, status);
  digitalWrite(ledPin3, status);
}
