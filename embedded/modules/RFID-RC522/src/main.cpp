#include <SPI.h>
#include <MFRC522.h>

// Pin definitions
#define SS_PIN 10        // SDA/SS Pin for SPI
#define RST_PIN 41       // Reset Pin for RFID
#define LED_PIN 18       // LED Pin
#define BUZZER_PIN 16    // Buzzer Pin
#define BUTTON_PIN 20    // Button Pin for resetting RFID

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Flag to signal when card is detected
volatile bool cardDetected = false;  
// Variable to store the card UID in hexadecimal format (e.g. "DEADBEEF") 
String cardUID = ""; 

// Forward declarations of task functions
void rfidTask(void * pvParameters);
void feedbackTask(void * pvParameters);
void checkButtonAndResetRFID();

void setup() {
  Serial.begin(115200);           // Start serial communication for debugging
  SPI.begin(36, 37, 35, 10);      // Initialize SPI with SCK, MISO, MOSI, and SS
  mfrc522.PCD_Init();             // Initialize RFID module

  Serial.println("RFID Reader Initialized");
  // Initialize LED and buzzer pins as outputs
  pinMode(LED_PIN, OUTPUT);       
  digitalWrite(LED_PIN, LOW);     
  pinMode(BUZZER_PIN, OUTPUT);    

  // Multithreading setup for RFID and feedback tasks
  xTaskCreate(rfidTask, "RFID Task", 10000, NULL, 1, NULL);   // Task for RFID scanning
  Serial.println("RFID Task Created");
  xTaskCreate(feedbackTask, "Feedback Task", 10000, NULL, 1, NULL); // Task for feedback (LED/Buzzer)
  Serial.println("Feedback Task Created");
}

void loop() {
  checkButtonAndResetRFID(); 
}

// Task to continuously scan for RFID cards
void rfidTask(void * pvParameters) {
  Serial.println("RFID Task Started");
  while (true) {
    // Check if a new card is present
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      // Store the card UID
      cardUID = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        cardUID += String(mfrc522.uid.uidByte[i], HEX);
      }
      cardUID.toUpperCase();
      Serial.println("Card detected: " + cardUID);

      // Signal that card has been selected for feedbackTask
      cardDetected = true;
      
      // Halt the PICC to prevent repeated scans
      mfrc522.PICC_HaltA();
    }

    // Delay between checks
    vTaskDelay(100 / portTICK_PERIOD_MS);  // 100 ms delay for task switching
  }
}

// Task to handle feedback (LED and Buzzer) when a card is detected
void feedbackTask(void * pvParameters) {
  while (true) {
    if (cardDetected) {
      // Turn on the LED
      digitalWrite(LED_PIN, HIGH);

      // Play a sound
      // TODO: Improve this code.
      tone(BUZZER_PIN, 523);  // Play tone C4
      delay(100);             // Delay for 100 ms
      noTone(BUZZER_PIN);
      tone(BUZZER_PIN, 784);  // Play tone G4
      delay(100);
      noTone(BUZZER_PIN);

      // Turn on the LED
      digitalWrite(LED_PIN, LOW);
      cardDetected = false;  // Reset flag after feedback is given
    }
        
    // Delay between checks
    vTaskDelay(10 / portTICK_PERIOD_MS);  // 10 ms delay for task switching
  }
}

// Function to check if the button is pressed and reset the RFID
void checkButtonAndResetRFID() {
  if (digitalRead(BUTTON_PIN) == HIGH) {  // Button pressed (active LOW)
    Serial.println("Resetting RFID scanner...");
    mfrc522.PCD_Init();  // Reinitialize the RFID module
    Serial.println("RFID scanner reset.");
    delay(1000);  // Debounce delay to prevent multiple resets
  }
}