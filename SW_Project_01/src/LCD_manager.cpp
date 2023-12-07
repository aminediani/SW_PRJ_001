#include "LCD_manager.h"

static LiquidCrystal_PCF8574 lcd(LCD_address);



void showTextLine1(String text)
{
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print(text);
}

void showText(String text1, String text2)
{
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(text1);
    lcd.setCursor(0,1);
    lcd.print(text2);
}

void init_LCD()
{

  int error=0;
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    lcd.begin(16, 2);  // initialize the lcd
    Serial.println("begin: ");


  } else {
    Serial.println(": LCD not found.");
  }  // if
}
