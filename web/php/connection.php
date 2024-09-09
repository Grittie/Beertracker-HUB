<?php
// Make a connection to your database
$dbConnection = new mysqli("mariadb", "root", "7YKyE8R2AhKzswfN", "smart_time_registration");

// Check the connection
if ($dbConnection->connect_error) {
    echo json_encode(array("success" => false, "error" => $dbConnection->connect_error));
} else {
    echo json_encode(array("success" => true, "message" => "Connection successful"));
}

// Close the connection
$dbConnection->close();
?>
