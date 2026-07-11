/*
  程式功能：
  此程式碼使用 Arduino Mega 2560 開發板，透過外部按鈕控制板載 LED 的亮滅。
  當按鈕被按下時，LED 恆亮；當按鈕被放開時，LED 恆滅。
  程式碼中加入了按鍵防抖動設計，以確保 LED 狀態的穩定切換。

  Program Function:
  This code uses the Arduino Mega 2560 development board to control the onboard LED
  by an external button. The LED will turn on when the button is pressed,
  and turn off when the button is released.
  Button debounce is implemented to ensure stable LED state switching.
*/

// 定義板載 LED 連接的腳位
// Define the pin connected to the onboard LED
const int ledPin = 13;

// 定義按鈕連接的腳位
// Define the pin connected to the button
const int buttonPin = 2;

// 變數用於儲存按鈕的狀態
// Variable to store the state of the button
int buttonState = 0;

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

  // 判斷按鈕是否被按下
  // Check if the button is pressed
  if (buttonState == LOW) { // 如果使用 INPUT_PULLUP，按下時為 LOW
    // If using INPUT_PULLUP, pressed state is LOW
    // 如果按鈕被按下，點亮 LED
    // If the button is pressed, turn on the LED
    digitalWrite(ledPin, HIGH);
  } else {
    // 如果按鈕未被按下，熄滅 LED
    // If the button is not pressed, turn off the LED
    digitalWrite(ledPin, LOW);
  }
}
