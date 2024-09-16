#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <SPI.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
}

void loop() {
  lcd.setContrast(0);```
  lcd.print("Hello, world!");
  delay(1000);
  lcd.clear();
  delay(1000);
}