# Technical documentation

## Wiring Diagram
A wiring diagram shows how components are connected. It uses symbols for parts like resistors and power sources to illustrate connections and current flow, helping with troubleshooting.

### RFID Scanning system
### Breadboard Diagram
<img src="../../assets/fritzing/RFID RC522_bb.png" alt="Alt text" width="600" />

### Schematic Diagram
<img src="../../assets/fritzing/RFID RC522_schem.png" alt="Alt text" width="500" />


## Bill of Materials (BOM)

A BOM lists all components needed for a project, including names, quantities, and specifications. It acts as a checklist for sourcing parts and assembling the system.

| Component                          | Description                                                                 | Quantity | Price (EUR) | Supplier     | Purchase Link                                                                                                                                             |
|------------------------------------|-----------------------------------------------------------------------------|----------|-------------|--------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Perfboard**                      | Prototyping board for soldering components                                  | 1        | €1.95       | TinyTronics  | [Perfboard 7x9cm - Single Sided](https://www.tinytronics.nl/en/prototyping-and-installation/prototyping-supplies/perfboards/perfboard-7x9cm-single-sided) |
| **Bambu Lab PLA Matte White**      | Matte white PLA filament for 3D printing                                    | 1        | $19.99      | Bambu Lab    | [Bambu Lab PLA Matte - Ivory White](https://store.bambulab.com/en/products/pla-matte-filament)                                                            |
| **Bambu Lab PLA Basic Yellow**     | Basic yellow PLA filament for 3D printing                                   | 1        | $19.99      | Bambu Lab    | [Bambu Lab PLA Basic - Sunflower Yellow](https://store.bambulab.com/en/products/pla-basic-filament)                                                       |
| **RFID RC522 Module**              | RFID reader module for reading RFID tags                                    | 1        | €4.95       | TinyTronics  | [RFID Kit MFRC522 S50 Mifare with Card and Key Tag](https://www.tinytronics.nl/en/communication-and-signals/wireless/rfid/rfid-kit-mfrc522-s50-mifare-with-card-and-key-tag) |
| **16x2 LCD Screen**                | 16x2 character LCD display                                                  | 1        | €3.95       | TinyTronics  | [LCD1602 16x2 Character Display - Yellow/Green](https://www.tinytronics.nl/en/displays/lcd/lcd1602-16x2-character-display-yellow-green)                   |
| **I2C Backpack for LCD**           | I2C interface module for LCD screens                                        | 1        | €1.95       | TinyTronics  | [I2C Interface Module for LCD1602 and LCD2004](https://www.tinytronics.nl/en/displays/lcd/i2c-interface-module-for-lcd1602-and-lcd2004)                   |
| **Green LED**                      | 5mm green LED                                                               | 1        | €0.95       | TinyTronics  | [LED 5mm Green - 10 pieces](https://www.tinytronics.nl/en/light-and-sound/lighting/individual-leds/led-5mm-green-10-pieces)                               |
| **Blue LED**                       | 5mm blue LED                                                                | 1        | €0.95       | TinyTronics  | [LED 5mm Blue - 10 pieces](https://www.tinytronics.nl/en/light-and-sound/lighting/individual-leds/led-5mm-blue-10-pieces)                                 |
| **Red LED**                        | 5mm red LED                                                                 | 1        | €0.95       | TinyTronics  | [LED 5mm Red - 10 pieces](https://www.tinytronics.nl/en/light-and-sound/lighting/individual-leds/led-5mm-red-10-pieces)                                   |
| **1kΩ Resistors**                  | 1k ohm resistors                                                            | 5        | €0.95       | TinyTronics  | [Resistor 1kΩ - 10 pieces](https://www.tinytronics.nl/en/components/components/resistors/resistor-1kω-10-pieces)                                          |
| **Passive Buzzer**                 | Passive piezoelectric buzzer                                                | 1        | €0.95       | TinyTronics  | [Passive Buzzer Module](https://www.tinytronics.nl/en/light-and-sound/audio/buzzer-module-passive)                                                        |
| **Jumper Wires**                   | Male-to-male jumper wires for prototyping                                   | 1 set    | €2.95       | TinyTronics  | [Jumper Wires Male to Male - 65 pieces](https://www.tinytronics.nl/en/prototyping-and-installation/prototyping-supplies/jumper-wires-male-to-male-65-pieces) |
| **DHT11 Sensor Module**            | Temperature and humidity sensor module                                      | 1        | €2.95       | TinyTronics  | [DHT11 Temperature and Humidity Sensor Module](https://www.tinytronics.nl/en/sensors/air/dht11-temperature-and-humidity-sensor-module)                    |
| **ESP32 Development Board**        | ESP32 microcontroller board with Wi-Fi and Bluetooth                        | 1        | €7.95       | TinyTronics  | [ESP32 Development Board - WiFi and Bluetooth](https://www.tinytronics.nl/en/development-boards/esp32-development-board-wifi-and-bluetooth)               |
| **Push Buttons**                   | Momentary push buttons                                                      | 2        | €0.95       | TinyTronics  | [Push Button 12x12mm - 5 pieces](https://www.tinytronics.nl/en/components/components/switches/manual-switches/push-button-12x12mm-5-pieces)               |
| **Capacitor Assortment**           | Assorted electrolytic capacitors                                            | 1 set    | €4.95       | TinyTronics  | [Electrolytic Capacitor Assortment - 24 values](https://www.tinytronics.nl/en/components/components/capacitors/electrolytic-capacitor-assortment-24-values) |
| **USB-C Cable**                    | USB 2.0 cable A to USB-C, 1 meter                                           | 1        | €2.95       | TinyTronics  | [USB 2.0 Cable A to USB-C - 1 meter](https://www.tinytronics.nl/en/cables-and-connectors/cables-and-adapters/usb-cables/usb-2.0-cable-a-to-usb-c-1-meter) |
| **USB-C Wall Adapter (5V 3A)**     | 5V 3A USB-C power adapter                                                   | 1        | €7.95       | TinyTronics  | [USB-C Power Adapter 5V 3A](https://www.tinytronics.nl/en/power/power-adapters/usb-c-power-adapter-5v-3a)                                                 |
| **Soldering Tin**                  | 0.8mm soldering tin, 100g                                                   | 1        | €9.95       | TinyTronics  | [Soldering Tin 0.8mm 100g](https://www.tinytronics.nl/en/prototyping-and-installation/prototyping-supplies/soldering-tin-0.8mm-100g)                      |
