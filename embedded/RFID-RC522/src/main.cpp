#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 15
#define RST_PIN 2
#define LED_PIN 3

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();          // Initiate SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  digitalWrite(LED_PIN, LOW); // Ensure LED is off initially
}

bool cardScanned = false;  // Flag to check if card has been scanned
bool cardPresent = false;  // Flag to check if card is in proximity

void loop() {
  // Check if a new card is present
  if (!mfrc522.PICC_IsNewCardPresent()) {
    // If no card is present, reset the flags and turn off the LED
    if (cardPresent) {
      Serial.println("Card removed");
      cardPresent = false;
      cardScanned = false;
      digitalWrite(LED_PIN, LOW); // Turn off the LED
    }
    return;
  }

  // If a card is present but has already been scanned, exit the loop to avoid spamming
  if (cardScanned) {
    return;
  }

  // Read the card's serial number (UID)
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Card is detected and scanned
  Serial.println("Card detected");
  Serial.print("UID: ");
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();  // Add a newline for readability
  content.toUpperCase();

  // Set the flags to indicate that the card has been scanned and is present
  cardScanned = true;
  cardPresent = true;
  
  // Turn on the LED to indicate the card is being processed/
  // Keep led on for a seconds
  digitalWrite(LED_PIN, HIGH);
  delay(1000);

  // Halt PICC to prevent repeated scans until the card is removed
  mfrc522.PICC_HaltA();
}