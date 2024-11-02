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
// SPI Pins
#define SS_PIN 5    // SDA/SS Pin for SPI
#define RST_PIN 17  // Reset Pin for SPI
#define SCK_PIN 18  // SCK Pin for SPI
#define MOSI_PIN 23 // MOSI Pin for SPI
#define MISO_PIN 19 // MISO Pin for SPI

// LED Pins
#define SCAN_LED 33       // LED Pin
#define CONNECTION_LED 27 // LED Pin

// Buzzer Pin
#define BUZZER_PIN 25 // Buzzer Pin

// Button Pins
#define DECREASE_BUTTON 15 // Button Pin -
#define INCREASE_BUTTON 2  // Button Pin +

// Temperature Sensor Pin
#define TEMP_SENSOR 26 // Analog Pin
#define DHTTYPE DHT11  // DHT 11 type sensor

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
DHT dht(TEMP_SENSOR, DHTTYPE);    // Create DHT instance

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Flag to signal when card is detected
volatile bool cardDetected = false;

// Variable to store the card UID in hexadecimal format (e.g. "DEADBEEF")
String cardUID = "";

// Variables for adjusting the wifi check function
unsigned long lastReconnectAttempt = 0;        // Global variable to track last reconnection attempt
const unsigned long reconnectInterval = 10000; // Interval to try reconnecting (in milliseconds)

// Server IP
String serverIP = "http://192.168.50.170";

// Menu variables
volatile int currentMenuOption = 0;                                   // Track the current menu option (0 = Clock In, 1 = Clock Out, 2 = Add Pitcher)
volatile int needToSelect = 1;                                        // Flag to indicate if a menu option needs to be selected
const char *menuOptions[] = {"Clock In", "Clock Out", "Add Pitcher"}; // Menu options array
volatile bool menuUpdated = false;                                    // Flag to indicate menu option changed

// Forward declarations of task functions
void rfidTask(void *pvParameters);
void feedbackTask(void *pvParameters);
void temperatureTask(void *pvParameters);
void checkWiFiConnection(void *pvParameters);
void sendDataToAPI(String dataType, String data1, String data2);
void menuTask(void *pvParameters);
void heartbeatTask(void *pvParameters);
void addressTask(void *pvParameters);

void setup()
{
  // Start serial communication
  Serial.begin(115200);
  Serial.println("Starting...");

  // Initialize SPI communication
  SPI.begin();
  Serial.println("SPI Initialized");

  // Initialize RFID reader
  mfrc522.PCD_Init();
  Serial.println("RFID Initialized");

  // Initialize DHT sensor
  dht.begin();
  Serial.println("DHT Initialized");

  // Initialize LED and buzzer pins as outputs
  pinMode(SCAN_LED, OUTPUT);
  digitalWrite(SCAN_LED, LOW);
  pinMode(CONNECTION_LED, OUTPUT);
  digitalWrite(CONNECTION_LED, LOW);
  pinMode(BUZZER_PIN, OUTPUT);

  // Intialize buttons as inputs
  pinMode(DECREASE_BUTTON, INPUT_PULLUP);
  pinMode(INCREASE_BUTTON, INPUT_PULLUP);

  // Create a Wi-Fi Manager object
  WiFiManager wifiManager;

  // This line resets saved Wi-Fi credentials
  // wifiManager.resetSettings();

  // Changes the theme to dark mode
  wifiManager.setClass("invert");

  // Automatically connect, or go to config portal if not connected
  if (!wifiManager.autoConnect("BeertrackerHUB™️ Wifi Portal", "3opMujJtembh6"))
  {
    Serial.println("Failed to connect and hit timeout");
    ESP.restart();
  }

  // If you get here you have connected to the WiFi
  Serial.println("Connected to Wi-Fi");
  digitalWrite(CONNECTION_LED, HIGH);

  // 3 tone buzzer with high tone beeps
  tone(BUZZER_PIN, 1000); // Play tone C4
  delay(100);             // Delay 100ms
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 784); // Play tone G4
  delay(100);            // Delay 100ms
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 523); // Play tone C5
  delay(100);            // Delay 100ms
  noTone(BUZZER_PIN);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(HIGH);

  // Beertracker HUB on LCD
  lcd.setCursor(0, 0);
  lcd.print("Beertracker HUB");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select option");
  lcd.setCursor(0, 1);
  lcd.print("Press buttons");

  // Multithreading setup for RFID and feedback tasks
  xTaskCreate(rfidTask, "RFID Task", 10000, NULL, 1, NULL);               // Task for RFID scanning
  xTaskCreate(feedbackTask, "Feedback Task", 10000, NULL, 2, NULL);       // Task for feedback (LED/Buzzer)
  xTaskCreate(temperatureTask, "Temperature Task", 10000, NULL, 3, NULL); // Task for temperature readings
  xTaskCreate(menuTask, "Menu Task", 10000, NULL, 2, NULL);               // Task for the menu system
  xTaskCreate(heartbeatTask, "Heartbeat Task", 10000, NULL, 4, NULL);     // Task for heartbeat
  xTaskCreate(addressTask, "Address Task", 10000, NULL, 5, NULL);         // Task for address
}

