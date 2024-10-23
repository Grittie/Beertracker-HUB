#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         17          // Configurable, see wiring above
#define SS_PIN          5           // Configurable, see wiring above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

void setup() {
  Serial.begin(115200);   // Initialize serial communications
  SPI.begin();            // Init SPI bus
  mfrc522.PCD_Init();     // Init MFRC522
  Serial.println("Scan a RFID card to see its UID:");
}

void loop() {
  // Look for a new card
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Print UID in hex format
  Serial.print("UID tag: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Halt PICC (stop reading the card)
  mfrc522.PICC_HaltA();
}
