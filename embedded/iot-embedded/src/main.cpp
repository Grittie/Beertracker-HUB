/*
 * MIT License
 * 
 * Copyright (c) [2024] Lars Grit
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file main.cpp
 * @brief Main source file for the Beertracker HUB™️ project
 * 
 * This code is part of the Beertracker HUB™️ project, developed for the IoT Thematic semester 
 * at the HvA. It includes key functionalities such as:
 * - RFID card scanning
 * - Temperature and humidity monitoring
 * - Wi-Fi connectivity
 * - API server interaction
 * 
 * Components and libraries utilized in this project include:
 * - WiFiManager
 * - MFRC522 (RFID sensor)
 * - DHT sensor (temperature and humidity)
 * - LiquidCrystal_I2C (LCD display)
 * - ESPAsyncWebServer
 * 
 * @note Designed for ESP32 microcontrollers.
 * 
 * @author Lars Grit
 */

#include <WiFiManager.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <base64.h>
#include "config.h"
#include <U8g2lib.h>

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

// Initialize the OLED display using U8g2
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 21);

// Create an instance of the server
AsyncWebServer server(80);

// Flag to signal when card is detected
volatile bool cardDetected = false;

// Variable to store the card UID in hexadecimal format (e.g. "DEADBEEF")
String cardUID = "";

// Variables for adjusting the wifi check function
unsigned long lastReconnectAttempt = 0;        // Global variable to track last reconnection attempt
const unsigned long reconnectInterval = 10000; // Interval to try reconnecting (in milliseconds)

// Server IP
String serverIP = SERVER_URL;

// Menu variables
volatile int currentMenuOption = -1;                                   // Track the current menu option (0 = Clock In, 1 = Clock Out, 2 = Add Pitcher)
volatile int needToSelect = 1;                                        // Flag to indicate if a menu option needs to be selected
const char *menuOptions[] = {"Clock In", "Clock Out", "Add Pitcher"}; // Menu options array
volatile bool menuUpdated = false;                                    // Flag to indicate menu option changed
volatile bool readyToScan = false;

// Forward declarations of task functions
void rfidTask(void *pvParameters);
void feedbackTask(void *pvParameters);
void temperatureTask(void *pvParameters);
void checkWiFiConnection(void *pvParameters);
void sendDataToAPI(String dataType, String data1, String data2);
void menuTask(void *pvParameters);
void heartbeatTask(void *pvParameters);
void addressTask(void *pvParameters);
void httpServerTask(void *pvParameters);
void displayMenu(const char *line1, const char *line2);

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
  if (!wifiManager.autoConnect("Wifi portal please work", "3opMujJtembh6"))
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

  u8g2.begin(); // Initialize OLED
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, "Beertracker HUB");
  u8g2.drawStr(0, 30, "Initializing...");
  u8g2.sendBuffer();
  delay(2000);

  // Clear display and prompt for menu
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, "Select option");
  u8g2.drawStr(0, 30, "Press button.");
  u8g2.sendBuffer();

  // Multithreading setup for RFID and feedback tasks
  xTaskCreate(rfidTask, "RFID Task", 10000, NULL, 1, NULL);               // Task for RFID scanning
  xTaskCreate(feedbackTask, "Feedback Task", 10000, NULL, 2, NULL);       // Task for feedback (LED/Buzzer)
  xTaskCreate(temperatureTask, "Temperature Task", 10000, NULL, 3, NULL); // Task for temperature readings
  xTaskCreate(menuTask, "Menu Task", 10000, NULL, 2, NULL);               // Task for the menu system
  xTaskCreate(heartbeatTask, "Heartbeat Task", 10000, NULL, 4, NULL);     // Task for heartbeat
  xTaskCreate(addressTask, "Address Task", 10000, NULL, 5, NULL);         // Task for address
  xTaskCreate(httpServerTask, "HTTP Server Task", 10000, NULL, 5, NULL);  // Task for HTTP server
}

void loop()
{
}

