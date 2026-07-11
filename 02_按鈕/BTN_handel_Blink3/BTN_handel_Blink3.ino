//==============================
// 計算機五大單元 + 暫存器設定註解
//==============================

//【記憶單元】宣告 LED 與按鈕腳位 (變數存放在 SRAM 中)
const int ledPin1 = 3;   // 對應 ATmega2560 的 PD3 腳位
const int ledPin2 = 4;   // 對應 ATmega2560 的 PD4 腳位
const int ledPin3 = 5;   // 對應 ATmega2560 的 PD5 腳位
const int ButtonPin = 8; // 對應 ATmega2560 的 PB0 腳位

//【記憶單元】變數用於儲存按鈕狀態
int buttonState = 0;

//【控制單元】setup() 初始化設定
void setup() {
  //【輸出單元】設定 LED 腳位為輸出
  // pinMode(3, OUTPUT) → DDRD3 = 1
  // pinMode(4, OUTPUT) → DDRD4 = 1
  // pinMode(5, OUTPUT) → DDRD5 = 1
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  //【輸入單元】設定按鈕腳位為輸入並啟用內部上拉電阻
  // pinMode(8, INPUT_PULLUP) → DDRB0 = 0 (輸入模式)，PORTB0 = 1 (啟用上拉電阻)
  pinMode(ButtonPin, INPUT_PULLUP);
}

//【控制單元】loop() 主迴圈
void loop() {
  //【輸入單元】讀取按鈕狀態
  // digitalRead(8) → 讀取 PINB0 暫存器的值
  buttonState = digitalRead(ButtonPin);

  //【控制單元】判斷按鈕是否被按下 (LOW = 按下)
  if (buttonState == LOW) 
  {
    //【輸出單元】LED 全亮
    // digitalWrite(3, HIGH) → PORTD3 = 1
    // digitalWrite(4, HIGH) → PORTD4 = 1
    // digitalWrite(5, HIGH) → PORTD5 = 1
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);

    //【控制單元】延遲 1 秒 (透過 Timer 暫存器計算)
    delay(1000);

    //【輸出單元】LED 全滅
    // digitalWrite(3, LOW) → PORTD3 = 0
    // digitalWrite(4, LOW) → PORTD4 = 0
    // digitalWrite(5, LOW) → PORTD5 = 0
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);

    //【控制單元】延遲 1 秒
    delay(1000);     
  }
}
