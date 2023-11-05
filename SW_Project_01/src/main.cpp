#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>
#include "display7seg_manager.h"

// 7seg Module connection pins (Digital Pins)
#define CLK_7seg 9
#define DIO_7seg 8

#define TEST_DELAY   500

TM1637Display display(CLK_7seg, DIO_7seg);
void sys_test();
void setup() {
  // 
  display.setBrightness(0x0f);


}

void loop() {
  
  sys_test();


}

void sys_test(){

  //7seg Test
  float value = 12.34f;
  showValue7seg(display,value);
  delay(TEST_DELAY);
  clear7seg(display);
  delay(TEST_DELAY);
}