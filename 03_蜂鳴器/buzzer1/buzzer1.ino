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

const int buzzerPin = 8;


// 設定函式，於重啟或開機時執行一次
// The setup function runs once when the board is powered on or reset
void setup() {  
  pinMode(buzzerPin, OUTPUT);
  
}

// 主迴圈函式，持續執行
// The loop function runs continuously
void loop() {
  frequencySweep();
}
// 控制蜂鳴器頻率變化的副程式
// Subroutine to control the buzzer's frequency variation
void frequencySweep() {
  
    // If the input is LOW (buzzer is sounding)
    for (int i = 500; i <= 1500; i += 10) { // 從 500Hz 到 1500Hz 掃描頻率
      // Sweep the frequency from 500Hz to 1500Hz
      tone(buzzerPin, i);
      delay(5); // 延遲 5 毫秒
    }
  
}
void MimiSound(){
  tone(buzzerPin, 1000); // 發出 1000Hz 聲音
  delay(1000); // 延遲 1 秒
  noTone(buzzerPin); // 停止聲音
  delay(1000); // 延遲 1 秒
}