void loop()
{
}

// Task to continuously scan for RFID cards
void rfidTask(void *pvParameters)
{
  Serial.println("RFID Task Started");
  while (true)
  {
    // Check if a new card is present
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {
      // Store the card UID
      cardUID = "";
      for (byte i = 0; i < mfrc522.uid.size; i++)
      {
        cardUID += String(mfrc522.uid.uidByte[i], HEX);
      }
      cardUID.toUpperCase();
      Serial.println("Card detected: " + cardUID);

      cardDetected = true; // Set flag to indicate card is detected for feedback task

      // Send the card UID to the API and get the username
      sendDataToAPI("card", cardUID, String(currentMenuOption));

      // Halt the PICC to prevent repeated scans
      mfrc522.PICC_HaltA();
    }

    // Delay between checks
    vTaskDelay(100 / portTICK_PERIOD_MS); // 100 ms delay for task switching
  }
}

// Task to handle feedback (LED and Buzzer) when a card is detected
void feedbackTask(void *pvParameters)
{
  while (true)
  {
    if (cardDetected)
    {
      // Turn on the LED
      digitalWrite(SCAN_LED, HIGH);

      // Play a sound for feedback
      tone(BUZZER_PIN, 523); // Play tone C4
      vTaskDelay(100);
      noTone(BUZZER_PIN);
      tone(BUZZER_PIN, 784); // Play tone G4
      vTaskDelay(100);
      noTone(BUZZER_PIN);

      // Turn off the LED
      digitalWrite(SCAN_LED, LOW);

      // Keep the feedback visible for a short period
      vTaskDelay(500);

      cardDetected = false; // Reset flag after feedback is given
    }

    // Delay between checks
    vTaskDelay(10 / portTICK_PERIOD_MS); // 10 ms delay for task switching
  }
}

void menuTask(void *pvParameters)
{
  Serial.println("Menu Task Started");

  // Variable to track the last selection time
  unsigned long lastSelectionTime = 0;
  const unsigned long selectionDisplayDuration = 3000; // 3 seconds in milliseconds
  bool selectionActive = false;                        // Flag to check if an option is selected

  while (true)
  {
    // Check if LEFT button is pressed
    if (digitalRead(DECREASE_BUTTON) == LOW)
    {
      Serial.println("Minus (LEFT) button pressed");
      needToSelect = 0;
      currentMenuOption--;
      if (currentMenuOption < 0)
      {
        currentMenuOption = 2; // Wrap around to the last option (Add Pitcher)
      }
      menuUpdated = true;
      vTaskDelay(150 / portTICK_PERIOD_MS); // Moderate debounce delay
    }

    // Check if RIGHT button is pressed
    if (digitalRead(INCREASE_BUTTON) == LOW)
    {

      Serial.println("Plus (RIGHT) button pressed");
      needToSelect = 0;
      currentMenuOption++;
      if (currentMenuOption > 2)
      {
        currentMenuOption = 0; // Wrap around to the first option (Clock In)
      }
      menuUpdated = true;
      vTaskDelay(150 / portTICK_PERIOD_MS); // Moderate debounce delay
    }

    // If menu updated, show the new option on the LCD
    if (menuUpdated)
    {
      tone(BUZZER_PIN, 1000); // Play a tone to indicate menu change
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Option:");
      lcd.setCursor(0, 1);
      lcd.print(menuOptions[currentMenuOption]);
      vTaskDelay(100);
      noTone(BUZZER_PIN);

      // Mark the selection as active and record the time
      selectionActive = true;
      lastSelectionTime = millis();
      menuUpdated = false;
    }

    // Check if selection is active and duration has passed
    if (selectionActive && (millis() - lastSelectionTime >= selectionDisplayDuration))
    {
      lcd.clear();
      vTaskDelay(250);             // Clear the LCD after the duration
      // Display selected option on the LCD
      lcd.setCursor(0, 0);
      lcd.print("Selected:");
      lcd.setCursor(0, 1);
      lcd.print(menuOptions[currentMenuOption]);
      
      selectionActive = false; // Reset selection state
    }

    // Small delay before the next loop iteration
    vTaskDelay(50 / portTICK_PERIOD_MS); // Keep it responsive
  }
}

