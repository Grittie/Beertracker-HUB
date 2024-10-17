# Technical documentation

## Wiring Diagram
### RFID Scanning system
### Breadboard Diagram
<img src="../../assets/fritzing/RFID RC522_bb.png" alt="Alt text" width="600" />
### Schematic Diagram
<img src="../../assets/fritzing/RFID RC522_schem.png" alt="Alt text" width="500" />


## BOM
| **Component**               | **Description**                             | **Quantity** | **Part Number / Specs**         | **Supplier** | **Price** (Estimate) | **Notes**                              |
|-----------------------------|---------------------------------------------|--------------|---------------------------------|--------------|----------------------|----------------------------------------|
| ESP32-S3                     | Microcontroller                            | 1            | ESP32-S3-WROOM-1                | TinyTronics  | €10.00               | Central unit of the system            |
| RFID RC522                   | RFID Sensor for reading cards              | 1            | RFID-RC522                      | TinyTronics  | €4.50                | Used for reading RFID tags            |
| LCD Screen                   | Display for system feedback                | 1            | 16x2 LCD (I2C)                  | TinyTronics  | €6.00                | Requires I2C backpack                 |
| I2C Backpack for LCD         | I2C interface module for LCD               | 1            | PCF8574                         | TinyTronics  | €1.50                | Allows simpler connection via I2C     |
| Orange LED                   | Status indicator                           | 1            | 5mm Orange LED                  | TinyTronics  | €0.10                | Indicates successful actions          |
| Red LED                      | Status indicator                           | 1            | 5mm Red LED                     | TinyTronics  | €0.10                | Indicates errors or warnings          |
| Blue LED                     | Status indicator                           | 1            | 5mm Blue LED                    | TinyTronics  | €0.10                | Indicates Active WiFi Connection      |
| Tactile Push Buttons         | User input buttons                         | 2            | 6mm Tactile Button              | TinyTronics  | €0.20 (each)          | For user interactions (e.g., clock in/out)|
| Buzzer                       | Audio feedback                             | 1            | Active Buzzer                   | TinyTronics  | €1.00                | Used to signal actions with sound     |
| Breadboards                  | Prototyping boards                         | 2            | 830 tie-points                  | TinyTronics  | €4.00 (each)          | For quick wiring of components        |
| Resistor for Orange LED       | Limits current to the orange LED           | 1            | 330Ω Resistor                   | TinyTronics  | €0.05                | Protects the LED from excessive current |
| Resistor for Buttons         | Pull-down resistors to prevent floating inputs | 2         | 10kΩ Resistor                   | TinyTronics  | €0.05 (each)          | Ensures proper button states          |
| Jumper Wires                 | Wiring connections                         | As needed    | Male-to-Male, Male-to-Female    | TinyTronics  | €3.00 (set)           | For connecting components             |
| DHT11 Sensor                 | Temperature sensor                         | 1            | DHT11                           | TinyTronics  | €3.00                | Measures temperature                  |
| DC-DC Converter              | Voltage converter                          | 1            | Buck Converter                  | TinyTronics  | €2.50                | Converts 9V to 5V for powering ESP32  |
| 9V Battery                   | Battery for powering the system            | 1            | 9V Battery                      | TinyTronics  | €2.00                | Main power source                     |
| Barrel Jack Adapter          | 9V power connector                         | 1            | Barrel Jack Adapter             | TinyTronics  | €1.50                | Connects battery to the system        |
| Capacitor                    | Power supply stabilization                 | 1            | 10µF Electrolytic Capacitor     | TinyTronics  | €0.10                | Helps stabilize the power supply      |
