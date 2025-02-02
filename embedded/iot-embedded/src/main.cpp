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
#define SS_PIN 5    // SDA/SS Pin for SPI
#define RST_PIN 17  // Reset Pin for SPI
#define SCAN_LED 33 // LED Pin
#define BUZZER_PIN 25 // Buzzer Pin
#define TEMP_SENSOR 26 // Analog Pin
#define DHTTYPE DHT11  // DHT 11 type sensor

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
DHT dht(TEMP_SENSOR, DHTTYPE);    // Create DHT instance

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 21);
AsyncWebServer server(80);

volatile bool cardDetected = false;
String cardUID = "";
String serverIP = SERVER_URL;

void rfidTask(void *pvParameters);
void feedbackTask(void *pvParameters);
void temperatureTask(void *pvParameters);
void heartbeatTask(void *pvParameters);
void addressTask(void *pvParameters);
void sendDataToAPI(String dataType, String data1, String data2);

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    SPI.begin();
    mfrc522.PCD_Init();
    dht.begin();

    pinMode(SCAN_LED, OUTPUT);
    digitalWrite(SCAN_LED, LOW);
    pinMode(BUZZER_PIN, OUTPUT);

    // Initialize OLED
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Beertracker WiFi");
    u8g2.drawStr(0, 30, "Connect to:");
    u8g2.drawStr(0, 40, "SSID: Beertracker WiFi");
    u8g2.drawStr(0, 50, "Pass: password123");
    u8g2.sendBuffer();

    WiFiManager wifiManager;
    if (!wifiManager.autoConnect("Beertracker WiFi", "password123")) {
        Serial.println("Failed to connect and hit timeout");
        u8g2.clearBuffer();
        u8g2.drawStr(0, 10, "WiFi Failed");
        u8g2.drawStr(0, 30, "Restarting...");
        u8g2.sendBuffer();
        delay(2000);
        ESP.restart();
    }

    Serial.println("Connected to Wi-Fi");

    // Update OLED to show WiFi connected
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "WiFi Connected!");
    u8g2.sendBuffer();
    delay(2000);

    // Play startup sound
    tone(BUZZER_PIN, 1000);
    delay(100);
    noTone(BUZZER_PIN);

    // Show Initialization message
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "Beertracker HUB");
    u8g2.drawStr(0, 30, "Initializing...");
    u8g2.sendBuffer();
    delay(2000);
    u8g2.clearBuffer();

    // Start tasks
    xTaskCreate(rfidTask, "RFID Task", 10000, NULL, 1, NULL);
    xTaskCreate(feedbackTask, "Feedback Task", 10000, NULL, 2, NULL);
    xTaskCreate(temperatureTask, "Temperature Task", 10000, NULL, 3, NULL);
    xTaskCreate(heartbeatTask, "Heartbeat Task", 10000, NULL, 4, NULL);
    xTaskCreate(addressTask, "Address Task", 10000, NULL, 5, NULL);

    // Show scan message
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "Beertracker");
    u8g2.drawStr(0, 30, "Scan card below");
    u8g2.sendBuffer();
}


void loop() {}

