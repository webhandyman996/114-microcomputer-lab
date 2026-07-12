/*
  BuzzTest3 —— 蜂鳴器測試進階版（不同接腳配置 + 多副程式對照）

  跟 BuzzTest1、BuzzTest2 的差異：
  - BuzzTest2：ledPin=4、buzzerPin 未特別標註、inputPin=3、預設 inputState=1
  - BuzzTest3（本檔）：改用 Arduino Mega 2560 的接法，ledPin=13（板載LED）、
    buzzerPin=8、inputPin=2、預設 inputState=0。同一個「按鈕控制LED+蜂鳴器」
    的邏輯，換了一組接腳，讓學生練習看接線圖改程式，而不是死背腳位數字。

  教學重點：
  - 同時提供 TonControl()（用 tone() 產生固定音）跟 BuzzWithLED()
    （用 digitalWrite() 直接開關蜂鳴器，沒有音調變化）兩種寫法對照，
    體會 tone() 跟 digitalWrite() 驅動蜂鳴器的差別
  - frequencySweep() 示範頻率掃描效果，寫在 loop() 裡但預設用註解關閉，
    可以打開讓學生實驗

  （原始程式為課程資料整理保留，供不同接線版本對照教學用）
*/

const int ledPin = 13;
const int buzzerPin = 8;
const int inputPin = 2;

int inputState = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(inputPin, INPUT_PULLUP);
}

void loop() {
  TonControl();
  // frequencySweep(); // 想看頻率掃描效果時取消這行註解
}

// 用 tone() 依輸入狀態控制 LED 跟蜂鳴器（固定 1000Hz）
void TonControl() {
  inputState = digitalRead(inputPin);
  if (inputState == LOW) {
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000);
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
  }
}

// 對照組：直接用 digitalWrite() 開關蜂鳴器，沒有音調（可跟 TonControl 比較差異）
void BuzzWithLED() {
  inputState = digitalRead(inputPin);
  if (inputState == LOW) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
}

// 頻率掃描副程式：輸入為 LOW 時從 500Hz 掃到 1500Hz
void frequencySweep() {
  if (inputState == LOW) {
    for (int i = 500; i <= 1500; i += 10) {
      tone(buzzerPin, i);
      delay(5);
    }
  }
}
