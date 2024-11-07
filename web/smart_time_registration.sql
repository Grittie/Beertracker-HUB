CREATE DATABASE smart_time_registration;
USE smart_time_registration;

CREATE TABLE users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    role VARCHAR(100)
);

CREATE TABLE cards (
    card_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    rfid_tag VARCHAR(255) UNIQUE NOT NULL,
    status VARCHAR(50) DEFAULT 'Active',
    issue_date DATE,
    expiry_date DATE,
    FOREIGN KEY (user_id) REFERENCES users(user_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

CREATE TABLE sessions (
    session_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    session_date DATE NOT NULL,
    check_in_time TIME NOT NULL,
    check_out_time TIME,
    pitchers INT DEFAULT 0,
    total_hours FLOAT GENERATED ALWAYS AS (
        TIMESTAMPDIFF(SECOND, CONCAT(session_date, ' ', check_in_time), CONCAT(session_date, ' ', check_out_time)) / 3600
    ) STORED,
    FOREIGN KEY (user_id) REFERENCES users(user_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

CREATE TABLE attendance_logs (
    log_id INT AUTO_INCREMENT PRIMARY KEY,
    card_id INT NOT NULL,
    timestamp DATETIME NOT NULL,
    event_type ENUM('Clock In', 'Clock Out') NOT NULL,
    FOREIGN KEY (card_id) REFERENCES cards(card_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

CREATE TABLE internal_temperature (
    temp_log_id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME NOT NULL,
    temperature FLOAT NOT NULL,
    humidity FLOAT NOT NULL
);

CREATE TABLE connection_status (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    status VARCHAR(20)
);

CREATE TABLE heartbeats (
    heartbeat_id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    status VARCHAR(20) DEFAULT 'Active'
);

CREATE TABLE activity_log (
    activity_id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    activity_type VARCHAR(50) NOT NULL,
    description VARCHAR(255)
);

CREATE TABLE device_information (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    ip VARCHAR(45) NOT NULL,
    mac VARCHAR(17) NOT NULL
);
