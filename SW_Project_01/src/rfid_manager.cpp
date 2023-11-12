#include "rfid_manager.h"

static MFRC522 mfrc522(SS_PIN, RST_PIN);
String tagID = "";

bool check_presence()
{
    //
    // Getting ready for Reading PICCs
    // If a new PICC placed to RFID reader continue
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return false;
    }
    //
    // Since a PICC placed get Serial and continue
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return false;
    }
    //
    tagID = "";
    // The MIFARE PICCs that we use have 4 byte UID
    for (uint8_t i = 0; i < 4; i++)
    {
        // readCard[i] = mfrc522.uid.uidByte[i];
        //  Adds the 4 bytes in a single String variable
        tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    //
    tagID.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading
    return true;
    //
}

void init_RFID()
{
    mfrc522.PCD_Init();
    // Show details of PCD - MFRC522 Card Reader
    mfrc522.PCD_DumpVersionToSerial();
}
