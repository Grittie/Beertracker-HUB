#include <Wire.h>
#include <Adafruit_PN532.h>
#include <LiquidCrystal_I2C.h> // Library for the I2C LCD

// I2C Pins for ESP32
#define SDA_PIN 21
#define SCL_PIN 22

// Create an instance for PN532 and LCD
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to your LCD's I2C address

void setup(void) {
  Serial.begin(115200);
  Serial.println("Looking for PN532...");

  nfc.begin();
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the LCD backlight

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Didn't find PN532 board");
    while (1); // Halt if no PN532 is found
  }

  nfc.SAMConfig(); // Initialize PN532 to detect cards
  Serial.println("Waiting for an NFC card...");
  lcd.setCursor(0, 0);
  lcd.print("Waiting for card...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[7]; // Buffer to store the UID
  uint8_t uidLength;

  // Try to read a card
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display the UID in Serial Monitor
    Serial.print("Card detected! UID: ");
    lcd.clear(); // Clear the LCD before showing the UID
    lcd.setCursor(0, 0);
    lcd.print("Card UID:");
    lcd.setCursor(0, 1);
    
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX); // Print UID in HEX format
      lcd.print(uid[i], HEX);    // Show UID on LCD
      lcd.print(" ");
    }
    Serial.println();
    delay(3000); // Wait before looking for another card
  }
}
