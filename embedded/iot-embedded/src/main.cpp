#include <WiFiManager.h>
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


// Pin definitions
#define SS_PIN 10           // SDA/SS Pin for SPI
#define RST_PIN 41          // Reset Pin for RFID
#define SCAN_LED 18         // LED Pin
#define CONNECTION_LED 15   // LED Pin
#define BUZZER_PIN 16       // Buzzer Pin
#define DECREASE_BUTTON 20  // Button Pin
#define INCREASE_BUTTON 21  // Button Pin
#define TEMP_SENSOR 17       // Analog Pin
#define DHTTYPE DHT11       // DHT 11 type sensor


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
DHT dht(TEMP_SENSOR, DHTTYPE);    // Create DHT instance

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Flag to signal when card is detected
volatile bool cardDetected = false;

// Variable to store the card UID in hexadecimal format (e.g. "DEADBEEF") 
String cardUID = ""; 

// Forward declarations of task functions
void rfidTask(void * pvParameters);
void feedbackTask(void * pvParameters);
void temperatureTask(void * pvParameters);
void sendDataToAPI(String dataType, String data1, String data2);
String getUserNameFromAPI(String cardUID);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Set the onboard LED pin as output
  digitalWrite(LED_BUILTIN, LOW); // Turn off the LED
  
  // Start serial communication
  Serial.begin(115200);
  SPI.begin(36, 37, 35, 10);      // Initialize SPI with SCK, MISO, MOSI, and SS
  mfrc522.PCD_Init();            // Initialize RFID module
  dht.begin();                   // Initialize DHT sensor

  // Initialize LED and buzzer pins as outputs
  pinMode(SCAN_LED, OUTPUT);       
  digitalWrite(SCAN_LED, LOW);    
  pinMode(CONNECTION_LED, OUTPUT);       
  digitalWrite(CONNECTION_LED, LOW);          
  pinMode(BUZZER_PIN, OUTPUT);  

  // Create a Wi-Fi Manager object
  WiFiManager wifiManager;

  // This line resets saved Wi-Fi credentials
  // wifiManager.resetSettings();
  
  // Changes the theme to dark mode
  wifiManager.setClass("invert");

  // Automatically connect, or go to config portal if not connected
  if (!wifiManager.autoConnect("BeertrackerHUB™️ Wifi Portal", "3opMujJtembh6")) {
    Serial.println("Failed to connect and hit timeout");
    ESP.restart();
  }

  // If you get here you have connected to the WiFi
  Serial.println("Connected to Wi-Fi");
  digitalWrite(CONNECTION_LED, HIGH);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);

  // Multithreading setup for RFID and feedback tasks
  xTaskCreate(rfidTask, "RFID Task", 10000, NULL, 1, NULL);   // Task for RFID scanning
  xTaskCreate(feedbackTask, "Feedback Task", 10000, NULL, 2, NULL); // Task for feedback (LED/Buzzer)
  xTaskCreate(temperatureTask, "Temperature Task", 10000, NULL, 3, NULL); // Task for temperature readings
}

void loop() {
}

// Task to continuously scan for RFID cards
void rfidTask(void * pvParameters) {

  // Perform a self-test to check if the RFID module is working
  if (!mfrc522.PCD_PerformSelfTest()) {
  Serial.println("RFID Self-test failed!");
  } else {
    Serial.println("RFID Self-test passed!");
  }


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
      digitalWrite(SCAN_LED, HIGH);

      // Send the card UID to the API to get the user name
      String userName = getUserNameFromAPI(cardUID);

      if (userName != "") {
        // Display the user's name on the LCD
        lcd.setCursor(0, 0);
        lcd.print("User: " + userName);
      } else {
        // If user name is not found, display the card UID instead
        lcd.setCursor(0, 0);
        lcd.print("UID:" + cardUID);
      }

      // Play a sound
      tone(BUZZER_PIN, 523);  // Play tone C4
      delay(100);             
      noTone(BUZZER_PIN);
      tone(BUZZER_PIN, 784);  // Play tone G4
      delay(100);
      noTone(BUZZER_PIN);

      // Turn off the LED
      digitalWrite(SCAN_LED, LOW);

      // Keep the text visible for a short period
      delay(2000);  // Wait 2 seconds

      // Clear the LCD
      lcd.clear();
      cardDetected = false;  // Reset flag after feedback is given
    }
        
    // Delay between checks
    vTaskDelay(10 / portTICK_PERIOD_MS);  // 10 ms delay for task switching
  }
}


// Task to continuously read the temperature
void temperatureTask(void * pvParameters) {
  while (true) {
    // Read temperature from DHT sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    // Check if the reading is valid
    if (isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println("°C");
      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println("%");

      // Send temperature and humidity to API
      sendDataToAPI("temperature", String(temperature), String(humidity));
    }
    
    // Delay for 30 seconds (30000 ms)
    vTaskDelay(30000 / portTICK_PERIOD_MS);
  }
}

// Function to send data to API
void sendDataToAPI(String dataType, String data1, String data2) {
  if (WiFi.status() == WL_CONNECTED) { // Check WiFi connection status
    HTTPClient http;
    http.begin("http://145.92.189.155/php/api.php");  // Specify the URL
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Set the POST content type

    String postData = "";

    if (dataType == "temperature") {
      // Send temperature and humidity data
      postData = "type=temperature&temperature=" + data1 + "&humidity=" + data2;
    } else if (dataType == "card") {
      // Send card UID data
      postData = "type=card&uid=" + data1;
    } else {
      Serial.println("Invalid data type");
      return;
    }

    // Send the POST request
    int httpResponseCode = http.POST(postData);

    http.end();  // End the HTTP connection
  } else {
    Serial.println("Error: WiFi not connected");
  }
}


String getUserNameFromAPI(String cardUID) {
  if (WiFi.status() == WL_CONNECTED) { // Check WiFi connection status
    HTTPClient http;
    http.begin("http://145.92.189.155/php/api.php");  // Specify the URL to your API
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Set the POST content type

    String postData = "type=card&uid=" + cardUID;

    int httpResponseCode = http.POST(postData);
    
    String payload = ""; // Variable to store the API response
    if (httpResponseCode == 200) { // If the request was successful
      payload = http.getString();  // Get the response from the server
      Serial.println(payload); // For debugging purposes

      // Parse the JSON response
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);
      if (!error) {
        if (doc["status"] == "success") {
          return doc["name"].as<String>(); // Return the user's name
        }
      }
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();  // End the HTTP connection
  } else {
    Serial.println("Error: WiFi not connected");
  }

  return "";  // Return an empty string if no user name is found or an error occurs
}
