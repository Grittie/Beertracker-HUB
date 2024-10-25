<?php
// Get the connection status and device ID from the POST request
$status = $_POST["status"] ?? null;

// Check if the status and deviceId values are valid
if (($status === "connected" || $status === "disconnected")) {
    // Insert connection status data into the database
    $statement = $dbConnection->prepare("INSERT INTO ConnectionStatus (timestamp, status) VALUES (NOW(), ?)");
    $statement->bind_param("ss", $deviceId, $status);

    // Execute the statement
    if ($statement->execute()) {
        echo json_encode(array("status" => "success"));
    } else {
        echo json_encode(array("status" => "error", "message" => $statement->error));
    }

    // Close the statement
    $statement->close();
} else {
    echo json_encode(array("status" => "error", "message" => "Invalid connection status data or missing deviceId"));
}
?>
