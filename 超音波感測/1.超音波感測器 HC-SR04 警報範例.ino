/*
 * ╔══════════════════════════════════════════════════════════════╗
 * ║      超音波感測器 HC-SR04 警報範例                           ║
 * ║      平台：Arduino Mega 2560（ATmega2560）                   ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  【微電腦五大單元對應】                                       ║
 * ║                                                              ║
 * ║  1. 輸入單元 (Input Unit)                                    ║
 * ║     → HC-SR04 ECHO 腳（D11）：接收超音波反射回波             ║
 * ║     → 對應 ATmega2560 的 PINx 暫存器（讀取腳位狀態）         ║
 * ║                                                              ║
 * ║  2. 輸出單元 (Output Unit)                                   ║
 * ║     → HC-SR04 TRIG 腳（D10）：發送觸發脈衝                   ║
 * ║     → LED（D6）：視覺警示輸出                                ║
 * ║     → 無源蜂鳴器（D8）：聲音警示輸出                         ║
 * ║     → 對應 ATmega2560 的 PORTx 暫存器（設定輸出電位）        ║
 * ║                                                              ║
 * ║  3. 記憶單元 (Memory Unit)                                   ║
 * ║     → Flash ROM（256KB）：儲存此程式碼（唯讀，永久保存）      ║
 * ║     → SRAM（8KB）：儲存執行時的變數（如 dist, duration）      ║
 * ║     → EEPROM（4KB）：可掉電保存的資料區（本程式未使用）       ║
 * ║                                                              ║
 * ║  4. 算術邏輯單元 (ALU)                                       ║
 * ║     → 執行距離換算：distance = duration × 0.034 ÷ 2         ║
 * ║     → 執行條件判斷：dist > 0 && dist < ALERT_DIST            ║
 * ║     → 執行迴圈計數：i < FLASH_COUNT                          ║
 * ║                                                              ║
 * ║  5. 控制單元 (Control Unit)                                  ║
 * ║     → 程式計數器（PC）依序執行 setup() → loop()              ║
 * ║     → 遇 if/for 條件時決定跳轉位址                           ║
 * ║     → 呼叫函式時將返回位址壓入堆疊（Stack）                   ║
 * ╚══════════════════════════════════════════════════════════════╝
 *
 * 接線說明：
 *   HC-SR04  →  Mega 2560
 *   VCC      →  5V
 *   GND      →  GND
 *   TRIG     →  D10
 *   ECHO     →  D11
 *   LED      →  D6（串接 220Ω 限流電阻）
 *   蜂鳴器   →  D8（無源蜂鳴器）
 */

// ── 腳位定義 ──────────────────────────────────────────────────────
// Arduino 的 pinMode()/digitalWrite() 底層操作的是
// ATmega2560 的 DDRx / PORTx 暫存器
//   DDRx  暫存器：設定腳位方向（0=INPUT, 1=OUTPUT）
//   PORTx 暫存器：設定輸出電位（0=LOW, 1=HIGH）
//   PINx  暫存器：讀取輸入電位（唯讀）
const int TRIG_PIN   = 10;  // 輸出單元：觸發 HC-SR04 發射超音波
const int ECHO_PIN   = 11;  // 輸入單元：接收 HC-SR04 回波訊號
const int LED_PIN    = 6;   // 輸出單元：LED 閃爍警示
const int BUZZER_PIN = 8;   // 輸出單元：無源蜂鳴器聲音警示

// ── 參數設定（儲存於 SRAM，程式執行時配置）────────────────────────
const int ALERT_DIST  = 20;   // 觸發警報距離門檻（公分）
const int FLASH_COUNT = 10;   // 閃爍次數
const int FLASH_DELAY = 100;  // 閃爍間隔時間（毫秒）
const int BUZZ_FREQ   = 1000; // 蜂鳴器頻率（Hz）