void rfidTask(void *pvParameters) {
    Serial.println("RFID Task Started");
    while (true) {
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
            // play a short beep
            tone(BUZZER_PIN, 1000, 100);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            noTone(BUZZER_PIN);

            cardUID = "";
            for (byte i = 0; i < mfrc522.uid.size; i++) {
                cardUID += String(mfrc522.uid.uidByte[i], HEX);
            }
            cardUID.toUpperCase();
            Serial.println("Card detected: " + cardUID);

            // Step 1: Check if the user is clocked in
            HTTPClient http;
            http.begin(serverIP + "/api/card");
            http.addHeader("Content-Type", "application/x-www-form-urlencoded");

            String credentials = base64::encode(String(API_USERNAME) + ":" + String(API_PASSWORD));
            http.addHeader("Authorization", "Basic " + credentials);

            String postData = "uid=" + cardUID + "&option=0"; // Check clock-in status
            int httpResponseCode = http.POST(postData);

            if (httpResponseCode > 0) {
                String response = http.getString();
                Serial.println("Response: " + response);

                DynamicJsonDocument doc(2048); // Increased buffer size
                DeserializationError error = deserializeJson(doc, response);
                if (!error) {
                    const char *status = doc["status"];
                    const char *message = doc["message"];

                    Serial.print("Parsed Status: ");
                    Serial.println(status);
                    Serial.print("Parsed Message: ");
                    Serial.println(message);

                    if (strcmp(status, "success") == 0) {
                        const char *action = doc["action"];
                        const char *userName = doc["name"];
                        if (strcmp(action, "checked_in") == 0) {
                            // User is now clocked in
                            Serial.println("User clocked in: " + cardUID);
                            u8g2.clearBuffer();
                            u8g2.setFont(u8g2_font_6x10_tf);
                            u8g2.drawStr(0, 10, "Welcome,");
                            u8g2.drawStr(0, 30, userName);
                            u8g2.sendBuffer();

                            // Play "boom boom" sound
                            tone(BUZZER_PIN, 500, 200); // First tone: 500 Hz, 200 ms
                            vTaskDelay(300 / portTICK_PERIOD_MS);
                            tone(BUZZER_PIN, 700, 200); // Second tone: 700 Hz, 200 ms
                            vTaskDelay(300 / portTICK_PERIOD_MS);
                            noTone(BUZZER_PIN);

                            // wait for 2 seconds before clearing the display
                            vTaskDelay(2000 / portTICK_PERIOD_MS);
                            u8g2.clearBuffer();
                            u8g2.setFont(u8g2_font_6x10_tf);
                            u8g2.drawStr(0, 10, "Beertracker");
                            u8g2.drawStr(0, 30, "Scan card below");
                            u8g2.sendBuffer();
                        }
                    } else if (strcmp(status, "error") == 0) {
                        if (strcmp(message, "User already checked in") == 0) {
                            // User is already clocked in, add a pitcher
                            Serial.println("+1 Pitcher for user: " + cardUID);
                            u8g2.clearBuffer();
                            u8g2.setFont(u8g2_font_6x10_tf);
                            u8g2.drawStr(0, 10, "+1 Pitcher,");
                            u8g2.drawStr(0, 30, "Thank you! :)");
                            u8g2.sendBuffer();

                            // Play "ta ta ta daaa" sound
                            tone(BUZZER_PIN, 500, 200); // First tone: 500 Hz, 200 ms
                            vTaskDelay(250 / portTICK_PERIOD_MS);
                            tone(BUZZER_PIN, 600, 200); // Second tone: 600 Hz, 200 ms
                            vTaskDelay(250 / portTICK_PERIOD_MS);
                            tone(BUZZER_PIN, 700, 200); // Third tone: 700 Hz, 200 ms
                            vTaskDelay(250 / portTICK_PERIOD_MS);
                            tone(BUZZER_PIN, 900, 500); // Fourth tone: 900 Hz, 500 ms
                            vTaskDelay(500 / portTICK_PERIOD_MS);
                            noTone(BUZZER_PIN);

                            // Send another request to increment pitcher count
                            String postData = "uid=" + cardUID + "&option=2";
                            http.begin(serverIP + "/api/card");
                            http.addHeader("Content-Type", "application/x-www-form-urlencoded");
                            http.addHeader("Authorization", "Basic " + credentials);
                            int httpResponseCode = http.POST(postData);

                            if (httpResponseCode > 0) {
                            String response = http.getString();
                            Serial.println("Pitcher increment response: " + response);

                            // Parse the JSON response
                            DynamicJsonDocument doc(1024); // Adjust buffer size if needed
                            DeserializationError error = deserializeJson(doc, response);
                            if (!error) {
                                const char *name = doc["name"];
                                int newPitcherCount = doc["newPitcherCount"];

                                // Prepare display content
                                String line1 = String(name) + " is now on";
                                String line2 = String(newPitcherCount) + " pitchers today!";

                                // Display on OLED
                                u8g2.clearBuffer();
                                u8g2.setFont(u8g2_font_6x10_tf);
                                u8g2.drawStr(0, 10, line1.c_str());
                                u8g2.drawStr(0, 30, line2.c_str());
                                u8g2.sendBuffer();

                                // Delay to show the message
                                vTaskDelay(2000 / portTICK_PERIOD_MS);
                            } else {
                                Serial.println("Failed to parse JSON response");
                            }
                        } else {
                            Serial.println("Error incrementing pitcher: " + String(httpResponseCode));
                        }
                            // wait for 2 seconds before clearing the display
                            vTaskDelay(2000 / portTICK_PERIOD_MS);
                            u8g2.clearBuffer();
                            u8g2.setFont(u8g2_font_6x10_tf);
                            u8g2.drawStr(0, 10, "Beertracker");
                            u8g2.drawStr(0, 30, "Scan card below");
                            u8g2.sendBuffer();
                        }

                        if (strcmp(message, "Card not found") == 0) {
                          tone(BUZZER_PIN, 300, 200);
                          vTaskDelay(250 / portTICK_PERIOD_MS);
                          noTone(BUZZER_PIN);

                          u8g2.clearBuffer();
                          u8g2.drawStr(0, 10, "Card not found");
                          u8g2.drawStr(0, 30, cardUID.c_str());
                          u8g2.sendBuffer();
                        }     
                    } else {
                        Serial.println("Unhandled Status or Message");
                    }
                } else {
                    Serial.println("Failed to parse response");
                }
            } else {
                Serial.println("HTTP Error: " + String(httpResponseCode));
            }

            http.end();
            cardDetected = true;
            mfrc522.PICC_HaltA();
        }

        vTaskDelay(100 / portTICK_PERIOD_MS); // Avoid WDT timeout
    }
}

