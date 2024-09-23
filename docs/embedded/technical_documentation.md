# Technical documentation

## Wiring Diagram
### RFID Scanning system
### Breadboard Diagram
<img src="../../assets/images/RC522/RFID RC522_bb.png" alt="Alt text" width="600" />
### Schematic Diagram
<img src="../../assets/images/RC522/RFID RC522_schem.png" alt="Alt text" width="500" />


## BOM
| **Component**               | **Description**                             | **Quantity** | **Part Number / Specs**         | **Supplier** | **Notes**                              |
|-----------------------------|---------------------------------------------|--------------|---------------------------------|--------------|----------------------------------------|
| ESP32-S3                     | Microcontroller                            | 1            | ESP32-S3-WROOM-1                | [Alixpress]   | Central unit of the system            |
| RFID RC522                   | RFID Sensor for reading cards              | 1            | RFID-RC522                      | [Alixpress]   | Used for reading RFID tags            |
| LCD Screen                   | Display for system feedback                | 1            | 16x2 LCD (I2C)                  | [HvA]   | Requires I2C backpack                 |
| I2C Backpack for LCD         | I2C interface module for LCD               | 1            | PCF8574                         | [HvA]   | Allows simpler connection via I2C     |
| Green LED                    | Status indicator                           | 1            | 5mm Green LED                   | [TinyTronics]   | Indicates successful actions          |
| Red LED                      | Status indicator                           | 1            | 5mm Red LED                     | [TinyTronics]   | Indicates errors or warnings          |
| Blue LED                     | Status indicator                           | 1            | 5mm Blue LED                    | [TinyTronics]   | Indicates Active Wifi Connection       |
| Tactile Push Buttons          | User input buttons                         | 2            | 6mm Tactile Button              | [HvA]   | For user interactions (e.g., clock in/out)|
| Buzzer                       | Audio feedback                             | 1            | Active Buzzer                   | [HvA]   | Used to signal actions with sound     |
| Breadboards                  | Prototyping boards                         | 2            | 830 tie-points                  | [TinyTronics]   | For quick wiring of components, will be replaced by pcbs        |
| Resistor for Green LED  | Limits current to the green LED            | 1            | 330Ω Resistor                   | [HvA]   | Protects the LED from excessive current|
| Resistor for Buttons     | Pull-down resistors to prevent floating inputs | 2         | 10kΩ Resistor                   | [HvA]   | Used to ensure proper button states   |
| Jumper Wires                 | Wiring connections                         | As needed    | Male-to-Male, Male-to-Female    | [TinyTronics]   | For connecting components             |
| 5V Power Supply              | Provides power to the system               | 1            | 5V 1A USB Power Adapter         | [Alixpress]   | Supplies power to ESP32 and peripherals|
| Capacitor                | Power supply stabilization                 | 1            | 10µF Electrolytic Capacitor     | [Alixpress]   | Helps stabilize the power supply      |
