## **System Architecture**

### **Introduction**

The **System Architecture Diagram** provides an abstraction of the entire clock-in/clock-out system. It shows the interaction between the embedded system (ESP32-S3), frontend, backend, database, and the networking components. This diagram is essential to understand how the hardware and software components are connected, enabling smooth communication and efficient logging of user check-ins/outs and sensor data (e.g., temperature and humidity). This architecture also ensures the system is scalable and maintainable for future expansions.

### **Legenda**

The symbols used in the system architecture diagram are:

- **Solid Rectangle**: Represents hardware components like the ESP32, sensors, and RFID readers.
- **Dashed Rectangle**: Represents software components, including the PHP API, frontend files, and Docker components.
- **Rounded Rectangle**: Represents virtualized components, such as Docker containers or network components.
- **Cylinder**: Represents databases or data storage elements.
- **Arrow**: Indicates communication protocols between the components, with labels specifying the protocol and TCP/IP port when applicable.

#### **Legend of Symbols**
- **Hardware Components**: Rectangles with solid borders.
- **Software Components**: Rectangles with dashed borders.
- **Data Storage**: Cylindrical shapes (for databases).
- **Virtual Components**: Rounded rectangles (for Docker containers, virtual environments).
- **Communication Protocols**: Arrows with labels like HTTP, MySQL, or I2C, indicating direction of data flow.

### **System Architecture Diagram**

_**[Insert your system architecture diagram here]**_

---

### **Component Breakdown and Functionality**

#### **Embedded System (Subsystem)**

- **ESP32-S3-WROOM-1** (Hardware Component): The core microcontroller responsible for controlling the RFID scanning, user interaction through buttons and LEDs, and communicating with the backend system. It also handles temperature and humidity data acquisition via the DHT11 sensor.
  
- **PN532 RFID Reader** (Hardware Component): This module scans RFID cards and retrieves user information for clocking in and out. It communicates with the ESP32 via I2C.

- **DHT11 Sensor** (Hardware Component): A temperature and humidity sensor that continuously monitors the environment and sends data to the ESP32.

- **Buzzer/LEDs** (Hardware Components): Provide user feedback during operations, such as successful or failed RFID scans.

- **C/C++ Program** (Software Component): The firmware running on the ESP32 that handles input from the sensors, communication with the web server, and control of the system’s outputs (LEDs, buzzer).

#### **Webserver (Subsystem)**

- **NGINX Web Server**: Acts as a reverse proxy, directing traffic to the backend PHP services that handle API requests.
  
  - **PHP Module** (Software Component): Responsible for processing HTTP requests, interacting with the database, and managing user sessions.

#### **Frontend (Subsystem)**

- **HTML Files** (Software Components): Define the structure of the web interface where users can view their clock-in/clock-out records and session data.

- **CSS Files** (Software Components): Provide the styling and layout for the web interface.

- **JavaScript Files** (Software Components): Handle frontend logic, fetch data from the backend (using AJAX/HTTP requests), and update the page dynamically.

- **Frontend PHP Files** (Software Components): Handle the dynamic generation of the HTML content, ensuring the frontend is connected to the backend API.

#### **Backend (Subsystem)**

- **API PHP Files** (Software Components): These files handle the logic for user authentication, session management, and communication with the embedded ESP32 system. The `api.php` delegates requests to handlers (e.g., `temperature_handler.php`, `card_handler.php`, `session_handler.php`) based on the type of data being processed.

- **Backend Libraries** (Software Components): These libraries provide support for managing sessions, database queries, and user authentication.

#### **Database System (Subsystem)**

- **MariaDB Database**: Stores all user data, RFID card associations, session logs, and temperature readings. Each session includes data like the check-in/check-out time and the number of pitchers.

- **Database Instance**: The actual instance running within a Docker container, handling all the persistent data storage.

#### **Docker Registry (External System)**

- **Docker Images**: Pre-built containers for PHP, MariaDB, and supporting services, facilitating a portable and consistent environment across development and production.

#### **Docker Host (Subsystem)**

- **Docker Network** (Virtual Hardware Component): Provides network communication between containers running the webserver, backend, and database services.

- **Docker Containers** (Virtual Hardware Component): Run the isolated instances of the webserver (NGINX + PHP) and MariaDB database.

#### **Host System (Subsystem)**

- **Linux (or your specific host OS)**: The operating system running the Docker containers and managing the clock-in/clock-out system.

#### **Mobile Device (External System)**

- **Mobile OS (e.g., Android/iOS)**: Allows users to interact with the system via a web browser to view their clock-in and clock-out logs.

- **Browser**: Accesses the frontend web interface for monitoring and interacting with the system.

#### **Network Access Point (External System)**

- **Wi-Fi Access Point**: Connects the ESP32 to the local network, enabling it to communicate with the backend server over HTTP.

#### **Internet/Intranet (Abstraction)**

- **Cloud Symbol**: Represents the internet or intranet that connects the mobile devices and ESP32 to the server.

#### **SSH Tunnel (Subsystem)**

- **SSH Tunnel**: Provides secure remote access to the server, allowing you to manage and monitor the system from external networks.

---

### **Communication Overview**

- **I2C Protocol (ESP32 <-> PN532, DHT11)**: Used for the ESP32 to communicate with the RFID reader and temperature sensor.
  
- **HTTP Protocol (ESP32 <-> Webserver)**: The ESP32 sends RFID data, user credentials, and sensor data via HTTP requests to the PHP backend.
  
- **HTTPS Protocol (Browser <-> Webserver)**: Users access the frontend interface over HTTPS to ensure secure transmission of data.

- **MySQL Protocol (PHP Backend <-> MariaDB)**: Used by the backend PHP files to query and store data in the database.

- **Docker Networking (Internal)**: Communication between Docker containers (NGINX, PHP, MariaDB) happens over an internal network using standard Docker networking mechanisms.
