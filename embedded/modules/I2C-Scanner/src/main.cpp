#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <SPI.h>
 
void setup() {
  Wire.begin(); // Start the I2C interface
  Serial.begin(115200); // Start the serial interface
  Serial.println("\nI2C Scanner");
}
 
void loop() {
  byte error, address; // Variable for storing I2C addresses
  int nDevices; // Variable for storing the number of devices
  Serial.println("Scanning..."); 
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address); // Begin transmission to the I2C address
    error = Wire.endTransmission();
    // If the device is found, print the address
    if (error == 0) {
      // Print the address in hexadecimal format
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      // Print the address in hexadecimal format
      Serial.println(address,HEX);
      nDevices++;
    }
    // If there is an error, print an error message
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  // If no devices are found, print a message
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}

