#include <Arduino.h>
#include <Wire.h>
#include <VL53L1X.h>
#include <L298N.h>
#include "LCD.h"
#include "TOF400.h"
#include "Sample.h"
 
LCD lcd(17, 16, 4); // กำหนดที่อยู่ I2C และขนาดจอ LCD
L298N motor(25, 33, 32); // กำหนดพินสำหรับควบคุมมอเตอร์ L298N
 
void setup()
{
  Serial.begin(9600);

  lcd.begin(); // เริ่มต้นการทำงานของ LCD
  TOF400::begin(); // เริ่มต้นการทำงานของเซ็นเซอร์ TOF400
  motor.setSpeed(0); // ตั้งความเร็วของมอเตอร์ (0-255)
}
 
int lastPrintTime = 0;

void loop()
{
  // if (millis() - lastPrintTime > 100) { // พิมพ์ทุกๆ 100ms เพื่อไม่ให้ข้อมูลล้น Serial Plotter
  //   lastPrintTime = millis();
  //   lcd.clearScreen();
  //   lcd.setCursor(0, 50);
  //   lcd.print("Distance: " + String(TOF400::getDistance() / 10.0) + " cm");
  // }

  for (int i = 0; i < 256; i++) {
    motor.setSpeed(i);
    motor.forward();
    
    Serial.println("Speed: " + String(i));
    if (i >= 255) {
      Serial.println("Max speed reached. Stopping motor.");
      delay(20000); // รอ 1 วินาทีเพื่อให้มอเตอร์หยุดนิ่ง
    }
    delay(200);
  }

}