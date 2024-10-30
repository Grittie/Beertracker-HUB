<?php
$status = $_POST["status"] ?? null;

if ($status == 'Active') {

    date_default_timezone_set('Europe/Amsterdam'); // Set the appropriate timezone
    $timestamp = date('Y-m-d H:i:s'); // Get the current timestamp in PHP

    // Insert status data
    $statement = $dbConnection->prepare("INSERT INTO Heartbeats (timestamp, status) VALUES (?, ?)");
    $statement->bind_param("ss", $timestamp, $status);

    // Execute the statement
    if ($statement->execute()) {
        echo json_encode(array("status" => "success"));
    } else {
        echo json_encode(array("status" => "error", "message" => $statement->error));
    }

    // Close the statement
    $statement->close();
} else {
    echo json_encode(array("status" => "error", "message" => "Invalid status data"));
}
?>
