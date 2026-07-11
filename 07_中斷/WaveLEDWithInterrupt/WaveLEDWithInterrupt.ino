// 腳位定義 / Pin assignments
const int ledPin1 = 3;         // 第1顆 LED 接腳位3 / LED 1 connected to pin 3
const int ledPin2 = 4;         // 第2顆 LED 接腳位4 / LED 2 connected to pin 4
const int ledPin3 = 5;         // 第3顆 LED 接腳位5 / LED 3 connected to pin 5
const int buzzerPin = 6;       // 蜂鳴器接腳位6 / Buzzer connected to pin 6
const int buttonPin = 7;       // 按鈕接腳位7 / Button connected to pin 7

// 狀態變數 / State variables
bool isRunning = false;        // 是否執行 LED 效果 / Is the LED effect running
bool lastButtonState = HIGH;   // 上一輪的按鈕狀態 / Last state of the button

void setup() {
  // 設定腳位模式 / Set pin modes
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // 啟用內建上拉電阻 / Enable internal pull-up
}

void loop() {
  bool buttonState = digitalRead(buttonPin);  // 讀取按鈕狀態 / Read button state

  // 偵測按下事件 / Detect falling edge (button press)
  if (lastButtonState == HIGH && buttonState == LOW) {
    isRunning = !isRunning;  // 切換執行狀態 / Toggle running state

    if (!isRunning) {
      ChangeAllLedStatus(LOW);  // 關閉 LED / Turn off all LEDs
      noTone(buzzerPin);        // 停止蜂鳴器 / Stop buzzer
    }

    delay(300);  // 去彈跳 / Debounce delay
  }
  lastButtonState = buttonState;  // 更新狀態 / Update last button state

  if (isRunning) {
    WaveLEDWithInterrupt();  // 執行 LED 效果 / Run wave LED effect
  }
}

// LED 波浪效果（可中途中斷）/ Wave LED effect (interruptible)
void WaveLEDWithInterrupt() {
  if (InterruptibleStep(ledPin1, 262)) return; // Do 音 / Do note
  if (InterruptibleStep(ledPin2, 294)) return; // Re 音 / Re note
  if (InterruptibleStep(ledPin3, 330)) return; // Mi 音 / Mi note

  // 熄燈階段 / Turning LEDs off one by one
  for (int i = 0; i < 3; i++) {
    if (digitalRead(buttonPin) == LOW) {   // 若按下則中斷 / Interrupt if button pressed
      isRunning = false;
      ChangeAllLedStatus(LOW);
      noTone(buzzerPin);
      delay(300);
      return;
    }
    digitalWrite(ledPin3 - i, LOW);  // 依序熄滅 / Turn off LEDs in reverse
    delay(300);
  }
}

// 可中斷的單步驟亮燈與音階 / Interruptible LED + tone step
bool InterruptibleStep(int ledPin, int freq) {
  if (!isRunning) return true;  // 若已停止，跳出 / Exit if already stopped

  digitalWrite(ledPin, HIGH);   // 亮燈 / Turn on LED
  tone(buzzerPin, freq);        // 播音階 / Play tone

  unsigned long startTime = millis();
  while (millis() - startTime < 500) {
    if (digitalRead(buttonPin) == LOW) {  // 中途按下 / Check for button press
      isRunning = false;
      ChangeAllLedStatus(LOW);
      noTone(buzzerPin);
      delay(300);
      return true;
    }
  }

  noTone(buzzerPin);  // 停止音階 / Stop tone
  return false;       // 未中斷 / Step completed normally
}

// 改變全部 LED 狀態 / Change all LED states at once
void ChangeAllLedStatus(int status) {
  digitalWrite(ledPin1, status);
  digitalWrite(ledPin2, status);
  digitalWrite(ledPin3, status);
}
