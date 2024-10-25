<?php
require_once 'db_connect.php';

// Query to get the latest temperature and humidity data
$statement = $dbConnection->prepare("SELECT temperature, humidity FROM InternalTemperature ORDER BY timestamp DESC LIMIT 1");
$statement->execute();
$statement->bind_result($temperature, $humidity);

// Fetch the data
if ($statement->fetch()) {
    echo json_encode(array("success" => true, "temperature" => $temperature, "humidity" => $humidity));
} else {
    echo json_encode(array("success" => false, "message" => "No sensor data available"));
}

$statement->close();
$dbConnection->close();
?>
