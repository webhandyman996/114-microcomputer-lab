/*
 * ╔══════════════════════════════════════════════════════════════╗
 * ║      超音波防盜警報系統                                       ║
 * ║      平台：Arduino Mega 2560（ATmega2560）                   ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  功能說明：                                                   ║
 * ║    持續以超音波掃描，偵測到有人進入範圍時觸發警報              ║
 * ║    （LED 亮 + 蜂鳴器持續鳴叫）。                             ║
 * ║    按下解除按鈕後，停止警報並恢復監控狀態。                    ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  【五大單元對應】                                             ║
 * ║  輸入單元：ECHO（D11）偵測距離、按鈕（D2）讀取輸入            ║
 * ║            PINx 暫存器讀取電位；INPUT_PULLUP 使用內建上拉電阻 ║
 * ║  輸出單元：LED（D6）視覺警示、蜂鳴器（D8）聲音警示            ║
 * ║            PORTx 暫存器 + Timer2 PWM 輸出                    ║
 * ║  記憶單元：SRAM 儲存 alarmOn 狀態旗標                        ║
 * ║  ALU     ：距離比較、digitalRead() 判斷按鈕電位               ║
 * ║  控制單元：狀態機（監控模式 ↔ 警報模式）流程跳轉             ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  接線：                                                       ║
 * ║    HC-SR04 VCC→5V, GND→GND, TRIG→D10, ECHO→D11             ║
 * ║    LED      → D6（串 220Ω）                                   ║
 * ║    無源蜂鳴器 → D8                                           ║
 * ║    按鈕一端 → D2，另一端 → GND（使用 INPUT_PULLUP）           ║
 * ╚══════════════════════════════════════════════════════════════╝
 */

// ── 腳位定義 ──────────────────────────────────────────────────────
const int TRIG_PIN   = 10;
const int ECHO_PIN   = 11;
const int LED_PIN    = 6;    // 輸出單元：警示 LED
const int BUZZER_PIN = 8;    // 輸出單元：無源蜂鳴器
const int BTN_PIN    = 2;    // 輸入單元：解除警報按鈕

// ── 參數設定 ──────────────────────────────────────────────────────
const int DETECT_DIST  = 50;   // 入侵偵測距離門檻（公分）
const int BUZZ_FREQ    = 1000; // 蜂鳴器頻率（Hz）
const int DEBOUNCE_MS  = 50;   // 按鈕去彈跳時間（毫秒）

// ── 狀態機旗標（儲存於 SRAM）────────────────────────────────────
// 控制單元依此旗標決定執行監控迴圈或警報迴圈
bool alarmOn = false;


// ── 距離量測 ──────────────────────────────────────────────────────
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

/*
 * triggerAlarm()
 * 設定警報旗標，啟動警報狀態
 * 控制單元下一輪迴圈將進入警報執行區段
 */
void triggerAlarm() {
  alarmOn = true;
  Serial.println("[警報] 偵測到入侵！");
}

/*
 * stopAlarm()
 * 清除警報旗標，關閉所有輸出，回到監控模式
 * PORTx 暫存器寫 0 → LED 熄滅
 * Timer2 停止 PWM → 蜂鳴器靜音
 */
void stopAlarm() {
  alarmOn = false;
  digitalWrite(LED_PIN, LOW);   // PORTx：LED 腳輸出 LOW
  noTone(BUZZER_PIN);           // Timer2：停止 PWM 輸出
  Serial.println("[解除] 警報已解除，恢復監控");
}

/*
 * checkButton()
 * 偵測解除按鈕是否被按下
 *
 * INPUT_PULLUP 原理：
 *   ATmega2560 內部有上拉電阻（約 20-50kΩ）連接 VCC
 *   未按下：腳位被拉至 HIGH（5V）
 *   按下後：腳位透過按鈕接 GND → 讀到 LOW（0V）
 *   不需要外接電阻，電路更簡潔
 *
 * 去彈跳（Debounce）原理：
 *   機械按鈕接觸瞬間會產生短暫電位抖動（彈跳）
 *   延遲 50ms 再次確認，確保是真實按下而非雜訊
 */
void checkButton() {
  if (digitalRead(BTN_PIN) == LOW) {       // PINx：讀到 LOW = 按下
    delay(DEBOUNCE_MS);                    // 等待彈跳消除
    if (digitalRead(BTN_PIN) == LOW && alarmOn) {
      stopAlarm();
      while (digitalRead(BTN_PIN) == LOW); // 等待按鈕放開再繼續
    }
  }
}

void setup() {
  pinMode(TRIG_PIN,   OUTPUT);
  pinMode(ECHO_PIN,   INPUT);
  pinMode(LED_PIN,    OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  // INPUT_PULLUP：DDRx bit=0（輸入），同時啟用內部上拉電阻
  pinMode(BTN_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("[啟動] 防盜系統監控中...");
}

void loop() {
  // ── 按鈕檢查（任何狀態下都優先檢查）───────────────────────────
  // 控制單元優先執行，確保按鈕能即時回應
  checkButton();

  if (!alarmOn) {
    // ── 監控模式：超音波掃描 ──────────────────────────────────────
    long dist = getDistance();

    Serial.print("[監控] 距離: ");
    Serial.print(dist);
    Serial.println(" cm");

    // ALU 判斷：有效距離 且 小於門檻 → 觸發警報（狀態機切換）
    if (dist > 0 && dist < DETECT_DIST) {
      triggerAlarm();  // alarmOn = true，下一輪進入警報區段
    }

    delay(300);  // 監控模式每 300ms 掃描一次

  } else {
    // ── 警報模式：LED 閃爍 + 蜂鳴器持續鳴叫 ─────────────────────
    // 輸出單元：PORTx 寫 1 → LED 亮
    digitalWrite(LED_PIN, HIGH);
    // 輸出單元：Timer2 PWM → 蜂鳴器發聲
    tone(BUZZER_PIN, BUZZ_FREQ);
    delay(200);

    // 閃爍效果：短暫熄滅 LED（蜂鳴器持續）
    digitalWrite(LED_PIN, LOW);
    delay(100);
    // 注意：此處不呼叫 noTone()，蜂鳴器保持鳴叫
  }
}