// Task to continuously read the temperature
void temperatureTask(void *pvParameters)
{
  while (true)
  {
    // Read temperature from DHT sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Check if the reading is valid
    if (isnan(temperature))
    {
      Serial.println("Failed to read from DHT sensor!");
    }
    else
    {
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
    vTaskDelay(60000 / portTICK_PERIOD_MS);
  }
}

// Function to check heartbeat
void heartbeatTask(void *pvParameters)
{
  Serial.println("Heartbeat Task Started");
  while (true)
  {
    String heartbeat = "Active";
    Serial.print("Checking ESP32 heartbeat... ");
    sendDataToAPI("heartbeat", heartbeat, "");

    // Delay for 15 seconds (15000 ms)
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}

void addressTask(void *pvParameters)
{
  Serial.println("Address Task Started");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Waiting for WiFi connection...");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
  }

  Serial.println("Locating address...");
  String ipAddress = WiFi.localIP().toString();
  Serial.println("IP Address: " + ipAddress);
  String macAddress = WiFi.macAddress();
  Serial.println("MAC Address: " + macAddress);

  sendDataToAPI("address", ipAddress, macAddress);
  vTaskDelete(NULL);
}

// Function to send data to API
void sendDataToAPI(String dataType, String data1, String data2)
{
  if (WiFi.status() == WL_CONNECTED)
  { // Check WiFi connection status
    HTTPClient http;
    http.setTimeout(5000); // Set a 5-second timeout for the request

    http.begin(serverIP + "/php/api.php");                               // Specify the URL of the API endpoint
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Set the POST content type

    String postData = "";

    // Check the data type and prepare the POST data
    if (dataType == "temperature")
    {
      Serial.println("Sending temperature and humidity data to API");
      // Send temperature and humidity data
      postData = "type=temperature&temperature=" + data1 + "&humidity=" + data2;
    }
    else if (dataType == "card")
    {
      Serial.println("Sending card UID data to API");
      // Send card UID data
      if (needToSelect == 1)
      {
        Serial.println("No option selected");
        // Show on the LCD that they have to select an option
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("No option selected");
        lcd.setCursor(0, 1);
        lcd.print("Press buttons");
        delay(2000); // Wait 2 seconds
        lcd.clear();
      } else {
        postData = "type=card&uid=" + data1 + "&option=" + data2;
        // Reset selected option after sending the card data
        needToSelect = 1;
      }
    }
    else if (dataType == "connection")
    {
      postData = "type=connection&status=" + data1;
    }
    else if (dataType == "heartbeat")
    {
      Serial.println("Sending ESP32 heartbeat to API");
      postData = "type=heartbeat&status=" + data1;
    }
    else if (dataType == "address")
    {
      Serial.println("Sending ESP32 address to API");
      postData = "type=address&ip=" + data1 + "&mac=" + data2;
    }

    // Send the POST request
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0)
    {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // If card data is being sent, handle the response
      if (dataType == "card" )
      {
        String response = http.getString();
        Serial.println("Response: " + response);

        // Parse the JSON response to get the username
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, response);
        if (!error)
        {
          const char *status = doc["status"];
          if (strcmp(status, "success") == 0)
          {
            const char *userName = doc["name"];
            Serial.print("Username: ");
            Serial.println(userName);

            // Display the username on the LCD
            lcd.clear();
            if (currentMenuOption == 0)
            { // Clock In
              lcd.print("Welcome,");
            }
            else if (currentMenuOption == 1)
            { // Clock Out
              lcd.print("Bye,");
            }
            else if (currentMenuOption == 2)
            { // Add Pitcher
              lcd.print("+1 Pitcher,");
            }
            lcd.setCursor(0, 1);
            lcd.print(userName); // Display the username on the second line

            // Keep the text visible for a short period
            delay(2000); // Wait 2 seconds
            lcd.clear();

            // Show Beertracker HUB on LCD and select option
            lcd.setCursor(0, 0);
            lcd.print("Beertracker HUB");
            lcd.setCursor(0, 1);
            lcd.print("Select option");
          }
          else
          {
            // Handle various error statuses
            const char *message = doc["message"];
            Serial.println("Error: " + String(message));

            // // Display the appropriate error message on the LCD
            // lcd.clear();
            // lcd.setCursor(0, 0);
            // lcd.print("Error:");
            // lcd.setCursor(0, 1);
            // lcd.print(message); // Display the error message

            // // Keep the text visible for a short period
            // delay(2000); // Wait 2 seconds
            // lcd.clear();

            if (strcmp(message, "Card not found") == 0)
            {
              // Say card not found on the LCD
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Card not found");
              lcd.setCursor(0, 1);
              lcd.print("Add card to acc");
              delay(2000); // Wait 2 seconds
          
              // Show cardUID on lcd
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Card UID:");
              lcd.setCursor(0, 1);
              lcd.print(cardUID);
            }

            if (strcmp(message, "Invalid type or missing data") == 0)
            {
              // Show Beertracker HUB on LCD and select option
              lcd.setCursor(0, 0);
              lcd.print("Beertracker HUB");
              lcd.setCursor(0, 1);
              lcd.print("Select option");
            }

            if (strcmp(message, "User already checked in") == 0)
            {
              // Say user already checked in on the LCD
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("User already");
              lcd.setCursor(0, 1);
              lcd.print("checked in");
              delay(2000); // Wait 2 seconds
              lcd.clear();

              // Show Beertracker HUB on LCD and select option
              lcd.setCursor(0, 0);
              lcd.print("Beertracker HUB");
              lcd.setCursor(0, 1);
              lcd.print("Select option");
            }

            if (strcmp(message, "User already checked out") == 0)
            {
              // Say user already checked out on the LCD
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("User already");
              lcd.setCursor(0, 1);
              lcd.print("checked out");
              delay(2000); // Wait 2 seconds
              lcd.clear();

              // Show Beertracker HUB on LCD and select option
              lcd.setCursor(0, 0);
              lcd.print("Beertracker HUB");
              lcd.setCursor(0, 1);
              lcd.print("Select option");
            }

            if (strcmp(message, "User already checked out, cannot add pitcher") == 0) {
              // Say user already checked out on the LCD
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("No pitcher");
              lcd.setCursor(0, 1);
              lcd.print("User checkd out");
              delay(2000); // Wait 2 seconds
              lcd.clear();

              // Show Beertracker HUB on LCD and select option
              lcd.setCursor(0, 0);
              lcd.print("Beertracker HUB");
              lcd.setCursor(0, 1);
              lcd.print("Select option");
            }
          }
        }
        else
        {
          Serial.println("Failed to parse response");
          // Handle parsing error
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Parse Error");
          delay(2000); // Wait 2 seconds
          lcd.clear();
        }
      }
    }
    else
    {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      // Display error on the LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("HTTP Error:");
      lcd.setCursor(0, 1);
      lcd.print(httpResponseCode);
      delay(2000); // Wait 2 seconds
      lcd.clear();
    }

    http.end(); // End the HTTP connection to free up resources
  }
  else
  {
    Serial.println("Error: WiFi not connected");
    // Display WiFi error on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Error");
    delay(2000); // Wait 2 seconds
    lcd.clear();
  }
}