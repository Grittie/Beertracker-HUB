<?php
require_once 'db_connect.php';

// Query to get the latest heartbeat entry with status 'Active'
$statement = $dbConnection->prepare("SELECT timestamp FROM Heartbeats WHERE status = 'Active' ORDER BY timestamp DESC LIMIT 1");
$statement->execute();
$statement->bind_result($timestamp);

// Fetch the data
if ($statement->fetch()) {
    // Get the current time and calculate the difference in seconds
    $currentTime = time();
    $heartbeatTime = strtotime($timestamp);
    $timeDifference = $currentTime - $heartbeatTime;

    // Check if the timestamp is older than 20 seconds
    if ($timeDifference > 20) {
        echo json_encode(array("success" => true, "status" => "DISCONNECTED"));
    } else {
        echo json_encode(array("success" => true, "status" => "CONNECTED"));
    }
} else {
    echo json_encode(array("success" => false, "message" => "No active heartbeat data available"));
}

$statement->close();
$dbConnection->close();
?>
