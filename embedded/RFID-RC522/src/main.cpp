#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10        // SDA/SS Pin
#define RST_PIN 41       // Reset Pin
#define LED_PIN 18       // LED Pin
#define BUZZER_PIN 16    // Buzzer Pin

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin(36, 37, 35, 10);    // SCK = 36, MISO = 37, MOSI = 35, SS = 10
  mfrc522.PCD_Init();           // Initialize MFRC522
  pinMode(LED_PIN, OUTPUT);     // Set LED pin as output
  digitalWrite(LED_PIN, LOW);   // Ensure LED is off initially
  pinMode(BUZZER_PIN, OUTPUT);  // Buzzer pin as output
  tone(BUZZER_PIN, 1000, 500); // Play a tone to indicate the system is ready
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

  String content = "";  // Initialize an empty string to store the UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    // Print each byte of the UID to the serial monitor in hexadecimal format
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    // Concatenate the UID byte to the content string, formatting it properly
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  content.toUpperCase();

  // Set the flags to indicate that the card has been scanned and is present
  cardScanned = true;
  cardPresent = true;
  
  // Turn on the LED to indicate the card is being processed
  // Keep LED on for a second
  // Turn on Buzzer
  tone(BUZZER_PIN, 523); // C4
  delay(100);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 784); // G4
  delay(100);
  noTone(BUZZER_PIN);
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  
  // Halt PICC to prevent repeated scans until the card is removed
  mfrc522.PICC_HaltA();
}
