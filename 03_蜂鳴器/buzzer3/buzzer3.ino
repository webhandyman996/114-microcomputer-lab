/*
  buzzer3 —— 蜂鳴器單元進階版（3 LED + 按鈕 + 頻率掃描）

  跟 buzzer1、buzzer2 的差異：
  - buzzer1／buzzer2：只有蜂鳴器本身的基本發聲練習
  - buzzer3（本檔）：多接了 3 顆 LED（腳位 3、4、5）跟 1 個按鈕（腳位 8，
    使用 INPUT_PULLUP 內建上拉電阻），蜂鳴器改到腳位 9，並示範
    frequencySweep() 用 for 迴圈讓音調從 500Hz 掃到 1500Hz 的漸變效果，
    以及 MimiSound() 這個固定音高、嗶一聲的簡化版寫法可以互相對照。

  教學重點：
  - 讓學生看到「同一顆蜂鳴器」可以用固定音（MimiSound）或掃頻（frequencySweep）
    兩種方式驅動，體會 tone() 的第二參數（頻率）怎麼影響聲音
  - 銜接後面「LED與蜂鳴器」「交通燈」單元的多元件整合寫法

  （原始程式來自 Arduino 官方 Blink 範例修改，經課程資料整理保留）
*/

const int ledPin1 = 3;
const int ledPin2 = 4;
const int ledPin3 = 5;

const int ButtonPin = 8;
const int buzzerPin = 9;

// 儲存按鈕的狀態
int buttonState = 0;

void setup() {
  pinMode(ledPin1, OUTPUT);  // 設定 LED 腳位為輸出
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP);  // 設定按鈕為輸入並啟用內建上拉電阻
}

void loop() {
  frequencySweep();
}

// 控制蜂鳴器頻率變化的副程式：從 500Hz 掃到 1500Hz
void frequencySweep() {
  for (int i = 500; i <= 1500; i += 10) {
    tone(buzzerPin, i);
    delay(5);
  }
}

// 固定音高嗶一聲的簡化版寫法，可跟 frequencySweep() 對照教學
void MimiSound() {
  tone(buzzerPin, 1000);
  delay(1000);
  noTone(buzzerPin);
  delay(1000);
}
