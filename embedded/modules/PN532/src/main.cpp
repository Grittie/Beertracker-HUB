#include <Wire.h>
#include <Adafruit_PN532.h>

// Define I2C pins for ESP32 (can be adjusted if needed)
#define SDA_PIN 21
#define SCL_PIN 22

// Create an instance of the PN532 library
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Looking for PN532...");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Didn't find PN532 board");
    while (1); // Halt the program if no PN532 is found
  }

  // Configure the PN532 to read RFID cards
  nfc.SAMConfig();

  Serial.println("Waiting for an NFC card...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the UID
  uint8_t uidLength;

  // Try to read a card
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display the UID
    Serial.print("Card detected! UID Length: "); 
    Serial.print(uidLength); 
    Serial.print(" bytes. UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX); // Print UID in hexadecimal
      Serial.print(" ");
    }
    Serial.println();

    // Wait a bit before looking for another card
    delay(1000);
  }
}
