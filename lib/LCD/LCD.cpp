#include "LCD.h" // อย่าลืม include header ของตัวเอง

#include <SPI.h>
#include "Ucglib.h"

LCD::LCD(int RES , int DC , int CS) {
    ucg = new Ucglib_ST7735_18x128x160_HWSPI(DC, CS, RES);
}

void LCD::begin(){
    ucg->begin(UCG_FONT_MODE_TRANSPARENT);
    ucg->setRotate90();
    ucg->clearScreen();
    ucg->setFont(ucg_font_ncenR12_tr);
}
void LCD::clearScreen(){
  ucg->clearScreen();
}
void LCD::setCursor(int x, int y){
  ucg->setPrintPos(x, y);
}
void LCD::print(const String &text){
  ucg->print(text);
}
void LCD::setColor(int r, int g, int b){
  ucg->setColor(r, g, b);
}