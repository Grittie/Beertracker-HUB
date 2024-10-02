#include <Arduino.h>

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Wait for the serial connection to establish
  while (!Serial) {
    delay(10);
  }

  Serial.println("ESP32 is ready!");
}

void loop() {
  // Print a message every second
  Serial.println("Hello from ESP32!");
  delay(1000);
}
