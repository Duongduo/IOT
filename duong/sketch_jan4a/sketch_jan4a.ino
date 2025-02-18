#define LED_GREEN 25
#define LED_YELLOW 26
#define LED_RED 27

#define BUTTON_GREEN 33
#define BUTTON_YELLOW 32
#define BUTTON_RED 14

bool ledGreenState = false;
bool ledYellowState = false;
bool ledRedState = false;

bool buttonGreenPrevState = HIGH;
bool buttonYellowPrevState = HIGH;
bool buttonRedPrevState = HIGH;

void setup() {
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(BUTTON_RED, INPUT_PULLUP);

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
}

void loop() {
  // Kiểm tra trạng thái nút xanh lá
  bool buttonGreenState = digitalRead(BUTTON_GREEN);
  if (buttonGreenState == LOW && buttonGreenPrevState == HIGH) {
    ledGreenState = !ledGreenState; // Đảo trạng thái LED
    digitalWrite(LED_GREEN, ledGreenState);
  }
  buttonGreenPrevState = buttonGreenState;

  // Kiểm tra trạng thái nút vàng
  bool buttonYellowState = digitalRead(BUTTON_YELLOW);
  if (buttonYellowState == LOW && buttonYellowPrevState == HIGH) {
    ledYellowState = !ledYellowState; // Đảo trạng thái LED
    digitalWrite(LED_YELLOW, ledYellowState);
  }
  buttonYellowPrevState = buttonYellowState;

  // Kiểm tra trạng thái nút đỏ
  bool buttonRedState = digitalRead(BUTTON_RED);
  if (buttonRedState == LOW && buttonRedPrevState == HIGH) {
    ledRedState = !ledRedState; // Đảo trạng thái LED
    digitalWrite(LED_RED, ledRedState);
  }
  buttonRedPrevState = buttonRedState;

  // Thêm độ trễ để chống dội nút
  delay(50);
}
