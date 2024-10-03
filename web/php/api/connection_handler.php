<?php
// Get the connection status and device ID from the POST request
$status = $_POST["status"] ?? null;

// Check if the status and deviceId values are valid
if (($status === "connected" || $status === "disconnected")) {
    // Insert connection status data into the database
    $stmt = $dbConnection->prepare("INSERT INTO ConnectionStatus (timestamp, status) VALUES (NOW(), ?)");
    $stmt->bind_param("ss", $deviceId, $status);

    // Execute the statement
    if ($stmt->execute()) {
        echo json_encode(array("status" => "success"));
    } else {
        echo json_encode(array("status" => "error", "message" => $stmt->error));
    }

    // Close the statement
    $stmt->close();
} else {
    echo json_encode(array("status" => "error", "message" => "Invalid connection status data or missing deviceId"));
}
?>
