/*
  WaveLED (波浪式 LED 閃爍)

  此程式依序點亮 LED，形成類似波浪的視覺效果。

  大多數 Arduino 板子都有內建 LED 可供控制。在 UNO、MEGA 和 ZERO 上，內建 LED 連接到數位腳位 13，
  而在 MKR1000 上則連接到腳位 6。使用 LED_BUILTIN 可確保程式適用於所有板子，無需修改腳位設定。

  如果想了解你的 Arduino 板子上的 LED 連接腳位，請參閱技術規格：
  https://docs.arduino.cc/hardware/

  修改日期：
  - 2014 年 5 月 8 日，Scott Fitzgerald
  - 2016 年 9 月 2 日，Arturo Guadalupi
  - 2016 年 9 月 8 日，Colby Newman

  此範例程式碼屬於公有領域 (public domain)。

  相關說明：
  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/

const int ledPin1 = 3;
const int ledPin2 = 4;
const int ledPin3 = 5;

const int ButtonPin = 8;

// 儲存按鈕的狀態
// Variable to store the state of the button
int buttonState = 0;

// 設定函式，於重啟或開機時執行一次
// The setup function runs once when the board is powered on or reset
void setup() {
  pinMode(ledPin1, OUTPUT);  // 設定 LED 腳位為輸出
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP);  // 設定按鈕為輸入並啟用內建上拉電阻
}

// 主迴圈函式，持續執行
// The loop function runs continuously
void loop() {
  WaveLED();  // 呼叫波浪式 LED 閃爍函式
}

// 波浪式 LED 閃爍函式
// Function for wave-like LED blinking effect
void WaveLED() {
   ChangeAllLedStatus(LOW);  // 關閉所有 LED
   digitalWrite(ledPin1, HIGH);  // 亮起第一個 LED
   delay(1000);                  
   digitalWrite(ledPin2, HIGH);  // 亮起第二個 LED
   delay(1000);                  
   digitalWrite(ledPin3, HIGH);  // 亮起第三個 LED
   delay(1000);                  
   digitalWrite(ledPin3, LOW);   // 熄滅第三個 LED
   delay(1000);     
   digitalWrite(ledPin2, LOW);   // 熄滅第二個 LED
   delay(1000);     
   digitalWrite(ledPin1, LOW);   // 熄滅第一個 LED
   delay(1000);     
}

// 設定所有 LED 狀態
// Set all LEDs to the given status (HIGH or LOW)
void ChangeAllLedStatus(int status) {
    digitalWrite(ledPin1, status);
    digitalWrite(ledPin2, status);
    digitalWrite(ledPin3, status);
}

// 簡單 LED 閃爍函式
// Basic blink function
void Blink() {
    ChangeAllLedStatus(HIGH);
    delay(1000);
    ChangeAllLedStatus(LOW);
    delay(1000);
}

// 根據按鈕狀態控制 LED 閃爍
// Blink LEDs based on button status
void ButtonStatusWithBlink() {
  buttonState = digitalRead(ButtonPin);
  if (buttonState == HIGH) {
    ChangeAllLedStatus(HIGH);  // 開啟所有 LED
    delay(1000);               
    ChangeAllLedStatus(LOW);   // 關閉所有 LED
    delay(1000);     
  }                 
}
