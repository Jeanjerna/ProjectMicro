#ifndef LCD_H  // ป้องกันการ include ซ้ำ (Include Guard)
#define LCD_H

#include <Arduino.h>
#include <SPI.h>
#include "Ucglib.h"

class LCD {
  private:
    Ucglib_ST7735_18x128x160_HWSPI *ucg; // ตัวแปรสำหรับควบคุม LCD

  public:
    // Constructor: รับค่าขา Pin เข้ามา
    LCD(int RES , int DC , int CS);

    // Method: ฟังก์ชันต่างๆ
    void begin();
    void clearScreen();
    void setCursor(int x, int y);
    void print(const String &text);
    void setColor(int r, int g, int b);
};

#endif