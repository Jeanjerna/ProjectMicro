#ifndef TOF400_H  // ป้องกันการ include ซ้ำ (Include Guard)
#define TOF400_H

#include <Arduino.h> // ต้องมี เพื่อให้รู้จัก type เช่น int, bool, HIGH, LOW
#include <VL53L1X.h>

class TOF400 {
    private:
        static VL53L1X sensor; // สร้าง instance ของเซ็นเซอร์เป็น static เพื่อใช้ทั่วทั้งคลาส

  public:
    /**
     * ฟังก์ชันสำหรับเริ่มต้นการทำงานของเซ็นเซอร์ TOF400 โดยจะตั้งค่าต่างๆ เช่น โหมดการวัด, ขนาด ROI, และความถี่ในการวัด
     * * ใช้ใน Setup()
     */
    static void begin();

    /**
     * ฟังก์ชันสำหรับอ่านค่าจากเซ็นเซอร์ TOF400
     * @return ค่าระยะทางที่อ่านได้จากเซ็นเซอร์ (หน่วย mm)
     */
    static double getDistance();
};

#endif