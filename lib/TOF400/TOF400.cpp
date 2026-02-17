#include "TOF400.h"
#include <Wire.h>


VL53L1X TOF400::sensor; // กำหนดตัวแปร sensor เป็น static เพื่อใช้ทั่วทั้งคลาส

void TOF400::begin() {
    Wire.begin();
    Wire.setClock(400000);

    sensor.setTimeout(500);
    if (!sensor.init())
    {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
    }

    // --- ส่วนที่ต้องแก้สำหรับการวัด Gravity ในท่อ ---

    // 1. ลดขนาด ROI (Region of Interest)
    // ค่าต่ำสุดคือ 4x4 (จากเต็ม 16x16) เพื่อบีบลำแสงให้แคบที่สุด ไม่ให้ชนผนังท่อ
    sensor.setROISize(4, 4); 

    // 2. ใช้ Short Mode เพื่อความไวสูงสุด (Max range ~1.3m)
    // ถ้าลูกบอลหลุดระยะ 1.3m ค่าจะกลายเป็น 0 หรือ 8190 ก็แค่ตัดทิ้งตอนคำนวณ
    sensor.setDistanceMode(VL53L1X::Medium);

    // 3. ปรับเวลาต่อครั้งให้สั้นที่สุด (20ms = 50Hz)
    // ยิ่งเวลาน้อย จุดกราฟยิ่งเยอะ คำนวณ g ได้แม่นยำขึ้น
    sensor.setMeasurementTimingBudget(20000);

    // เริ่มอ่านแบบต่อเนื่องทุกๆ 20ms
    sensor.startContinuous(20);
}

double TOF400::getDistance() {
    sensor.read();
    return sensor.ranging_data.range_mm; // Convert mm to cm for easier reading
}