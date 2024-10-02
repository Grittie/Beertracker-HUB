<?php
require_once 'db_connect.php';

// Get POST data
$type = $_POST["type"] ?? null;

// Delegate based on the request type
if ($type === 'temperature') {
    require_once 'api/temperature_handler.php';
} elseif ($type === 'card') {
    require_once 'api/card_handler.php';
    require_once 'api/session_handler.php';
} else {
    echo json_encode(array("status" => "error", "message" => "Invalid type or missing data"));
}

// Close the connection
$dbConnection->close();
?>
