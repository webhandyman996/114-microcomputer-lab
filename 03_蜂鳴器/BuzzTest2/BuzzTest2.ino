/*
  程式功能：
  此程式碼使用 Arduino Mega 2560 開發板，透過數位腳位 D2 的狀態控制板載 LED 和蜂鳴器。
  當 D2 為 LOW 時，LED 亮起且蜂鳴器發聲（頻率可變）；當 D2 為 HIGH 時，LED 熄滅且蜂鳴器靜音。

  Program Function:
  This code uses the Arduino Mega 2560 development board to control the onboard LED and buzzer
  by the state of digital pin D2. When D2 is LOW, the LED turns on and the buzzer sounds (variable frequency);
  when D2 is HIGH, the LED turns off and the buzzer is silent.
*/

// 定義板載 LED 連接的腳位
// Define the pin connected to the onboard LED
const int ledPin = 4;

// 定義蜂鳴器連接的腳位
// Define the pin connected to the buzzer
const int buzzerPin = 8;

// 定義輸入連接的腳位
// Define the pin connected to the input
const int inputPin = 3;

// 變數用於儲存輸入的狀態
// Variable to store the state of the input
int inputState = 1;

void setup() {
  // 設定 LED 腳位為輸出模式
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);

  // 設定蜂鳴器腳位為輸出模式
  // Set the buzzer pin as an output
  pinMode(buzzerPin, OUTPUT);

  // 設定輸入腳位為輸入模式，並啟用內部上拉電阻
  // Set the input pin as an input with internal pull-up resistor enabled
  pinMode(inputPin, INPUT_PULLUP);
}

void loop() {
  // 呼叫 TonControl 副程式，控制 LED 和蜂鳴器
  // Call the TonControl subroutine to control the LED and buzzer
  TonControl();
  // 呼叫 frequencySweep 副程式，控制蜂鳴器的頻率變化
  // Call the frequencySweep subroutine to control the buzzer's frequency variation
  //frequencySweep();
  //BuzzWithLED();
}

// 控制 LED 和蜂鳴器亮滅的副程式
// Subroutine to control the LED and buzzer on/off
void TonControl() {
  // 讀取輸入的狀態
  // Read the state of the input
  inputState = digitalRead(inputPin);

  // 根據輸入的狀態控制 LED 和蜂鳴器
  // Control the LED and buzzer based on the input state
  if (inputState == LOW) { // 如果輸入為 LOW
    // If the input is LOW
    digitalWrite(ledPin, HIGH);   // 點亮 LED
    tone(buzzerPin, 1000); // 蜂鳴器發聲，頻率為 1000Hz
  } else { // 如果輸入為 HIGH
    // If the input is HIGH
    digitalWrite(ledPin, LOW);    // 熄滅 LED
    noTone(buzzerPin); // 蜂鳴器靜音
  }
}

// 控制 LED 和蜂鳴器同時亮滅的副程式
// Subroutine to control the LED and buzzer on/off simultaneously
void BuzzWithLED() {
  // 讀取輸入的狀態
  // Read the state of the input
  inputState = digitalRead(inputPin);

  // 根據輸入的狀態控制 LED 和蜂鳴器
  // Control the LED and buzzer based on the input state
  if (inputState == LOW) { // 如果輸入為 LOW
    // If the input is LOW
    digitalWrite(ledPin, HIGH);   // 點亮 LED
    digitalWrite(buzzerPin, HIGH); // 蜂鳴器發聲
  } else { // 如果輸入為 HIGH
    // If the input is HIGH
    digitalWrite(ledPin, LOW);    // 熄滅 LED
    digitalWrite(buzzerPin, LOW); // 蜂鳴器靜音
  }
}

// 控制蜂鳴器頻率變化的副程式
// Subroutine to control the buzzer's frequency variation
void frequencySweep() {
  if (inputState == LOW) { // 如果輸入為 LOW (蜂鳴器發聲)
    // If the input is LOW (buzzer is sounding)
    for (int i = 500; i <= 1500; i += 10) { // 從 500Hz 到 1500Hz 掃描頻率
      // Sweep the frequency from 500Hz to 1500Hz
      tone(buzzerPin, i);
      delay(5); // 延遲 5 毫秒
    }
  }
}