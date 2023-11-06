#include "LCD_manager.h"

static LiquidCrystal_PCF8574 lcd(LCD_address);

void showTextLine1(String text)
{
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print("text");
}

