# Requirements

This document outlines the hardware *and* functional requirements for the **Smart Time Registration Device**. The system is based on an ESP32-S3-DevKitC-1-N8R8 and is designed to handle RFID-based user identification, API communication, provide user interaction through buttons to cycle between actions, LEDs, and a buzzer.

---

## Hardware Components

- **Microcontroller**: ESP32-S3-DevKitC-1-N8R8
<br> <img src="https://m.media-amazon.com/images/I/61AQzpQyIhL._AC_UF894,1000_QL80_.jpg" alt="Alt text" width="200" />

- **RFID Sensor**: RFID-RC522 for scanning HVA IDs
<br> <img src="https://components101.com/sites/default/files/components/RC522-RFID-module.jpg" alt="Alt text" width="250" />

- **LCD Display**: 16x2 LCD with I2C Backpack
<br> <img src="https://cdn.bodanius.com/media/1/9ba102557_16x2-karakters-lcd-module-blauw_900x600.png" alt="Alt text" width="250" />

- **Buzzer**: For feedback on card scan
- **LEDs**: Multiple for system status indication
- **Buttons**: For switching between actions (e.g., Clock In, Clock Out, Start Break, End Break)
- **Connectivity**: Wi-Fi for making API calls to the website
---

## Functional Requirements

### 1. **RFID Scanning**
   - The system will use the RFID-RC522 RFID sensor to scan user HVA IDs.
   - Upon scanning, the system will push the ID data via an API call.

### 2. **API Communication**
   - The ESP32-S3 will connect to the internet to make API calls:
     - **Send scanned data** (RFID) to the server.
     - **Receive user data** (e.g., user name) in response and display on the LCD.

### 3. **Display and Feedback**
   - The LCD screen, connected via I2C, will show user information and action statuses.
   - The buzzer will sound when a card is successfully scanned.
   - LEDs will provide status indications for:
     - Power (system turned on)
     - Internet connectivity (connected/disconnected)
     - Card scan status (success/failure)

### 4. **User Input and Actions**
   - The system will have buttons to switch between different actions:
     - Clock In
     - Clock Out
     - Start Break
     - End Break

---

## Inputs and Outputs

| **Component**      | **Type**  | **Function**                                                                  |
|--------------------|-----------|-------------------------------------------------------------------------------|
| **ESP32-S3-DevKitC-1-N8R8**       | MCU       | Microcontroller to handle processing and communication.                       |
| **RC522**          | Input     | Reads RFID cards to identify users.                                           |
| **LCD Display**    | Output    | Displays the user name and system statuses.                                   |
| **Buzzer**         | Output    | Emits sound on successful card scan.                                          |
| **LEDs**           | Output    | Shows system status: Power, Internet Connectivity, and Card Scan result.      |
| **Buttons**        | Input     | Allows the user to switch between actions (Clock In, Clock Out, etc.).        |

---

## System Workflow

1. **Power On**: System starts, LEDs indicate power and system status.
2. **Internet Connection**: LED indicates when the system is connected to the internet.
3. **RFID Scan**: User scans their ID card using the RC522 sensor.
4. **API Call**: The ESP32-S3 sends the RFID data to the server via API.
5. **API Response**: System receives user information and displays it on the LCD screen.
6. **Action Selection**: User selects the action (Clock In/Out, Start/End Break) via buttons.
7. **Feedback**: Buzzer sounds when a card is scanned, LEDs change based on status (e.g., card scan success).

---

## DLO Requirements

| **Requirement ID#** | **Requirement**                                                                                                                                                      | **MoSCoW** | **Compliant** |
|---------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------|------------|---------------|
| **EMBRQ#01**        | The embedded device acts as a client and sends measured sensor data to the application backend over HTTP or HTTPS.                                                     | MUST       | NO            |
| **EMBRQ#02**        | The embedded device also acts as a server and receives status messages from the application backend over HTTP or HTTPS.                                                | MUST       | NO            |
| **EMBRQ#03**        | The embedded device contains at least two types of input sensors (e.g. LDR, buttons, joystick, capacitive touch, etc.).                                                | MUST       | NO            |
| **EMBRQ#04**        | The embedded device contains at least two types of visual and/or sensory outputs (e.g. LED, LED Matrix, 7-segment display, motor, servo, actuator, LCD-screen, buzzer, etc.). | MUST       | NO            |
| **EMBRQ#05**        | The embedded device uses the WifiManager for configuration of SSID and password (PWD) for connecting to the network.                                                   | MUST       | NO            |

---

### EMBRQ#01

**Requirement**: The embedded device acts as a client and sends measured sensor data to the application backend over HTTP or HTTPS.

**Explanation**:  
[Insert text explaining how you fulfilled the requirement here]

**Code**:
```cpp
// lorem ipsum
```

---

### EMBRQ#02

**Requirement**: The embedded device also acts as a server and receives status messages from the application backend over HTTP or HTTPS.

**Explanation**:  
[Insert text explaining how you fulfilled the requirement here]

**Code**:
```cpp
// lorem ipsum
```

---

### EMBRQ#03

**Requirement**: The embedded device contains at least two types of input sensors (e.g. LDR, buttons, joystick, capacitive touch, etc.).

**Explanation**:  
[Insert text explaining how you fulfilled the requirement here]

**Code**:
```cpp
// lorem ipsum
```

---

### EMBRQ#04

**Requirement**: The embedded device contains at least two types of visual and/or sensory outputs (e.g. LED, LED Matrix, 7-segment display, motor, servo, actuator, LCD-screen, buzzer, etc.).

**Explanation**:  
[Insert text explaining how you fulfilled the requirement here]

**Code**:
```cpp
// lorem ipsum
```

---

### EMBRQ#05

**Requirement**: The embedded device uses the WifiManager for configuration of SSID and password (PWD) for connecting to the network.

**Explanation**:  
[Insert text explaining how you fulfilled the requirement here]

**Code**:
```cpp
// lorem ipsum
```
---