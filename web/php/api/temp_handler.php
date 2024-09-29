<?php
$humidity = $_POST["humidity"] ?? null;
$temperature = $_POST["temperature"] ?? null;

// Check if the temperature and humidity values are valid
if (is_numeric($temperature) && is_numeric($humidity)) {
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
} else {
    echo json_encode(array("status" => "error", "message" => "Invalid temperature or humidity data"));
}
?>
