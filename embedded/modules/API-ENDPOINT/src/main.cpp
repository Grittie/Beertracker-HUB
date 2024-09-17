#include <Arduino.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <SPI.h>

const float humidity = 0.81;
const float temperature = 10.3;
const char* serverURL = "http://localhost/php/api.php";  // Make sure this is the correct URL

void sendSensorData(float temperature, float humidity) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverURL);

        // Set POST request content type and data
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        String postData = "type=sensor_data&temperature=" + String(temperature) + "&humidity=" + String(humidity);

        // Send the POST request
        int httpResponseCode = http.POST(postData);

        // Check for the response
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Response: " + response);
        } else {
            Serial.println("Error on sending POST: " + String(httpResponseCode));
        }

        http.end();  // End the HTTP request
    }
}

void sendTimeRegistration(String uid, String time, String action) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverURL);

        // Set POST request content type and data
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        String postData = "type=time_registration&uid=" + uid + "&time=" + time + "&action=" + action;

        // Send the POST request
        int httpResponseCode = http.POST(postData);

        // Check for the response
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Response: " + response);
        } else {
            Serial.println("Error on sending POST: " + String(httpResponseCode));
        }

        http.end();  // End the HTTP request
    }
}

void setup() {
    // Start serial communication
    Serial.begin(115200);

    // Create a Wi-Fi Manager object
    WiFiManager wifiManager;

    // This line resets saved Wi-Fi credentials
    wifiManager.resetSettings();

    // Changes the theme to dark mode in the Wi-Fi config portal
    wifiManager.setClass("invert");

    // Automatically connect, or go to config portal if not connected
    if (!wifiManager.autoConnect("BeertrackerHUB™️ Wifi Portal", "3opMujJtembh6")) {
        Serial.println("Failed to connect and hit timeout");
        ESP.restart();
    }

    // If you get here, you are connected to the WiFi
    Serial.println("Connected to Wi-Fi");
}

void loop() {
    // Send sensor data in the loop
    sendSensorData(temperature, humidity);
    delay(1000);  // Wait for 60 seconds before sending again
}
