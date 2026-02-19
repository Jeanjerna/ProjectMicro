#include <Arduino.h>
#include <Wire.h>
#include <VL53L1X.h>
#include "LCD.h"
#include "TOF400.h"
#include "Sample.h"
#include "Solenoid.h"
#include "Magnet.h"
 
LCD lcd(17, 16, 4); 
Magnet magnet(25, 33, 32);
TOF400 tof;
Solenoid solenoid(26);
 
void setup()
{
  Serial.begin(9600);

  lcd.begin();
  tof.begin();
  solenoid.begin();
  magnet.begin();
}
 
int lastPrintTime = 0;

void loop()
{
  // if (millis() - lastPrintTime > 100) { // พิมพ์ทุกๆ 100ms เพื่อไม่ให้ข้อมูลล้น Serial Plotter
  //   lastPrintTime = millis();
  //   lcd.clearScreen();
  //   lcd.setCursor(0, 50);
  //   lcd.print("Distance: " + String(tof.getDistance() / 10.0) + " cm");
  // }

  for (int i = 0; i < 256; i++) {
    magnet.on(i);
    
    Serial.println("Speed: " + String(i));
    if (i >= 255) {
      Serial.println("Max speed reached. Stopping magnet.");
      delay(6000); // รอ 1 วินาทีเพื่อให้มอเตอร์หยุดนิ่ง
    }
    delay(50);
  }

  // digitalWrite(solenoldPin, HIGH); // เริ่มต้นให้โซลินอยด์ปิดอยู่
  // delay(800);
  // digitalWrite(solenoldPin, LOW); // เริ่มต้นให้โซลินอยด์ปิดอยู่
  // delay(2000);

}