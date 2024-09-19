#include <Arduino.h>

const int analogPin = A0;  // Analog pin on the Wemos
int sensorValue = 0;

void setup() {
  Serial.begin(115200);  // Start serial communication for debugging
}

void loop() {
  analogWrite(analogPin, HIGH);
  Serial.println("Heating up the sensor...");
  delay(60000); 
  analogWrite(analogPin, LOW);
  sensorValue = analogRead(analogPin);  // Read the gas sensor value
  Serial.print("Gas concentration: ");
  Serial.println(sensorValue);  // Print the value to the Serial Monitor
  delay(1000);  // Read every second
}