void feedbackTask(void *pvParameters) {
    while (true) {
        if (cardDetected) {
            digitalWrite(SCAN_LED, HIGH);
            tone(BUZZER_PIN, 523);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            noTone(BUZZER_PIN);
            digitalWrite(SCAN_LED, LOW);

            vTaskDelay(500 / portTICK_PERIOD_MS);
            cardDetected = false;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS); // Yield to WDT
    }
}

void temperatureTask(void *pvParameters) {
    while (true) {
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        if (!isnan(temperature)) {
            Serial.printf("Temperature: %.2f, Humidity: %.2f\n", temperature, humidity);
            sendDataToAPI("temperature", String(temperature), String(humidity));
        } else {
            Serial.println("Failed to read temperature or humidity from DHT sensor.");
        }

        vTaskDelay(60000 / portTICK_PERIOD_MS); // 1 minute delay
    }
}

void heartbeatTask(void *pvParameters) {
    while (true) {
        Serial.println("Sending heartbeat...");
        sendDataToAPI("heartbeat", "active", "");
        vTaskDelay(15000 / portTICK_PERIOD_MS); // 15 seconds delay
    }
}

void addressTask(void *pvParameters) {
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Waiting for WiFi connection...");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    }

    String ipAddress = WiFi.localIP().toString();
    String macAddress = WiFi.macAddress();
    sendDataToAPI("address", ipAddress, macAddress);
    vTaskDelete(NULL);
}

void sendDataToAPI(String dataType, String data1, String data2) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverIP + "/api/" + dataType);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        String credentials = base64::encode(String(API_USERNAME) + ":" + String(API_PASSWORD));
        http.addHeader("Authorization", "Basic " + credentials);

        String postData;
        if (dataType == "temperature") {
            postData = "temperature=" + data1 + "&humidity=" + data2;
        } else if (dataType == "card") {
            postData = "uid=" + data1;
        } else if (dataType == "heartbeat") {
            postData = "status=" + data1;
        } else if (dataType == "address") {
            postData = "ip=" + data1 + "&mac=" + data2;
        }

        int httpResponseCode = http.POST(postData);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.printf("Successfully sent %s data. Response: %s\n", dataType.c_str(), response.c_str());
        } else {
            Serial.printf("Error sending %s data. HTTP Response code: %d\n", dataType.c_str(), httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("Error: WiFi not connected");
    }
}