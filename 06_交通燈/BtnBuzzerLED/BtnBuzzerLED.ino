/*
  BtnBuzzerLED —— 交通燈單元的入門陽春版（按鈕+LED+蜂鳴器）

  跟 TraficLamp／TraficLamp2／TraficLamp3 的差異：
  - TraficLamp 系列：用非阻塞（non-blocking）狀態機寫法，紅黃綠燈自動循環，
    程式架構較複雜，適合已經熟悉 Arduino 的階段
  - BtnBuzzerLED（本檔）：用最直觀的 delay() 阻塞式寫法，按住按鈕時
    紅黃燈交替閃爍＋蜂鳴器響，放開按鈕時綠燈閃爍＋蜂鳴器靜音，
    邏輯單純，適合當交通燈單元「正式版」之前的暖身練習

  教學重點：
  - 先讓學生用簡單的 if/else + delay() 做出「看起來像」紅綠燈的效果，
    建立成就感，之後再進到 TraficLamp 系列學習為什麼要改用非阻塞寫法
    （這裡用 delay() 會讓按鈕在閃爍延遲期間沒辦法即時反應，可以拿來
    當作引出「非阻塞控制」概念的活教材）

  （原始程式為課程資料整理保留，供交通燈單元銜接教學用）
*/

const int greenLED = 2;
const int redLED = 3;
const int yellowLED = 4;
const int buzzer = 5;
const int button = 6;

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(button, INPUT_PULLUP); // 使用內部上拉電阻
}

void loop() {
  if (digitalRead(button) == LOW) { // 按鈕按下
    // 紅色與黃色 LED 輪流閃爍
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    delay(200);
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    delay(200);

    // 綠色 LED 滅掉
    digitalWrite(greenLED, LOW);

    // 蜂鳴器使用 tone() 控制
    tone(buzzer, 1000);
  } else { // 按鈕放開
    // 紅色與黃色 LED 滅掉
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);

    // 綠色 LED 閃爍
    digitalWrite(greenLED, HIGH);
    delay(200);
    digitalWrite(greenLED, LOW);
    delay(200);

    // 蜂鳴器靜音
    noTone(buzzer);
  }
}
