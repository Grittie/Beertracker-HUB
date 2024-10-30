<?php
require_once 'db_connect.php';

// Query to get all temperature and humidity data
$statement = $dbConnection->prepare("SELECT temperature, humidity, timestamp FROM InternalTemperature ORDER BY timestamp DESC LIMIT 60");
$statement->execute();
$statement->bind_result($temperature, $humidity, $timestamp);

// Initialize an array to store the results
$data = array();

while ($statement->fetch()) {
    // Format the timestamp to European format
    $formattedTimestamp = date('d/m/Y H:i', strtotime($timestamp));
    
    $data[] = array(
        "timestamp" => $formattedTimestamp,
        "temperature" => $temperature,
        "humidity" => $humidity
    );
}

// Reverse the array to have the oldest data first
$data = array_reverse($data);

// Check if data is available
if (!empty($data)) {
    echo json_encode(array("success" => true, "data" => $data));
} else {
    echo json_encode(array("success" => false, "message" => "No sensor data available"));
}

// Close the statement and database connection
$statement->close();
$dbConnection->close();
?>