// ── setup()：初始化，只執行一次 ───────────────────────────────────
// 控制單元在上電後優先執行此函式
// 底層動作：設定 DDRx 暫存器，定義各腳位方向
void setup() {
  // DDR（Data Direction Register）設定：
  // pinMode(pin, OUTPUT) → 對應 DDRx bit 寫入 1
  // pinMode(pin, INPUT)  → 對應 DDRx bit 寫入 0
  pinMode(TRIG_PIN,   OUTPUT); // 觸發腳：設為輸出（DDR bit = 1）
  pinMode(ECHO_PIN,   INPUT);  // 回波腳：設為輸入（DDR bit = 0）
  pinMode(LED_PIN,    OUTPUT); // LED   ：設為輸出（DDR bit = 1）
  pinMode(BUZZER_PIN, OUTPUT); // 蜂鳴器：設為輸出（DDR bit = 1）

  Serial.begin(9600);  // 初始化 UART 序列埠（設定鮑率暫存器 UBRR）
}


// ── getDistance()：測量距離 ────────────────────────────────────────
// 輸入單元 + ALU 協同運作
//
// HC-SR04 工作原理：
//   TRIG 收到 ≥10µs 高電位 → 發射 8 個 40kHz 超音波
//   超音波碰到障礙物反射 → ECHO 腳輸出高電位
//   ECHO 高電位時間 = 超音波來回飛行時間
//
// 距離換算（ALU 執行）：
//   聲速 ≈ 340 m/s = 0.034 cm/µs
//   distance = duration(µs) × 0.034(cm/µs) ÷ 2
//   ÷2 原因：超音波「去＋回」共走兩倍距離
//
// pulseIn() 底層：使用 Timer1 計數暫存器（TCNT1）測量時間
long getDistance() {
  // PORTx 暫存器操作：拉低 TRIG，確保觸發訊號邊緣乾淨
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // PORTx 暫存器操作：輸出 10µs 高電位觸發脈衝
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // PINx 暫存器操作：等待 ECHO 腳高電位並計時（單位：微秒）
  long duration = pulseIn(ECHO_PIN, HIGH);

  // ALU 執行距離換算
  long distance = duration * 0.034 / 2;
  return distance;
}


// ── alert()：警報輸出 ──────────────────────────────────────────────
// 輸出單元動作：LED 閃爍 + 蜂鳴器鳴叫
//
// tone() 底層原理：
//   設定 Timer2 的 OCR2A 比較暫存器產生 PWM 方波
//   頻率 = CPU時脈(16MHz) ÷ 預除頻係數 ÷ (OCR2A+1) ÷ 2
//   無源蜂鳴器依此 PWM 頻率振動發聲（有源蜂鳴器不需要 tone()）
void alert() {
  for (int i = 0; i < FLASH_COUNT; i++) {
    // 輸出單元：PORTx 暫存器寫 1 → LED 腳輸出 HIGH（5V）→ LED 亮
    digitalWrite(LED_PIN, HIGH);
    // 輸出單元：Timer2 PWM 輸出 1000Hz 方波 → 蜂鳴器振膜振動發聲
    tone(BUZZER_PIN, BUZZ_FREQ);
    delay(FLASH_DELAY);

    // 輸出單元：PORTx 暫存器寫 0 → LED 腳輸出 LOW（0V）→ LED 滅
    digitalWrite(LED_PIN, LOW);
    // 輸出單元：停止 Timer2 PWM 輸出 → 蜂鳴器靜音
    noTone(BUZZER_PIN);
    delay(FLASH_DELAY);
  }
}


// ── loop()：主迴圈，無限重複 ──────────────────────────────────────
// 控制單元持續循環執行此函式（程式計數器不斷跳回 loop 起點）
void loop() {
  // 呼叫 getDistance()：控制單元將返回位址壓入堆疊，跳至函式位址
  long dist = getDistance();  // 結果存入 SRAM

  // ALU 執行字串組合；UART 暫存器（UDR）送出序列資料
  Serial.print("距離: ");
  Serial.print(dist);
  Serial.println(" cm");

  // ALU 執行條件判斷（比較暫存器 SREG 的 Zero/Carry flag）
  // dist > 0：排除 pulseIn() 逾時（障礙物過遠或無回波）
  // dist < ALERT_DIST：進入警報範圍
  if (dist > 0 && dist < ALERT_DIST) {
    alert();  // 控制單元：壓堆疊 → 跳至 alert() → 執行後返回
  }

  delay(200); // 每 200ms 偵測一次，減少 CPU 不必要的空轉
}
