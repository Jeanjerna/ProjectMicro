#ifndef SOLENOID_H  // ป้องกันการ include ซ้ำ (Include Guard)
#define SOLENOID_H

#include <Arduino.h> // ต้องมี เพื่อให้รู้จัก type เช่น int, bool, HIGH, LOW

class Solenoid {
    private:
        int _pin; // ตัวแปร private เก็บเลขขาที่ใช้ควบคุมโซลินอยด์

    public:
    // Constructor: รับค่าขา Pin เข้ามา
    Solenoid(int pin);

    /**
     * ฟังก์ชันสำหรับเริ่มต้นการทำงานของโซลินอยด์ โดยจะตั้งค่าพินที่ใช้ควบคุมโซลินอยด์เป็น OUTPUT และเริ่มต้นให้โซลินอยด์ปิดอยู่
     * * ใช้ใน Setup()
     */
    void begin();

    /**
     * ฟังก์ชันสำหรับเปิดโซลินอยด์ โดยจะส่งสัญญาณ HIGH ไปยังพินที่ใช้ควบคุมโซลินอยด์
     */
    void on();

    /**
     * ฟังก์ชันสำหรับปิดโซลินอยด์ โดยจะส่งสัญญาณ LOW ไปยังพินที่ใช้ควบคุมโซลินอยด์
     */
    void off();

};

#endif