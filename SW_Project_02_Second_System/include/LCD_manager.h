#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#define LCD_address 0x27


void showTextLine1(String text);
void showTextLine2(String text);
void showText(String text1, String text2);
void init_LCD();
