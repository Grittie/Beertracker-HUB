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
    Temperature FLOAT NOT NULL,
    Humidity FLOAT NOT NULL
);

INSERT INTO Users (Name, Role) 
VALUES ('Bob', 'Employee'), ('Alice', 'Manager');

INSERT INTO Cards (UserID, RFID_Tag, Status, IssueDate, ExpiryDate)
VALUES 
(1, '12345ABC', 'Active', '2024-09-08', NULL),
(2, '67890XYZ', 'Active', '2024-09-08', NULL);

INSERT INTO WorkSessions (UserID, ClockInTime, ClockOutTime)
VALUES 
(1, '2024-09-08 08:00:00', '2024-09-08 17:00:00'),
(2, '2024-09-08 09:00:00', '2024-09-08 18:00:00');

INSERT INTO AttendanceLogs (CardID, Timestamp, EventType)
VALUES 
(1, '2024-09-08 08:00:00', 'Clock In'),
(2, '2024-09-08 09:00:00', 'Clock In'),
(1, '2024-09-08 17:00:00', 'Clock Out'),
(2, '2024-09-08 18:00:00', 'Clock Out');
