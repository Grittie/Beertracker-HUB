<?php
require_once 'db_connect.php';

// If the connection was successful, output the success message
echo json_encode(array(
    "success" => true,
    "message" => "Connection successful",
    "host" => $dbHost,
    "user" => $dbUser,
    "database" => $dbName,
    "directoryPath" => $directoryPath
));

// Close the connection
$dbConnection->close();
?>
