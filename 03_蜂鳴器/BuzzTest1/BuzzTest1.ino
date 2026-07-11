const int buzzerPin = 8; // 定義蜂鳴器連接的引腳

void setup() {
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  digitalWrite(buzzerPin, HIGH); // 使蜂鳴器發聲
  delay(1000); // 延遲 1 秒
  digitalWrite(buzzerPin, LOW); // 使蜂鳴器靜音
  delay(1000); // 延遲 1 秒
}