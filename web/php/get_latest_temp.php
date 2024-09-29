<?php
require_once 'db_connect.php';

// Query to get the latest temperature and humidity data
$stmt = $dbConnection->prepare("SELECT temperature, humidity FROM InternalTemperature ORDER BY timestamp DESC LIMIT 1");
$stmt->execute();
$stmt->bind_result($temperature, $humidity);

// Fetch the data
if ($stmt->fetch()) {
    echo json_encode(array("success" => true, "temperature" => $temperature, "humidity" => $humidity));
} else {
    echo json_encode(array("success" => false, "message" => "No sensor data available"));
}

$stmt->close();
$dbConnection->close();
?>
