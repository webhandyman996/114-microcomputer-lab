/*
  程式功能：
  此程式碼使用 Arduino Mega 2560 開發板，透過外部按鈕控制板載 LED 的亮滅。
  第一次按下按鈕時，LED 恆亮；第二次按下按鈕時，LED 恆滅。
  程式碼中加入了按鍵防抖動設計，以確保 LED 狀態的穩定切換。

  Program Function:
  This code uses the Arduino Mega 2560 development board to control the onboard LED
  by an external button. The LED will turn on when the button is pressed the first time,
  and turn off when the button is pressed the second time.
  Button debounce is implemented to ensure stable LED state switching.
*/

// 定義板載 LED 連接的腳位
// Define the pin connected to the onboard LED
const int ledPin = 3;

// 定義按鈕連接的腳位
// Define the pin connected to the button
const int buttonPin = 2;

// 變數用於儲存按鈕的狀態
// Variable to store the state of the button
int buttonState = 0;

// 變數用於儲存上一個按鈕的狀態
// Variable to store the previous state of the button
int lastButtonState = 1; // 初始化為 1，假設按鈕初始狀態為未按下
                           // Initialize to 1, assuming the button is not pressed initially

// 變數用於儲存 LED 的狀態
// Variable to store the state of the LED
bool ledOn = false; // 初始化為 false，表示 LED 初始狀態為熄滅
                      // Initialize to false, indicating the LED is off initially

void setup() {
  // 設定 LED 腳位為輸出模式
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);

  // 設定按鈕腳位為輸入模式，並啟用內部上拉電阻
  // Set the button pin as an input with internal pull-up resistor enabled
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // 讀取按鈕的狀態
  // Read the state of the button
  buttonState = digitalRead(buttonPin);

  // 比較當前按鈕狀態和上一個按鈕狀態
  // Compare the current button state with the previous button state
  if (buttonState != lastButtonState) {
    // 如果按鈕狀態發生變化
    // If the button state has changed
    if (buttonState == LOW) { // 如果按鈕被按下
      // If the button is pressed
      // 切換 LED 的狀態
      // Toggle the LED state
      ledOn = !ledOn;
      // 根據 LED 的狀態控制 LED 的亮滅
      // Control the LED on/off based on the LED state
      digitalWrite(ledPin, ledOn ? HIGH : LOW);
    }
    // 更新上一個按鈕的狀態
    // Update the previous button state
    lastButtonState = buttonState;
  }
  delay(50); // 加入延遲，減少按鍵抖動的影響
              // Add a delay to reduce button debounce effect
}