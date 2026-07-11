// ───────────────────────────────
// 功能說明
// 主功能：紅綠燈程式 + 蜂鳴器音效
// 副功能：按住按鈕閃黃燈（故障模式）
// 亮點：非阻塞式程式設計、陣列使用
// 本週新增：各燈號對應不同蜂鳴器聲音
//   綠燈 → 快速短嗶（100ms嗶 / 400ms靜，1000Hz）
//   黃燈 → 持續中頻聲（750Hz，提醒注意）
//   紅燈 → 慢速長嗶（200ms嗶 / 800ms靜，500Hz）
//   故障 → 隨黃燈閃爍同步嗶聲（800Hz）
// ───────────────────────────────

// ───────────────────────────────
// 腳位定義
// ───────────────────────────────
const int BTN        = 8;   // 按鈕
const int GreenLamp  = 3;   // 綠燈
const int YellowLamp = 4;   // 黃燈
const int Redlamp    = 5;   // 紅燈
const int BUZZ       = 9;   // 蜂鳴器（本週新增）

// ───────────────────────────────
// 燈號時序設定
// ───────────────────────────────
const unsigned long PHASE_DUR[] = {3000, 1000, 3000}; // 綠/黃/紅 亮燈時間(ms)
const int           PINS[]      = {GreenLamp, YellowLamp, Redlamp};

// ───────────────────────────────
// 各燈號蜂鳴器設定（本週新增）
// BUZZ_ON_MS = 0 代表持續發聲，不閃爍
// ───────────────────────────────
const int           BUZZ_FREQ[]   = {1000, 750,  500};  // 綠/黃/紅 頻率(Hz)
const unsigned long BUZZ_ON_MS[]  = {100,  0,    200};  // 發聲持續時間(ms)
const unsigned long BUZZ_OFF_MS[] = {400,  0,    800};  // 靜音持續時間(ms)

// ───────────────────────────────
// 狀態變數
// ───────────────────────────────
int           phase      = 0;
unsigned long phaseStart = 0;
unsigned long faultBlink = 0;
bool          faultOn    = false;

unsigned long buzzTimer = 0;    // 蜂鳴器計時基準
bool          buzzState = false; // 目前蜂鳴器是否正在發聲

// ───────────────────────────────
// 將所有燈號熄滅
// ───────────────────────────────
void allOff() {
    digitalWrite(GreenLamp,  LOW);
    digitalWrite(YellowLamp, LOW);
    digitalWrite(Redlamp,    LOW);
}

// ───────────────────────────────
// 啟動某個燈號階段（同時重設蜂鳴器）
// ───────────────────────────────
void startPhase(int p) {
    allOff();
    noTone(BUZZ);
    phase      = p;
    phaseStart = millis();
    buzzTimer  = millis();
    buzzState  = false;
    digitalWrite(PINS[p], HIGH);
}

// ───────────────────────────────
// 非阻塞式蜂鳴器控制
// 根據目前燈號 (phase) 決定音效
// ───────────────────────────────
void updateBuzzer(unsigned long now) {
    if (BUZZ_ON_MS[phase] == 0) {
        tone(BUZZ, BUZZ_FREQ[phase]); // 持續發聲（黃燈）
        return;
    }

    if (buzzState) {
        // 目前發聲中，時間到就停止
        if (now - buzzTimer >= BUZZ_ON_MS[phase]) {
            noTone(BUZZ);
            buzzState = false;
            buzzTimer = now;
        }
    } else {
        // 目前靜音中，時間到就發聲
        if (now - buzzTimer >= BUZZ_OFF_MS[phase]) {
            tone(BUZZ, BUZZ_FREQ[phase]);
            buzzState = true;
            buzzTimer = now;
        }
    }
}

// ───────────────────────────────
// 初始化設定
// ───────────────────────────────
void setup() {
    pinMode(BTN,        INPUT_PULLUP);
    pinMode(GreenLamp,  OUTPUT);
    pinMode(YellowLamp, OUTPUT);
    pinMode(Redlamp,    OUTPUT);
    pinMode(BUZZ,       OUTPUT);    // 蜂鳴器設為輸出（本週新增）
    startPhase(0);
}

// ───────────────────────────────
// 主迴圈：持續檢查按鈕與燈號狀態
// ───────────────────────────────
void loop() {
    unsigned long now = millis();

    if (digitalRead(BTN) == LOW) {
        // ── 故障模式：黃燈與蜂鳴器同步閃爍 ──
        if (now - faultBlink >= 500) {
            faultBlink = now;
            faultOn    = !faultOn;
            allOff();
            if (faultOn) {
                digitalWrite(YellowLamp, HIGH);
                tone(BUZZ, 800);    // 黃燈亮時，蜂鳴器同步響
            } else {
                noTone(BUZZ);       // 黃燈暗時，蜂鳴器同步停
            }
        }
        phaseStart = now;   // 凍結正常模式計時，放開按鈕後從現在開始算
        buzzTimer  = now;   // 凍結蜂鳴器計時，放開按鈕後重新開始
        buzzState  = false;

    } else {
        // ── 正常時序模式 ──
        faultOn = false;
        if (now - phaseStart >= PHASE_DUR[phase]) {
            startPhase((phase + 1) % 3); // 循環切換：綠→黃→紅→綠
        } else {
            updateBuzzer(now);           // 非阻塞式更新蜂鳴器狀態（本週新增）
        }
    }
}
