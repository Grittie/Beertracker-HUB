<?php

// Set the default timezone
date_default_timezone_set('Europe/Amsterdam');

// Use environment variables
$dbHost = "localhost";
$dbUser = "grittie";
$dbPass = "protector";
$dbName = "smart_time_registration";

// Make a connection to your database
$dbConnection = new mysqli($dbHost, $dbUser, $dbPass, $dbName);

// Check the connection
if ($dbConnection->connect_error) {
    die(json_encode(array(
        "success" => false,
        "error" => $dbConnection->connect_error,
        "host" => $dbHost,
        "user" => $dbUser,
        "database" => $dbName,
        "directoryPath" => $directoryPath
    )));
}
?>
