# 蜂鳴器教學 PPT 文稿
## 微算機原理與實習

---

## Slide 1｜封面

**蜂鳴器控制**
從單晶片輸出原理到頻率音效

> 本節對應範例：BuzzTest1 / BuzzTest2 / buzzer1 / buzzer2

---

## Slide 2｜【理論】單晶片如何驅動周邊元件？

單晶片（MCU）內部主要由四大區塊組成：

```
┌─────────────────────────────────────┐
│              單晶片 (MCU)            │
│                                     │
│   CPU          記憶體               │
│  （運算）    （程式/資料儲存）        │
│                                     │
│   Timer/Counter    I/O Port  ───────┼──→ 蜂鳴器、LED、按鈕
│  （計時/產生波形）  （輸入/輸出腳位） │
└─────────────────────────────────────┘
```

**重點：**
- 蜂鳴器、LED 都接在 **I/O Port（輸入/輸出腳位）**
- 音調的產生靠 **Timer/Counter** 硬體，不需要 CPU 一直忙著計時
- `pinMode(pin, OUTPUT)` 就是告訴 MCU：「這隻腳要當輸出用」

---

## Slide 3｜【理論】數位輸出與方波

MCU 的輸出腳位只有兩種電壓狀態：

```
電壓
 5V ─────┐    ┌─────┐    ┌─────
         │    │     │    │
 0V      └────┘     └────┘
         HIGH  LOW  HIGH  LOW
```

- `digitalWrite(pin, HIGH)` → 輸出 **5V**
- `digitalWrite(pin, LOW)`  → 輸出 **0V**
- 快速交替 HIGH / LOW → 形成**方波（Square Wave）**
- 蜂鳴器膜片隨電壓變化**振動** → 產生聲音

> 振動快 → 頻率高 → 音調高；振動慢 → 頻率低 → 音調低

---

## Slide 4｜蜂鳴器種類與接線

| 類型 | 特性 | 控制方式 |
|------|------|----------|
| **主動式** | 內建振盪電路，直接給電就響 | `digitalWrite(HIGH/LOW)` |
| **被動式** | 需外部提供頻率才能發聲 | `tone(pin, 頻率)` |

> 本課使用**被動式蜂鳴器**，音調由程式控制。

**接線：**
```
Arduino GND ──── 蜂鳴器負極（短腳）
Arduino D8  ──── 蜂鳴器正極（長腳）   ← BuzzTest1 / BuzzTest2
Arduino D9  ──── 蜂鳴器正極（長腳）   ← buzzer1 / buzzer2
```

---

## Slide 5｜BuzzTest1｜GPIO 直接驅動

`digitalWrite()` 控制蜂鳴器，概念與控制 LED 完全相同：

```cpp
const int buzzerPin = 8;

void loop() {
  digitalWrite(buzzerPin, HIGH); // 輸出 5V → 膜片振動 → 發聲
  delay(1000);
  digitalWrite(buzzerPin, LOW);  // 輸出 0V → 膜片靜止 → 靜音
  delay(1000);
}
```

**限制：** 音調由蜂鳴器硬體決定，程式無法改變頻率。

---

## Slide 6｜【理論】頻率、周期與 Timer

**頻率（f）與周期（T）的關係：**

$$f = \frac{1}{T} \quad \text{單位：Hz（赫茲）= 次/秒}$$

```
tone(buzzerPin, 1000)  →  1000 Hz  →  每秒振動 1000 次
                                       周期 T = 1ms
```

**人耳可聽範圍：** 約 20 Hz ～ 20,000 Hz

| 頻率 | 聽感 | 對應音符 |
|------|------|----------|
| 262 Hz | 低 | Do（中央C） |
| 523 Hz | 中低 | 高八度Do |
| 1000 Hz | 中 | 嗶聲 |
| 2000 Hz | 高 | 尖銳聲 |

**`tone()` 背後的機制：**
- 呼叫 `tone()` 後，Arduino 內部的 **Timer 硬體**接手工作
- Timer 自動產生指定頻率的方波，**CPU 不需要一直等待**
- 這就是為什麼呼叫 `tone()` 後，程式可以繼續執行其他指令

