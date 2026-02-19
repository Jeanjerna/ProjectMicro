#ifndef MAGNET_H  // ป้องกันการ include ซ้ำ (Include Guard)
#define MAGNET_H

#include <Arduino.h> // ต้องมี เพื่อให้รู้จัก type เช่น int, bool, HIGH, LOW
#include <L298N.h>

class Magnet {
  private:
    L298N *magnet; // ตัวแปรสำหรับควบคุมมอเตอร์ L298N

  public:
    // Constructor: รับค่าขา Pin เข้ามา
    Magnet(int EN , int IN1 , int IN2);

    // Method: ฟังก์ชันต่างๆ
    void begin();
    void on(int speed);
    void off();
};

#endif