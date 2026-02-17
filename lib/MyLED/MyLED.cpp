#include "MyLed.h" // อย่าลืม include header ของตัวเอง

MyLed::MyLed(int pin) {
  _pin = pin;
}

// Implement begin()
void MyLed::begin() {
  pinMode(_pin, OUTPUT);
  off(); // เริ่มต้นให้ดับก่อน
}

// Implement on()
void MyLed::on() {
  digitalWrite(_pin, HIGH);
}

// Implement off()
void MyLed::off() {
  digitalWrite(_pin, LOW);
}

// Implement toggle()
void MyLed::toggle() {
  int state = digitalRead(_pin);
  digitalWrite(_pin, !state);
}