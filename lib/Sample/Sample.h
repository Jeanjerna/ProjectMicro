#ifndef SAMPLE_H  // ป้องกันการ include ซ้ำ (Include Guard)
#define SAMPLE_H

#include <Arduino.h> // ต้องมี เพื่อให้รู้จัก type เช่น int, bool, HIGH, LOW
#include "LCD.h" // อย่าลืม include header ของ LCD เพื่อใช้ใน Sample

class Sample {

  public:
    static void SampleLCD(LCD &lcd);
};

#endif