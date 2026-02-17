#include "Sample.h" // อย่าลืม include header ของตัวเอง
#include "LCD.h" // อย่าลืม include header ของ LCD เพื่อใช้ใน Sample

void Sample::SampleLCD(LCD &lcd) {
  lcd.clearScreen();
  lcd.setColor(255, 255, 255);

  lcd.setColor(255, 0, 0);

  lcd.setCursor(0, 25);
  lcd.print("Hello World!");
  lcd.setCursor(0, 50);
  lcd.setColor(255, 255, 0);
  lcd.print("Myarduino.net");
}