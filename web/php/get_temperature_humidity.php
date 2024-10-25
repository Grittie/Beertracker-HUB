<?php
require_once 'db_connect.php';

// Query to get all temperature and humidity data
$statement = $dbConnection->prepare("SELECT temperature, humidity, timestamp FROM InternalTemperature ORDER BY timestamp DESC LIMIT 60");
$statement->execute();
$statement->bind_result($temperature, $humidity, $timestamp);

// Initialize arrays to store the results
$data = array();

while ($statement->fetch()) {
    $data[] = array(
        "timestamp" => $timestamp,
        "temperature" => $temperature,
        "humidity" => $humidity
    );
}

$data = array_reverse($data);

// Check if data is available
if (!empty($data)) {
    echo json_encode(array("success" => true, "data" => $data));
} else {
    echo json_encode(array("success" => false, "message" => "No sensor data available"));
}

$statement->close();
$dbConnection->close();
?>
