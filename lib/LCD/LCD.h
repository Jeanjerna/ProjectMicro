#ifndef LCD_H  // ป้องกันการ include ซ้ำ (Include Guard)
#define LCD_H

#include <Arduino.h>
#include <SPI.h>
#include "Ucglib.h"

class LCD {
  private:
    Ucglib_ST7735_18x128x160_HWSPI *ucg; // ตัวแปรสำหรับควบคุม LCD

  public:
    /**
     * ? @param RES พินสำหรับ Reset ของ LCD
     * ? @param DC พินสำหรับ Data/Command ของ LCD
     * ? @param CS พินสำหรับ Chip Select ของ LCD
     */
    LCD(int RES , int DC , int CS);

    // Method: ฟังก์ชันต่างๆ
    /**
     * ฟังก์ชันสำหรับเริ่มต้นการทำงานของ LCD โดยจะตั้งค่าต่างๆ
     * * ใช้ใน Setup()
     */
    void begin();
    
    /**
     * * ฟังก์ชันสำหรับล้างหน้าจอ LCD
     */
    void clearScreen();

    /**
     * ? @param x ตำแหน่งแกน X ที่ต้องการตั้ง Cursor
     * ? @param y ตำแหน่งแกน Y ที่ต้องการตั้ง Cursor
     * ฟังก์ชันสำหรับตั้งตำแหน่ง Cursor บนหน้าจอ LCD เพื่อเตรียมพร้อมสำหรับการพิมพ์ข้อความในตำแหน่งนั้น
     */
    void setCursor(int x, int y);

    /**
     * ? @param text ข้อความที่ต้องการพิมพ์บนหน้าจอ LCD
     * ฟังก์ชันสำหรับพิมพ์ข้อความบนหน้าจอ LCD โดยข้อความจะถูกพิมพ์ที่ตำแหน่งที่ตั้งไว้ด้วย setCursor()
     */
    void print(const String &text);

    /**
     * ? @param r ค่าความเข้มของสีแดง (0-255)
     * ? @param g ค่าความเข้มของสีเขียว (0-255)
     * ? @param b ค่าความเข้มของสีน้ำเงิน (0-255)
     * ฟังก์ชันสำหรับตั้งค่าสีที่ใช้ในการพิมพ์ข้อความบนหน้าจอ LCD โดยรับค่าความเข้มของสีแดง เขียว และน้ำเงินเป็นพารามิเตอร์
     */
    void setColor(int r, int g, int b);
};

#endif