void displayMenu(const char *line1, const char *line2)
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, line1);
  u8g2.drawStr(0, 30, line2);
  u8g2.sendBuffer();
}

// Task to continuously scan for RFID cards
void rfidTask(void *pvParameters)
{
  Serial.println("RFID Task Started");
  while (true)
  {
    // Check if a new card is present
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() && readyToScan)
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

      // Reset the flag to prevent repeated scans
      readyToScan = false;
      
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
  const unsigned long selectionDisplayDuration = 1000;
  bool selectionActive = false;                       

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
      displayMenu("Option:", menuOptions[currentMenuOption]);
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
      u8g2.clearBuffer();
      vTaskDelay(250);             // Clear the LCD after the duration
      // Display selected option on the LCD
      displayMenu("Selected:", menuOptions[currentMenuOption]);
      readyToScan = true; // Set flag to indicate ready to scan
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

// Function to reset the esp32
void httpServerTask(void *pvParameters) {
    server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "ESP32 is resetting...");
        ESP.restart();
    });

    server.begin();

    while (true) {
        delay(1000);
    }
}

// Function to send data to API
void sendDataToAPI(String dataType, String data1, String data2)
{
  if (WiFi.status() == WL_CONNECTED)
  { // Check WiFi connection status
    HTTPClient http;
    http.setTimeout(5000); // Set a 5-second timeout for the request

    http.begin(serverIP + "/api/" + dataType);                               // Specify the URL of the API endpoint
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Set the POST content type

    // Add Basic Authorization Header
    String credentials = base64::encode(String(API_USERNAME) + ":" + String(API_PASSWORD));
    http.addHeader("Authorization", "Basic " + credentials);

    String postData = "";

    // Check the data type and prepare the POST data
    if (dataType == "temperature")
    {
      Serial.println("Sending temperature and humidity data to API");
      // Send temperature and humidity data
      postData = "temperature=" + data1 + "&humidity=" + data2;
    }
    else if (dataType == "card")
    {
      Serial.println("Sending card UID data to API");
      // Send card UID data
      if (needToSelect == 1)
      {
        Serial.println("No option selected");
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.drawStr(0, 10, "No option selected");
        u8g2.drawStr(0, 30, "Press buttons");
        u8g2.sendBuffer();
        delay(2000);
        u8g2.clearBuffer();
        u8g2.sendBuffer();
      } else {
        postData = "uid=" + data1 + "&option=" + data2;
        // Reset selected option after sending the card data
        needToSelect = 1;
      }
    }
    else if (dataType == "connection")
    {
      postData = "status=" + data1;
    }
    else if (dataType == "heartbeat")
    {
      Serial.println("Sending ESP32 heartbeat to API");
      postData = "status=" + data1;
    }
    else if (dataType == "address")
    {
      Serial.println("Sending ESP32 address to API");
      postData = "ip=" + data1 + "&mac=" + data2;
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

            // Display the username on the OLED
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_6x10_tf);
            if (currentMenuOption == 0)
            {
              u8g2.drawStr(0, 10, "Welcome,");
            }
            else if (currentMenuOption == 1)
            {
              u8g2.drawStr(0, 10, "Bye,");
            }
            else if (currentMenuOption == 2)
            {
              u8g2.drawStr(0, 10, "+1 Pitcher,");
            }
            u8g2.drawStr(0, 30, userName);
            u8g2.sendBuffer();

            delay(2000);

            u8g2.clearBuffer();
            u8g2.drawStr(0, 10, "Beertracker HUB");
            u8g2.drawStr(0, 30, "Select option");
            u8g2.sendBuffer();
          }
          else
          {
            // Handle various error statuses
            const char *message = doc["message"];
            Serial.println("Error: " + String(message));

            if (strcmp(message, "Card not found") == 0)
            {
              u8g2.clearBuffer();
              u8g2.setFont(u8g2_font_6x10_tf);
              u8g2.drawStr(0, 10, "Card not found");
              u8g2.drawStr(0, 30, "Add card to acc");
              u8g2.sendBuffer();
              delay(2000);

              u8g2.clearBuffer();
              u8g2.drawStr(0, 10, "Card UID:");
              u8g2.drawStr(0, 30, cardUID.c_str());
              u8g2.sendBuffer();
            }

            if (strcmp(message, "Invalid type or missing data") == 0)
            {
              u8g2.clearBuffer();
              u8g2.setFont(u8g2_font_6x10_tf);
              u8g2.drawStr(0, 10, "Beertracker HUB");
              u8g2.drawStr(0, 30, "Select option");
              u8g2.sendBuffer();
            }

            if (strcmp(message, "User not checked in") == 0)
            {
              u8g2.clearBuffer();
              u8g2.setFont(u8g2_font_6x10_tf);
              u8g2.drawStr(0, 10, "User not");
              u8g2.drawStr(0, 30, "checked in");
              u8g2.sendBuffer();
              delay(2000);

              u8g2.clearBuffer();
              u8g2.drawStr(0, 10, "Beertracker HUB");
              u8g2.drawStr(0, 30, "Select option");
              u8g2.sendBuffer();
            }

            if (strcmp(message, "User already checked in") == 0)
            {
              u8g2.clearBuffer();
              u8g2.setFont(u8g2_font_6x10_tf);
              u8g2.drawStr(0, 10, "User already");
              u8g2.drawStr(0, 30, "checked in");
              u8g2.sendBuffer();
              delay(2000);

              u8g2.clearBuffer();
              u8g2.drawStr(0, 10, "Beertracker HUB");
              u8g2.drawStr(0, 30, "Select option");
              u8g2.sendBuffer();
            }

            if (strcmp(message, "User already checked out") == 0)
            {
              u8g2.clearBuffer();
              u8g2.setFont(u8g2_font_6x10_tf);
              u8g2.drawStr(0, 10, "User already");
              u8g2.drawStr(0, 30, "checked out");
              u8g2.sendBuffer();
              delay(2000);

              u8g2.clearBuffer();
              u8g2.drawStr(0, 10, "Beertracker HUB");
              u8g2.drawStr(0, 30, "Select option");
              u8g2.sendBuffer();
            }

            if (strcmp(message, "User already checked out, cannot add pitcher") == 0) {
              u8g2.clearBuffer();
              u8g2.setFont(u8g2_font_6x10_tf);
              u8g2.drawStr(0, 10, "No pitcher");
              u8g2.drawStr(0, 30, "User checked out");
              u8g2.sendBuffer();
              delay(2000);

              u8g2.clearBuffer();
              u8g2.drawStr(0, 10, "Beertracker HUB");
              u8g2.drawStr(0, 30, "Select option");
              u8g2.sendBuffer();
            }
          }
        }
        else
        {
          Serial.println("Failed to parse response");
          // Handle parsing error
          u8g2.clearBuffer();
          u8g2.setFont(u8g2_font_6x10_tf);
          u8g2.drawStr(0, 10, "Parse Error");
          u8g2.sendBuffer();
          delay(2000);
          u8g2.clearBuffer();
          u8g2.sendBuffer();
        }
      }
    }
    else
    {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      // Display error on the OLED
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.drawStr(0, 10, "HTTP Error:");
      u8g2.drawStr(0, 30, String(httpResponseCode).c_str());
      u8g2.sendBuffer();
      delay(2000);
      u8g2.clearBuffer();
      u8g2.sendBuffer();
    }

    http.end(); // End the HTTP connection to free up resources
  }
  else
  {
    Serial.println("Error: WiFi not connected");
    // Turn off the LED
    digitalWrite(CONNECTION_LED, LOW);

    // Display WiFi error on the OLED
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "WiFi Error");
    u8g2.drawStr(0, 30, "Not connected");
    u8g2.sendBuffer();
    delay(2000);
    u8g2.clearBuffer();
    u8g2.sendBuffer();
  }
}