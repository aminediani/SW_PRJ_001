#include <Arduino.h>
#include "display7seg_manager.h"
#include "LCD_manager.h"
#include "rfid_manager.h"

#define TEST_DELAY 500

// Function declaration
void dig7seg_test();
void crystal_Test();
void I2C_Scanner(int address);
void RFID_test();

static int counter = 0;
bool tagPresence = 0;
int inByte = 11211;
// Setup function
void setup() {
  
  Serial.begin(9600);
  Serial.println("Start...");
  delay(1000);

  init_LCD();
  init_7seg();
  init_RFID();
  Serial.println("Init Done...");


}

//Main Loop
void loop() {

    Serial.println("loop...");
    delay(100);
    dig7seg_test();
    crystal_Test();
    RFID_test();
}
void RFID_test(){
  bool tagPresence = 0;

  tagPresence = check_new_Card_Presence();
  if(tagPresence){
  Serial.println("Tag is present ");
  Serial.println(getID_RFID());
  Serial.println(getData_RFID());
  Serial.print("the returned value is = ");
  Serial.println(getValue_RFID());

  delay(1000);
    setData_RFID(inByte);
  }
  
  

}

// digital 7 seg Test
void dig7seg_test(){
  float value = 12.34f+((float)counter)*0.1;
  showValue7seg(value);
  delay(TEST_DELAY);
  //clear7seg();
  //delay(TEST_DELAY);
}

// LCD crystal test
void crystal_Test(){
  showTextLine1("CNT = " + String(counter));
  delay(TEST_DELAY);
  counter++;

}

//I2C 
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