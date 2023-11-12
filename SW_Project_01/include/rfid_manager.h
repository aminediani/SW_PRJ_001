#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 7



bool check_presence();
void init_RFID();