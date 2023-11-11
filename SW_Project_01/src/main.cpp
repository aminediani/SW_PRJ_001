#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>
#include "display7seg_manager.h"
#include "LCD_manager.h"

//#include <string.h>

// 7seg Module connection pins (Digital Pins)
#define CLK_7seg 9
#define DIO_7seg 8

#define TEST_DELAY 500



// Function declaration
void dig7seg_test();
void crystal_Test();
void I2C_Scanner(int address);

static int counter = 0;

// Setup function
void setup() {
  
  Serial.begin(9600);
  Serial.println("Start...");
  delay(1000);

  init_LCD();
  Serial.println("Init Done...");


}

//Main Loop
void loop() {
    Serial.println("loop...");
    delay(1000);
    //dig7seg_test();
    crystal_Test();
}

// digital 7 seg Test
void dig7seg_test(){
  // float value = 12.34f;
  // showValue7seg(display,value);
  // delay(TEST_DELAY);
  // clear7seg(display);
  // delay(TEST_DELAY);
}

// LCD crystal test
void crystal_Test(){
  showTextLine1("CNT = " + String(counter));
  delay(TEST_DELAY);
  counter++;

}

void I2C_Scanner(int address){
  
  int error;
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
  } else {
    Serial.println(": LCD not found.");
  }  // if
}