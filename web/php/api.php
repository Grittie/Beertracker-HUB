<?php
require_once 'db_connect.php';

// Get POST data
$type = $_POST["type"] ?? null;
$humidity = $_POST["humidity"] ?? null;
$temperature = $_POST["temperature"] ?? null;

if ($type === 'sensor_data') {
    // Prepare and bind
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
} else {
    echo json_encode(array("error" => "Invalid type"));
}

// Close the connection
$dbConnection->close();
?>
