#include <TM1637Display.h>
#include <Wire.h>
#include <Arduino.h>

#define CLK_7seg A0
#define DIO_7seg A1



void showValue7seg( float num);
void clear7seg();
void init_7seg();