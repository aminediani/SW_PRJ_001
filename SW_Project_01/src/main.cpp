#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>
#include "display7seg_manager.h"
//#include "LCD_manager.h"
#include <Wire.h>
//#include <string.h>

// 7seg Module connection pins (Digital Pins)
#define CLK_7seg 9
#define DIO_7seg 8

#define TEST_DELAY   500



//Devices initialization
TM1637Display display(CLK_7seg, DIO_7seg);
LiquidCrystal_PCF8574 lcd(0x27);

// Function declaration
void dig7seg_test();
void crystal_Test();
void LCD_init_I2C_Scanner(int address);

// Setup function
void setup() {
  
  Serial.begin(9600);
  Serial.println("Start...");
  LCD_init_I2C_Scanner(0x27);
  lcd.setBacklight(255);
  display.setBrightness(0x0f);

}

//Main Loop
void loop() {


  dig7seg_test();
  crystal_Test();
}

// digital 7 seg Test
void dig7seg_test(){
  float value = 12.34f;
  showValue7seg(display,value);
  delay(TEST_DELAY);
  clear7seg(display);
  delay(TEST_DELAY);
}

// LCD crystal test
void crystal_Test(){
    lcd.home();
    lcd.clear();
    lcd.print("lCD text");
  delay(TEST_DELAY);

}

void LCD_init_I2C_Scanner(int address){
  
byte dotOff[] = { 0b00000, 0b01110, 0b10001, 0b10001,
                  0b10001, 0b01110, 0b00000, 0b00000 };
byte dotOn[] = { 0b00000, 0b01110, 0b11111, 0b11111,
                 0b11111, 0b01110, 0b00000, 0b00000 };
  int error;
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    lcd.begin(16, 2);  // initialize the lcd

    lcd.createChar(1, dotOff);
    lcd.createChar(2, dotOn);

  } else {
    Serial.println(": LCD not found.");
  }  // if
}