---

## Slide 7｜BuzzTest2 / buzzer1｜tone() 控制音調

**tone() 與 noTone() 用法：**

```cpp
tone(腳位, 頻率);   // 交給 Timer 產生指定頻率，持續發聲
noTone(腳位);       // 停止 Timer，蜂鳴器靜音
```

**BuzzTest2：按下按鈕才發聲**
```cpp
if (digitalRead(inputPin) == LOW) {
    tone(buzzerPin, 1000);   // 1000Hz
} else {
    noTone(buzzerPin);
}
```

**buzzer1：頻率掃描（frequencySweep）**
```cpp
for (int i = 500; i <= 1500; i += 10) {
    tone(buzzerPin, i);  // 每次升高 10Hz
    delay(5);
}
// 500Hz → 510Hz → ... → 1500Hz → 循環，形成「嗚」聲
```

> `digitalWrite` vs `tone()`：前者給電壓、後者給頻率，這是主動式與被動式蜂鳴器的根本差異。

---

## Slide 8｜buzzer2｜整合應用：按鈕 + LED + 蜂鳴器

**按下按鈕 → LED 亮 + 蜂鳴器響，多元件同時協同：**

```cpp
void BuzzWithLED() {
  if (digitalRead(ButtonPin) == LOW) {
    digitalWrite(ledPin1, HIGH);   // LED 亮
    digitalWrite(buzzerPin, HIGH); // 蜂鳴器響
  } else {
    digitalWrite(ledPin1, LOW);
    digitalWrite(buzzerPin, LOW);
  }
}
```

**腳位配置：**

| 元件 | 腳位 | 方向 |
|------|------|------|
| LED 1 / 2 / 3 | D3 / D4 / D5 | OUTPUT |
| 按鈕 | D8 | INPUT\_PULLUP |
| 蜂鳴器 | D9 | OUTPUT |

> `INPUT_PULLUP`：MCU 內部上拉電阻，按下時腳位讀到 LOW。這是 MCU I/O Port 的硬體特性。

---

## Slide 9｜四範例的學習脈絡

```
BuzzTest1          BuzzTest2          buzzer1            buzzer2
────────────       ──────────────     ──────────────     ──────────────
GPIO 直接驅動      Timer + tone()     頻率掃描迴圈        多元件整合
固定音調            可指定頻率          連續音效變化         按鈕觸發協同
理解方波概念        理解 f = 1/T       理解 for 迴圈        整合所有概念
```

**對應理論：**
- BuzzTest1 → I/O Port 數位輸出、方波
- BuzzTest2/buzzer1 → Timer/Counter、頻率與周期
- buzzer2 → 多腳位協同、INPUT\_PULLUP

---

## Slide 10｜課堂練習

**練習 1（基礎）**
修改 `BuzzTest1`，讓蜂鳴器每 200ms 切換一次（快速嗶嗶聲）。
→ 對應概念：GPIO 輸出 + delay

**練習 2（進階）**
修改 `frequencySweep()`，改成從高頻掃到低頻（1500Hz → 500Hz）。
→ 對應概念：for 迴圈遞減、Timer 頻率

**練習 3（挑戰）**
讓三顆 LED 依序亮起時，蜂鳴器分別發出 500 / 1000 / 2000 Hz。
→ 對應概念：多腳位協同、tone() 頻率對應

---

## Slide 11｜下週預告

**紅綠燈 + 蜂鳴器音效整合（TraficLamp3）**

| 燈號 | 音效 | 對應本節 |
|------|------|----------|
| 綠燈 | 快速短嗶（1000Hz） | tone() + 非阻塞計時 |
| 黃燈 | 持續中頻聲（750Hz） | tone() 持續 |
| 紅燈 | 慢速長嗶（500Hz） | tone() + 非阻塞計時 |
| 故障閃黃 | 隨黃燈同步（800Hz） | Timer + I/O 同步 |

> 把這節的 Timer 原理與 tone() 用法，結合上週的非阻塞式設計，就是下週的核心！
