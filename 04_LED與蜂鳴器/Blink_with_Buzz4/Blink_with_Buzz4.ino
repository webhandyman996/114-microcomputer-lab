// 定義音階頻率（低音 Do~Si）
#define NOTE_C  261 //DO
#define NOTE_D  294 //RE
#define NOTE_E  329 //MI
#define NOTE_F  349 //FA
#define NOTE_G  392 //SO
#define NOTE_A  440 //LA
#define NOTE_B  493 //SI

int buzzerPin = 9;  // 蜂鳴器腳位

// 音樂音符（簡單旋律）
int melody[] = {
  NOTE_C, NOTE_C, NOTE_G, NOTE_G, NOTE_A, NOTE_A, NOTE_G,
  NOTE_F, NOTE_F, NOTE_E, NOTE_E, NOTE_D, NOTE_D, NOTE_C,
  NOTE_G, NOTE_G, NOTE_F, NOTE_F, NOTE_E, NOTE_E, NOTE_D,
  NOTE_G, NOTE_G, NOTE_F, NOTE_F, NOTE_E, NOTE_E, NOTE_D,
  NOTE_C, NOTE_C, NOTE_G, NOTE_G, NOTE_A, NOTE_A, NOTE_G,
  NOTE_F, NOTE_F, NOTE_E, NOTE_E, NOTE_D, NOTE_D, NOTE_C
};

// 每個音符的長度（4=四分音符，8=八分音符）
int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2
};

void setup() {
  
}

void loop() {
  for (int i = 0; i < sizeof(melody) / sizeof(int); i++) {
    int duration = 1000 / noteDurations[i]; // 四分音符=250ms
    tone(buzzerPin, melody[i], duration);
    delay(duration * 1.3);  // 稍作停頓
    noTone(buzzerPin);
  }
  delay(1000); 
}