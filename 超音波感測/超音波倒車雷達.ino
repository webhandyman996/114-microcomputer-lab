/*
 * ╔══════════════════════════════════════════════════════════════╗
 * ║      超音波倒車雷達模擬                                       ║
 * ║      平台：Arduino Mega 2560（ATmega2560）                   ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  功能說明：                                                   ║
 * ║    模擬汽車倒車雷達。                                         ║
 * ║    距離越近 → 嗶聲間隔越短；近到臨界距離 → 持續長鳴。          ║
 * ║    LED 亮度隨距離拉近而增強（PWM 類比輸出）。                  ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  【核心概念：map() 函式 — 線性比例換算】                      ║
 * ║    map(value, fromLow, fromHigh, toLow, toHigh)              ║
 * ║    將 value 從 [fromLow~fromHigh] 線性映射到 [toLow~toHigh]  ║
 * ║    本程式用來將「距離」換算為「嗶聲間隔」與「LED亮度」         ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  【五大單元對應】                                             ║
 * ║  輸入單元：HC-SR04 ECHO（D11）                               ║
 * ║  輸出單元：蜂鳴器（D8）tone()、LED（D6）analogWrite() PWM    ║
 * ║  記憶單元：SRAM 儲存 dist, interval, brightness 變數         ║
 * ║  ALU     ：map() 線性換算、條件判斷                           ║
 * ║  控制單元：依距離區間決定執行路徑（多層 if-else）              ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  接線：                                                       ║
 * ║    HC-SR04 VCC→5V, GND→GND, TRIG→D10, ECHO→D11             ║
 * ║    無源蜂鳴器 → D8                                           ║
 * ║    LED → D6（串 220Ω，D6 支援 PWM 輸出）                     ║
 * ╚══════════════════════════════════════════════════════════════╝
 */

// ── 腳位定義 ──────────────────────────────────────────────────────
const int TRIG_PIN   = 10;
const int ECHO_PIN   = 11;
const int BUZZER_PIN = 8;
const int LED_PIN    = 6;   // 必須是 PWM 腳（Mega: 2~13, 44~46）

// ── 距離參數 ──────────────────────────────────────────────────────
const int MAX_DIST  = 200;  // 超過此距離不動作（公分）
const int CONT_DIST = 10;   // 小於此距離：持續長鳴（公分）

// ── 嗶聲參數 ──────────────────────────────────────────────────────
const int MAX_INTERVAL  = 1000; // 最遠時的嗶聲間隔（毫秒）
const int MIN_INTERVAL  = 80;   // 最近時的嗶聲間隔（毫秒）
const int BEEP_DURATION = 60;   // 每次嗶聲持續時間（毫秒）
const int BEEP_FREQ     = 2000; // 嗶聲頻率（Hz）
const int CONT_FREQ     = 3000; // 持續長鳴頻率（Hz，音調更高表示危險）


// ── getDistance()：超音波距離量測 ─────────────────────────────────
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 逾時 30ms（約 500cm）
  if (duration == 0) return -1;                   // 逾時：無障礙物
  return duration * 0.034 / 2;
}

void setup() {
  pinMode(TRIG_PIN,   OUTPUT);
  pinMode(ECHO_PIN,   INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN,    OUTPUT);
  Serial.begin(9600);
  Serial.println("倒車雷達啟動，請靠近感測器...");
}

void loop() {
  long dist = getDistance();

  // ── 距離無效或超出範圍：靜音 + LED 熄滅 ──────────────────────
  if (dist <= 0 || dist > MAX_DIST) {
    noTone(BUZZER_PIN);
    analogWrite(LED_PIN, 0);   // PWM 輸出 0 → LED 熄滅
    Serial.println("距離: -- (範圍外)");
    delay(200);
    return;  // 控制單元：跳回 loop() 起點
  }

  // ── LED 亮度：距離越近越亮（analogWrite PWM）─────────────────
  // map() 將距離 [CONT_DIST ~ MAX_DIST] 反向映射到亮度 [255 ~ 0]
  // 距離最遠(MAX_DIST) → 亮度 0（熄滅）
  // 距離最近(CONT_DIST)→ 亮度 255（全亮）
  //
  // analogWrite() 底層原理：
  //   設定 Timer4 的 OCR4A 比較暫存器，產生 PWM 方波
  //   duty cycle = brightness/255（0%~100%）
  //   人眼感知到 LED 亮度隨 duty cycle 變化
  int brightness = map(dist, CONT_DIST, MAX_DIST, 255, 0);
  brightness = constrain(brightness, 0, 255); // 限制在合法範圍
  analogWrite(LED_PIN, brightness);

  // ── 距離區間判斷與嗶聲輸出 ────────────────────────────────────
  if (dist <= CONT_DIST) {
    // 區間①：極近（≤ 10cm）→ 持續長鳴，高頻警示
    tone(BUZZER_PIN, CONT_FREQ);
    Serial.print("距離: ");
    Serial.print(dist);
    Serial.println(" cm  ⚠ 持續長鳴！");
    delay(50);

  } else {
    // 區間②：一般範圍（10~200cm）→ 間歇嗶聲，間隔隨距離縮短
    //
    // map() 線性換算範例：
    //   dist=200cm → interval = map(200, 10, 200, 80, 1000) = 1000ms（慢）
    //   dist=100cm → interval = map(100, 10, 200, 80, 1000) ≈  540ms（中）
    //   dist= 20cm → interval = map( 20, 10, 200, 80, 1000) ≈  133ms（快）
    //   dist= 10cm → interval = map( 10, 10, 200, 80, 1000) =   80ms（最快）
    int interval = map(dist, CONT_DIST, MAX_DIST, MIN_INTERVAL, MAX_INTERVAL);

    Serial.print("距離: ");
    Serial.print(dist);
    Serial.print(" cm  間隔: ");
    Serial.print(interval);
    Serial.println(" ms");

    // tone(腳位, 頻率, 持續時間)：Timer2 產生 PWM，BEEP_DURATION 後自動停止
    // 非阻塞：tone() 呼叫後立即返回，Timer2 背景繼續輸出
    tone(BUZZER_PIN, BEEP_FREQ, BEEP_DURATION);

    // delay(interval)：等待整個嗶聲週期
    // 嗶聲 = 前 BEEP_DURATION ms 有聲，後 (interval-BEEP_DURATION) ms 靜音
    delay(interval);
  }
}
