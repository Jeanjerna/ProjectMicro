#include "Magnet.h"
#include <L298N.h>

Magnet::Magnet(int EN , int IN1 , int IN2) {
  magnet = new L298N(EN, IN1, IN2);
}

void Magnet::begin() {
    magnet->setSpeed(0); // ตั้งความเร็วเริ่มต้นเป็น 0
    magnet->stop(); // หยุดมอเตอร์ตอนเริ่มต้น
}

void Magnet::on(int speed) {
    magnet->setSpeed(speed); // ตั้งความเร็วตามที่รับเข้ามา
    magnet->forward(); // ให้มอเตอร์หมุนไปข้างหน้า
}

void Magnet::off() {
    magnet->stop(); // หยุดมอเตอร์
}