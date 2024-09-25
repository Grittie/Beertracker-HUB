<?php
require_once 'db_connect.php';

// Get POST data
$type = $_POST["type"] ?? null;
$humidity = $_POST["humidity"] ?? null;
$temperature = $_POST["temperature"] ?? null;
$uid = $_POST["uid"] ?? null;

// Check if the type is temperature
if ($type === 'temperature' && is_numeric($temperature) && is_numeric($humidity)) {
    // Insert temperature and humidity data
    $stmt = $dbConnection->prepare("INSERT INTO InternalTemperature (timestamp, temperature, humidity) VALUES (NOW(), ?, ?)");
    $stmt->bind_param("dd", $temperature, $humidity);

    // Execute the statement
    if ($stmt->execute()) {
        echo json_encode(array("status" => "success"));
    } else {
        echo json_encode(array("status" => "error", "message" => $stmt->error));
    }

    // Close the statement
    $stmt->close();
} elseif ($type === 'card' && $uid) {
    // Look for the card UID in the Cards table
    $stmt = $dbConnection->prepare("SELECT UserID FROM Cards WHERE RFID_Tag = ?");
    $stmt->bind_param("s", $uid);
    $stmt->execute();
    $stmt->store_result();
    
    // If the card UID is found
    if ($stmt->num_rows > 0) {
        $stmt->bind_result($userID);
        $stmt->fetch();
        $stmt->close();

        // Now look up the User Name from the Users table using the UserID
        $stmt = $dbConnection->prepare("SELECT Name FROM Users WHERE ID = ?");
        $stmt->bind_param("i", $userID);
        $stmt->execute();
        $stmt->store_result();
        
        // If the UserID is found in the Users table
        if ($stmt->num_rows > 0) {
            $stmt->bind_result($userName);
            $stmt->fetch();

            // Return the User Name to the ESP32
            echo json_encode(array("status" => "success", "name" => $userName));
        } else {
            echo json_encode(array("status" => "error", "message" => "User not found"));
        }

        // Close the statement
        $stmt->close();
    } else {
        // If the card is not found
        echo json_encode(array("status" => "error", "message" => "Card not found"));
    }
} else {
    echo json_encode(array("error" => "Invalid type or missing data"));
}

// Close the connection
$dbConnection->close();
?>
