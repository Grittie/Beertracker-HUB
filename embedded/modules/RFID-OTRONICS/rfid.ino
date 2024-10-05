#include <SPI.h>           // voor SPI-communicatie
#include <MFRC522.h>       // voor de MFRC522 NFC-module

#define SS_PIN 10
#define RST_PIN 9
String UW_UID = "6A560580";

MFRC522 mfrc522(SS_PIN, RST_PIN); // Maak een MFRC522 instantie aan

void setup() {
Serial.begin(9600); // Initialiseer seriële communicatie
SPI.begin(); // Initialiseer de SPI bus
mfrc522.PCD_Init(); // Initialiseer MFRC522
Serial.println("Klaar om NFC-tags te lezen...");
}

void loop() {
// Zoek naar nieuwe kaarten
if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
// Toon UID op de seriële monitor
Serial.print("UID tag :");
String content = "";
byte letter;
for (byte i = 0; i < mfrc522.uid.size; i++) {
Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
Serial.print(mfrc522.uid.uidByte[i], HEX);
content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
content.concat(String(mfrc522.uid.uidByte[i], HEX));
}
Serial.println();
Serial.print("Message : ");
content.toUpperCase();
if (content == UW_UID)
{
Serial.println("Kaart of tag geaccepteerd");
// Doe iets wanneer de geautoriseerde tag gedetecteerd is
}
else {
Serial.println("Kaart of tag geweigerd");
}
delay(1000);
}
}
