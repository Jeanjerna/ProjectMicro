#ifndef MYLED_H  // ป้องกันการ include ซ้ำ (Include Guard)
#define MYLED_H

#include <Arduino.h> // ต้องมี เพื่อให้รู้จัก type เช่น int, bool, HIGH, LOW

class MyLed {
  private:
    int static _pin; // ตัวแปร private (มักใช้ _ นำหน้า) เก็บเลขขา

  public:
    // Constructor: รับค่าขา Pin เข้ามา
    MyLed(int pin);

    // Method: ฟังก์ชันต่างๆ
    void begin();
    void on();
    void off();
    void toggle();
};

#endif