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

CREATE TABLE Sessions (
    SessionID INT AUTO_INCREMENT PRIMARY KEY,
    UserID INT NOT NULL,
    SessionDate DATE NOT NULL,
    CheckInTime TIME NOT NULL,
    CheckOutTime TIME,
    Pitchers INT DEFAULT 0,
    TotalHours FLOAT GENERATED ALWAYS AS (TIMESTAMPDIFF(SECOND, CONCAT(SessionDate, ' ', CheckInTime), CONCAT(SessionDate, ' ', CheckOutTime)) / 3600) STORED,
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

CREATE TABLE ConnectionStatus (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    status VARCHAR(20)
);

CREATE TABLE Heartbeats (
    HeartbeatID INT AUTO_INCREMENT PRIMARY KEY,
    Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    Status VARCHAR(20) DEFAULT 'Active'
);

CREATE TABLE ActivityLog (
    ActivityID INT AUTO_INCREMENT PRIMARY KEY,
    Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    ActivityType VARCHAR(50) NOT NULL,
    Description VARCHAR(255)
);

CREATE TABLE DeviceInformation (
    ID INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    ip VARCHAR(45) NOT NULL,
    mac VARCHAR(17) NOT NULL
);


INSERT INTO Users (Name, Role) 
VALUES ('Bob', 'Employee'), ('Alice', 'Manager');

INSERT INTO Cards (UserID, RFID_Tag, Status, IssueDate, ExpiryDate)
VALUES 
(1, '12345ABC', 'Active', '2024-09-08', NULL),
(2, '67890XYZ', 'Active', '2024-09-08', NULL);

INSERT INTO Sessions (UserID, SessionDate, CheckInTime, CheckOutTime, Pitchers)
VALUES 
(1, '2024-09-08', '08:00:00', '17:00:00', 0),
(2, '2024-09-08', '09:00:00', '18:00:00', 1);

INSERT INTO AttendanceLogs (CardID, Timestamp, EventType)
VALUES 
(1, '2024-09-08 08:00:00', 'Clock In'),
(2, '2024-09-08 09:00:00', 'Clock In'),
(1, '2024-09-08 17:00:00', 'Clock Out'),
(2, '2024-09-08 18:00:00', 'Clock Out');