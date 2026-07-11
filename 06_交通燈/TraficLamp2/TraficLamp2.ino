// ───────────────────────────────
// 功能說明
// 主功能:紅綠燈程式
// 副功能:按住按鈕閃黃燈
// 亮點:非阻塞式程式設計 
//      陣列使用
// ───────────────────────────────
// ───────────────────────────────
// 定義按鈕與燈號腳位
// ───────────────────────────────
const int BTN = 8;              // 按鈕接在腳位 8，用來切換到故障模式
const int GreenLamp = 3, YellowLamp = 4, Redlamp = 5;  // 綠燈、黃燈、紅燈分別接在腳位 3、4、5
//GrnLmp //Button->BTN 

// ───────────────────────────────
// 燈號時序設定
// ───────────────────────────────
// 正常模式下各燈號持續時間 (毫秒)
const unsigned long PHASE_DUR[] = {3000, 1000, 3000}; // 綠燈 5 秒，黃燈 2 秒，紅燈 5 秒
const int PINS[] = {GreenLamp, YellowLamp, Redlamp};   // 方便用陣列存取燈號腳位

// ───────────────────────────────
// 狀態變數
// ───────────────────────────────
int phase = 0;                  // 目前燈號階段：0=綠燈, 1=黃燈, 2=紅燈
unsigned long phaseStart = 0;   // 當前燈號開始的時間點 (毫秒)
unsigned long faultBlink = 0;   // 故障模式下的閃爍計時基準
bool faultOn = false;           // 故障模式下，黃燈是否亮起

// ───────────────────────────────
// 將所有燈號熄滅
// ───────────────────────────────
void allOff() {
    digitalWrite(GreenLamp, LOW);
    digitalWrite(YellowLamp, LOW);
    digitalWrite(Redlamp, LOW);
}

// ───────────────────────────────
// 啟動某個燈號階段
// ───────────────────────────────
void startPhase(int p) {
    allOff();                   // 先全部熄滅，避免重疊
    phase = p;                  // 更新目前階段
    phaseStart = millis();      // 記錄開始時間
    digitalWrite(PINS[p], HIGH);// 將對應燈號打開
}

// ───────────────────────────────
// 初始化設定
// ───────────────────────────────
void setup() {
    pinMode(BTN, INPUT_PULLUP); // 按鈕使用內建上拉電阻
    pinMode(GreenLamp, OUTPUT);         // 綠燈腳位設為輸出
    pinMode(YellowLamp, OUTPUT);         // 黃燈腳位設為輸出
    pinMode(Redlamp, OUTPUT);         // 紅燈腳位設為輸出

    startPhase(0);              // 程式啟動時，先從綠燈開始
}

// ───────────────────────────────
// 主迴圈：持續檢查按鈕與燈號狀態
// ───────────────────────────────
void loop() {
    unsigned long now = millis();   // 取得目前時間 (毫秒)

    if (digitalRead(BTN) == LOW) {
        // ── 故障模式：黃燈閃爍 ──
        if (now - faultBlink >= 500) {  // 每 500ms 切換一次狀態
            faultBlink = now;           // 更新閃爍基準時間
            faultOn = !faultOn;         // 切換亮/暗狀態
            allOff();                   // 先全部熄滅
            if (faultOn) digitalWrite(YellowLamp, HIGH); // 若狀態為亮，點亮黃燈
        }
        phaseStart = now;   // 若故障模式結束，重設正常模式的計時基準
    } else {
        // ── 正常時序模式 ──
        faultOn = false;    // 確保故障狀態關閉
        if (now - phaseStart >= PHASE_DUR[phase]) {
            // 當前燈號時間已到，切換到下一個燈號
            startPhase((phase + 1) % 3); // 循環切換：G→Y→R→G
        }
    }
}
