#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 15
#define RST_PIN 2

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{ Serial.begin (115200);
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
}

void loop()
{
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  Serial.print("UID:");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  { //Show UID on serial monitor
    Serial.print(mfrc522.uid.uidByte[i], HEX); // Serial.print("UID tag :");
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
}