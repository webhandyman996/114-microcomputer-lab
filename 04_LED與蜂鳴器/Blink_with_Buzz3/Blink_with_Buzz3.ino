// 定義音階對應的頻率（Hz）
#define NOTE_C  261  // Do
#define NOTE_D  293  // Re
#define NOTE_E  329  // Mi
#define NOTE_F  349  // Fa
#define NOTE_G  392  // So
#define NOTE_A  440  // La
#define NOTE_B  493  // Si

int buzzerPin = 9;  // 蜂鳴器接腳位

void setup() {
  // 不需設定 pinMode，tone() 會自動處理
  playScale();  // 播放一次音階
}

void loop() {
  // 可重複播放
  delay(1000);
  playScale();
}

void playScale() {
  tone(buzzerPin, NOTE_C, 500); delay(300);
  tone(buzzerPin, NOTE_D, 500); delay(300);
  tone(buzzerPin, NOTE_E, 500); delay(300);
  tone(buzzerPin, NOTE_F, 500); delay(300);
  tone(buzzerPin, NOTE_G, 500); delay(300);
  tone(buzzerPin, NOTE_A, 500); delay(300);
  tone(buzzerPin, NOTE_B, 500); delay(300);
  noTone(buzzerPin);  // 停止聲音
}