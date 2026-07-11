// 七段顯示器段位對應的腳位
// Segment pin mapping for the 7-segment display
const int segmentPins[7] = {2, 3, 4, 5, 6, 7, 8};

// 數字 0~9 對應的段位亮滅狀態（共陰極，1 = 亮）
// Segment on/off states for digits 0–9 (common cathode, 1 = ON)
const byte digits[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,0,0,1,1}  // 9
};

void setup() {
  // 將所有段位設定為輸出模式
  // Set all segment pins as OUTPUT
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
}

void loop() {
  // 循環顯示 0~9，每秒切換一次
  // Loop through digits 0–9, changing once every second
  for (int num = 0; num < 10; num++) {
    displayDigit(num);  // 顯示對應數字 / Display corresponding digit
    delay(1000);        // 等待 1 秒 / Wait for 1 second
  }
}

// 顯示特定數字的函式
// Function to display a specific digit
void displayDigit(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], digits[num][i]); // 控制每個段位的亮滅 / Set each segment ON or OFF
  }
}
