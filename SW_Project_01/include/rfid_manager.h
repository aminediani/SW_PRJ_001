#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 7
#define blockAddr 4


bool check_new_Card_Presence();

void init_RFID();

String getID_RFID();

void dump_byte_array(byte *buffer, byte bufferSize);

String dump_byte_array_string(byte *buffer, byte bufferSize);
String getData_RFID();
void setData_RFID(int value);
void splitIntegerToByteArray(uint32_t intValue, byte* byteArray);