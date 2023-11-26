#include "rfid_manager.h"

static MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;
String tagID = "";
MFRC522::StatusCode status;

bool check_new_Card_Presence()
{
    // byte bufferATQA[10];
    // byte bufferSize[10];
    bool val;
    val = mfrc522.PICC_IsNewCardPresent();
    // if (val)
    // {
    //     val = mfrc522.PICC_WakeupA(bufferATQA, bufferSize);
    //     mfrc522.PICC_HaltA();
    // }
    return val;
}

void init_RFID()
{
    SPI.begin();
    mfrc522.PCD_Init();
    // Show details of PCD - MFRC522 Card Reader
    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    mfrc522.PCD_DumpVersionToSerial();

    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    //"Using key (for A and B):"
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
}

String getID_RFID()
{
    String TagID = "";
    mfrc522.PICC_ReadCardSerial();
    TagID = dump_byte_array_string(mfrc522.uid.uidByte, mfrc522.uid.size);
    return TagID;
}

void dump_byte_array(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
    Serial.println();
}

String dump_byte_array_string(byte *buffer, byte bufferSize)
{
    // String text = String(bufferSize)+"HEX: ";
    String hex_text = "";
    for (byte i = 0; i < bufferSize; i++)
    {
        hex_text += String(buffer[i] < 0x10 ? " 0" : " ");
        hex_text += String(buffer[i], HEX);
    }

    return hex_text;
}

String getData_RFID()
{
    byte buffer[18];
    byte size = sizeof(buffer);
    byte trailerBlock = 7;
    byte sector = 1;

    // Authenticate using key A
    // Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return ""; // todo
    }

    // Show the whole sector as it currently is
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();

    status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    // Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    return dump_byte_array_string(buffer, 16);
}

bool setData_RFID(int value)
{
    if (value > 65535)
    {
        value = 65535;
    }
    bool flag = false;
    byte byteArray[4];
    byte trailerBlock = 7;

    splitIntegerToByteArray(value, byteArray);

    byte dataBlock[] = {
        byteArray[3], byteArray[2], byteArray[1], byteArray[0],
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff};

    // Authenticate using key B
    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));

    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        flag = true;
            return false;

    }
    if (!flag)
    {
        // Write data to the block
        Serial.print(F("Writing data into block "));
        Serial.print(blockAddr);
        Serial.println(F(" ..."));
        dump_byte_array(dataBlock, 16);
        status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);

        if (status != MFRC522::STATUS_OK)
        {
            Serial.print(F("MIFARE_Write() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return false;
        }
        else
            return true;
    }
    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
}

void splitIntegerToByteArray(uint32_t intValue, byte *byteArray)
{
    // Serial.print("converted value: ");
    for (int i = 0; i < 4; ++i)
    {
        // Use bitwise operations to extract each byte
        byteArray[i] = (intValue >> (i * 8)) & 0xFF;

        // Serial.print(byteArray[i], HEX);
    }
    Serial.println();
}

int getValue_RFID()
{
    byte buffer[18];
    byte size = sizeof(buffer);
    byte trailerBlock = 7;
    byte sector = 1;

    // Authenticate using key A
    // Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return 99; // todo
    }

    // // Show the whole sector as it currently is
    // Serial.println(F("Current data in sector:"));
    // mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    // Serial.println();

    status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    // Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    // dump_byte_array_string(buffer, 16);

    if (sizeof(buffer) >= 16)
    {
        byte bytearrayread[] = {buffer[3], buffer[2], buffer[1], buffer[0]};

        return combineByteArrayToValue(bytearrayread, sizeof(bytearrayread));
    }
    return 99;
}

void halt_RFID()
{
        mfrc522.PICC_HaltA();
        // Stop encryption on PCD
        mfrc522.PCD_StopCrypto1();
}

int combineByteArrayToValue(byte *bytes, int size)
{
    int result = 0;

    for (size_t i = size; i > 0; --i)
    {
        result = (result << 8) | bytes[i - 1];
        // Serial.println(bytes[i],HEX);
    }
    return result;
}