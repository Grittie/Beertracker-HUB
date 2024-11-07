# Embedded Code Documentation

This document provides a detailed explanation of the embedded code implementation found in the file [`main.cpp`](https://gitlab.fdmci.hva.nl/IoT/2024-2025-semester-1/individual-project/zuuliiyiizoo76/-/blob/main/embedded/iot-embedded/src/main.cpp). The goal is to describe both the general functionality and specific elements of the code to provide a deeper understanding of its workings.

## General Overview

The `main.cpp` file is the main entry point for an embedded system application. This code manages various functionalities of an IoT clock in and clock out system. The system is built around an ESP32-S3 microcontroller, which integrates multiple peripheral devices such as an RFID scanner, an I2C display, LEDs, buttons, and environmental sensors.

The code is structured to set up all peripherals and handle their operations efficiently using tasks and interrupts, leveraging the FreeRTOS multithreading environment. The implementation is designed to manage different actions, such as user authentication, environmental monitoring, and session management, all of which are coordinated through the central processing flow in `main.cpp`.

## Code Structure

### 1. **Libraries and Constants**

The code begins with importing the necessary libraries for handling different modules connected to the ESP32-S3, including:
- **Wire library** for I2C communication.
- **SPI library** for communication with RFID.
- **FreeRTOS-specific libraries** for task management.
- **Custom header files** that encapsulate specific device-related functionality like LCD, RFID, etc.

Several constants are defined for pins used, such as the SDA, SCL pins for I2C devices, and GPIO pins for LEDs, buttons, and RFID communication.

### 2. **Setup Function**

The `setup()` function is where all initialization happens. The steps include:
- **Initializing Serial Communication:** Primarily for debugging purposes.
- **RFID Initialization:** The RFID sensor is initialized, and its configuration is verified.
- **Display Initialization:** The I2C LCD screen is set up to display messages and provide feedback to the user.
- **Pin Configuration:** The pins for LEDs and buttons are set as input or output as required.
- **WiFi Connection Setup:** The code uses WiFiManager for connecting the ESP32 to the network, which enables communication with a backend server.

### 3. **Loop Function**

The `loop()` function typically contains the main execution cycle for an Arduino program, but in this case, it remains empty. Instead, the real functionality is executed using FreeRTOS tasks to achieve concurrent behavior.

### 4. **FreeRTOS Task Management**

The implementation uses several FreeRTOS tasks:
- **RFID Reading Task:** A task that continuously polls the RFID scanner to detect card taps and retrieve the UID. When a card is successfully scanned, the UID is sent to a backend server for validation and session management.
- **Environmental Monitoring Task:** This task reads data from the DHT11 temperature and humidity sensor and sends the data to the backend periodically.
- **Display Update Task:** Manages updating the LCD screen, such as displaying welcome messages, errors, or user-specific information when authenticated.
- **HTTP Server Task:** This task sets up an HTTP server to handle incoming requests from the web interface.

### 5. **Button Handling and Interrupts**

The code handles button presses using interrupts for better responsiveness. Two buttons are used to perform different actions:
- **Clock In Button:** Assigned to GPIO pin 20, it allows users to check in when pressed.
- **Clock Out Button:** The other button allows users to clock out of the system.

Interrupt Service Routines (ISRs) are configured to detect rising edges for both buttons, which helps in quick and accurate user interaction.

### 6. **WiFi Communication and Backend Integration**

Data such as RFID UIDs and temperature readings are sent to the backend using HTTP requests. The backend API handles data such as temperature (`temperature_handler.php`) or user sessions (`session_handler.php`). The integration with the backend allows storing the data centrally and generating reports for logged sessions.

### 7. **HTTP Server for Remote Reset**

The implementation includes an HTTP server task (`httpServerTask`) that enables remote interaction with the ESP32 through a web interface. One notable feature is the ability to reset the ESP32 remotely by sending an HTTP POST request to the `/reset` endpoint. When this endpoint is triggered, the server sends a response confirming the reset and then calls `ESP.restart()` to reset the microcontroller.

This feature is particularly useful for remotely troubleshooting or rebooting the device without needing physical access, ensuring that any configuration changes or software updates can be applied effectively.

```cpp
void httpServerTask(void *pvParameters) {
    server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "ESP32 is resetting...");
        ESP.restart();
    });

    server.begin();

    while (true) {
        delay(1000);
    }
}
```

### 8. **Error Handling**

Error handling is present in various functions, especially for device initialization and communication failures. If, for example, the RFID sensor fails to initialize, the system will show an error message on the display and prevent further operations until the error is resolved.

## Notable Features

- **Multithreading with FreeRTOS:** The use of FreeRTOS tasks enables the system to perform multiple actions simultaneously, such as scanning RFID tags while reading environmental data and updating the display.
- **Real-Time Updates:** The use of interrupts for buttons ensures that the system responds to user inputs in real time without noticeable delay.
- **Backend API Communication:** The ESP32 communicates with a backend server, enabling seamless logging of user sessions and temperature monitoring, which is essential for remote monitoring and analysis.
- **Remote Reset Capability:** The HTTP server task allows the ESP32 to be reset remotely, providing a convenient way to manage and troubleshoot the device.

## Link to Code Repository

For further information, you can refer to the code available in the repository: [main.cpp](https://gitlab.fdmci.hva.nl/IoT/2024-2025-semester-1/individual-project/zuuliiyiizoo76/-/blob/main/embedded/iot-embedded/src/main.cpp)