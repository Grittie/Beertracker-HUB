<?php
// Time threshold in seconds (40 seconds in this case)
$threshold = 40;

// Check the devices that have not updated their status within the threshold
$query = "SELECT deviceId FROM ConnectionStatus WHERE timestamp < NOW() - INTERVAL ? SECOND AND status = 'connected'";
$statement = $dbConnection->prepare($query);
$statement->bind_param("i", $threshold);
$statement->execute();

$result = $statement->get_result();
if ($result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        $deviceId = $row['deviceId'];

        // Insert the "disconnected" status for devices that haven't sent an update
        $disconnectstatement = $dbConnection->prepare("INSERT INTO ConnectionStatus (deviceId, timestamp, status) VALUES (?, NOW(), 'disconnected')");
        $disconnectstatement->bind_param("s", $deviceId);
        $disconnectstatement->execute();
        $disconnectstatement->close();

        echo "Device " . $deviceId . " marked as disconnected.\n";
    }
} else {
    echo "All devices are up-to-date.\n";
}

$statement->close();
?>
