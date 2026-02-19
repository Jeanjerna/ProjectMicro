#include "Solenoid.h" // อย่าลืม include header ของตัวเอง

Solenoid::Solenoid(int pin) {
  _pin = pin;
}

// Implement begin()
void Solenoid::begin() {
  pinMode(_pin, OUTPUT);
  off(); // เริ่มต้นให้ปิดก่อน
}

// Implement on()
void Solenoid::on() {
  digitalWrite(_pin, HIGH);
}

// Implement off()
void Solenoid::off() {
    digitalWrite(_pin, LOW);
}