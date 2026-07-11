/*
 * ╔══════════════════════════════════════════════════════════════╗
 * ║      超音波偵測行人紅綠燈                                    ║
 * ║      平台：Arduino Mega 2560（ATmega2560）                   ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  功能說明：                                                   ║
 * ║    模擬行人號誌控制。綠燈倒數結束時，若超音波偵測到             ║
 * ║    仍有行人在感應範圍內，自動延長綠燈時間。                    ║
 * ║    最多延長 MAX_EXTEND 次，防止綠燈無限延伸。                 ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  【五大單元對應】                                             ║
 * ║  輸入單元：HC-SR04 ECHO（D11）偵測行人距離                     ║
 * ║  輸出單元：綠燈(D4)、黃燈(D5)、紅燈(D6)、TRIG(D10)             ║
 * ║  記憶單元：SRAM 儲存 extendCount、greenStart 等變數           ║
 * ║  ALU     ：millis() 時間比較、距離換算、次數計數                ║
 * ║  控制單元：while/if 判斷跳轉，millis() 非阻塞流程控制           ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  接線：                                                       ║
 * ║    HC-SR04 VCC→5V, GND→GND, TRIG→D10(SCL), ECHO→D11(SDA)     ║
 * ║    綠燈 LED → D4（串 220Ω）                                   ║
 * ║    黃燈 LED → D5（串 220Ω）                                   ║
 * ║    紅燈 LED → D6（串 220Ω）                                   ║
 * ╚══════════════════════════════════════════════════════════════╝
 */

// ── 腳位定義 ──────────────────────────────────────────────────────
const int TRIG_PIN   = 13;
const int ECHO_PIN   = 12;
const int LED_GREEN  = 4;   // 輸出單元：綠燈
const int LED_YELLOW = 5;   // 輸出單元：黃燈
const int LED_RED    = 6;   // 輸出單元：紅燈

// ── 時間與距離參數（儲存於 Flash ROM 常數區）────────────────────
const unsigned long GREEN_TIME  = 5000;  // 綠燈持續時間（毫秒）
const unsigned long YELLOW_TIME = 2000;  // 黃燈持續時間（毫秒）
const unsigned long RED_TIME    = 5000;  // 紅燈持續時間（毫秒）
const unsigned long EXTEND_TIME = 3000;  // 每次延長時間（毫秒）
const int MAX_EXTEND  = 2;    // 最多延長次數（防止無限延長）
const int DETECT_DIST = 150;  // 行人感應距離門檻（公分）


// ── 距離量測（與前一範例相同原理）────────────────────────────────
// 輸出 TRIG 脈衝 → 計時 ECHO 高電位 → ALU 換算距離
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
 * personDetected()
 * 呼叫 getDistance() 判斷是否有行人在感應範圍內
 * 回傳：true = 有人, false = 無人
 */
bool personDetected() {
  long dist = getDistance();
  return (dist > 0 && dist < DETECT_DIST);
}

/*
 * setLight(g, y, r)
 * 同時控制三色燈號輸出（PORTx 暫存器操作）
 * 參數：true=亮, false=滅
 */
void setLight(bool g, bool y, bool r) {
  digitalWrite(LED_GREEN,  g ? HIGH : LOW);
  digitalWrite(LED_YELLOW, y ? HIGH : LOW);
  digitalWrite(LED_RED,    r ? HIGH : LOW);
}

void setup() {
  pinMode(TRIG_PIN,   OUTPUT);
  pinMode(ECHO_PIN,   INPUT_PULLUP);
  pinMode(LED_GREEN,  OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED,    OUTPUT);
  Serial.begin(9600);
  Serial.println("行人號誌系統啟動");
}

void loop() {
  // ── 綠燈階段 ────────────────────────────────────────────────────
  setLight(true, false, false);
  Serial.println("[綠燈] 行人通行");

  // millis() 非阻塞等待：
  // millis() 回傳 CPU 上電後經過的毫秒數（Timer0 計數暫存器累計）
  // 比 delay() 優：等待期間仍可執行其他邏輯
  unsigned long greenStart = millis();
  while (millis() - greenStart < GREEN_TIME) {
    // 等待綠燈時間結束（控制單元反覆跳回此判斷）
    delay(50);
  }

  // ── 綠燈結束：行人延長偵測 ──────────────────────────────────────
  // 若還有行人（ALU 判斷 dist < DETECT_DIST）且未超過延長次數上限
  int extendCount = 0;  // SRAM：延長計數器
  while (extendCount < MAX_EXTEND && personDetected()) {
    Serial.print("[延長] 偵測到行人，綠燈延長 ");
    Serial.print(EXTEND_TIME / 1000);
    Serial.println(" 秒");
    delay(EXTEND_TIME);  // 延長綠燈
    extendCount++;        // ALU：計數器遞增
  }
  if (extendCount > 0) {
    Serial.print("[延長結束] 共延長 ");
    Serial.print(extendCount);
    Serial.println(" 次");
  }

  // ── 黃燈階段 ────────────────────────────────────────────────────
  setLight(false, true, false);
  Serial.println("[黃燈] 準備停止通行");
  delay(YELLOW_TIME);

  // ── 紅燈階段 ────────────────────────────────────────────────────
  setLight(false, false, true);
  Serial.println("[紅燈] 禁止通行");
  delay(RED_TIME);
}
