<?php
require_once 'db_connect.php';

// Query to fetch the latest entry from the DeviceInformation table
$result = $dbConnection->query("SELECT ip, mac FROM DeviceInformation ORDER BY timestamp DESC LIMIT 1");

// Check if the query was successful and returned a result
if ($result && $result->num_rows > 0) {
    $address = $result->fetch_assoc();
    echo json_encode(array(
        "success" => true,
        "address" => $address
    ));
} else {
    echo json_encode(array(
        "success" => false,
        "error" => "No recent address data found"
    ));
}

// Close the connection
$dbConnection->close();
?>
