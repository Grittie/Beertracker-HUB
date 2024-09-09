# Database

I have created an ERD using mermaid that can create visuals from text input, both text input and visuals are added below.

## ERD
<img src="../../assets/images/web/database/ERD.png" alt="Datasheet of the LCD" width="500" />

````mermaid
erDiagram
    USERS {
        inUserID PK
        string Name
        string Role
    }
    CARDS {
        int CardID PK
        int UserID FK
        string RFID_Tag
        string Status
        date IssueDate
        date ExpiryDate
    }
    WORK_SESSIONS {
        int SessionID PK
        int UserID FK
        datetime ClockInTime
        datetime ClockOutTime
        float TotalHours
    }
    ATTENDANCE_LOGS {
        int LogID PK
        int CardID FK
        datetime Timestamp
        string EventType
    }
    INTERNAL_TEMPERATURE {
        int TempLogID PK
        datetime Timestamp
        float Temperature
    }

    USERS ||--o{ CARDS : "owns"
    CARDS ||--o{ ATTENDANCE_LOGS : "logs"
    USERS ||--o{ WORK_SESSIONS : "has"
    WORK_SESSIONS ||--o{ ATTENDANCE_LOGS : "records"
````

## EER
Using mysqldatabase I've created an EER with all the proper fields, relations and specifiers

<img src="../../assets/images/web/database/EER.png" alt="Datasheet of the LCD" width="500" />

## SQL
From the previously made EER I can generate a mysql querry to create the database.

```` mysql
CREATE DATABASE smart_time_registration;
USE smart_time_registration;

CREATE TABLE Users (
    UserID INT AUTO_INCREMENT PRIMARY KEY,
    Name VARCHAR(255) NOT NULL,
    Role VARCHAR(100)
);

CREATE TABLE Cards (
    CardID INT AUTO_INCREMENT PRIMARY KEY,
    UserID INT NOT NULL,
    RFID_Tag VARCHAR(255) UNIQUE NOT NULL,
    Status VARCHAR(50) DEFAULT 'Active',
    IssueDate DATE,
    ExpiryDate DATE,
    FOREIGN KEY (UserID) REFERENCES Users(UserID)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

CREATE TABLE WorkSessions (
    SessionID INT AUTO_INCREMENT PRIMARY KEY,
    UserID INT NOT NULL,
    ClockInTime DATETIME NOT NULL,
    ClockOutTime DATETIME,
    TotalHours FLOAT GENERATED ALWAYS AS (TIMESTAMPDIFF(SECOND, ClockInTime, ClockOutTime) / 3600) STORED,
    FOREIGN KEY (UserID) REFERENCES Users(UserID)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

CREATE TABLE AttendanceLogs (
    LogID INT AUTO_INCREMENT PRIMARY KEY,
    CardID INT NOT NULL,
    Timestamp DATETIME NOT NULL,
    EventType ENUM('Clock In', 'Clock Out') NOT NULL,
    FOREIGN KEY (CardID) REFERENCES Cards(CardID)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

CREATE TABLE InternalTemperature (
    TempLogID INT AUTO_INCREMENT PRIMARY KEY,
    Timestamp DATETIME NOT NULL,
    Temperature FLOAT NOT NULL
);
````