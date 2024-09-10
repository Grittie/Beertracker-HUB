<?php
require_once '/var/www/html/vendor/autoload.php';

use Dotenv\Dotenv;

// Get the directory path
$directoryPath = __DIR__ . '/../../';

// Load the .env file
$dotenv = Dotenv::createImmutable($directoryPath);
$dotenv->load();

// Use environment variables
$dbHost = $_ENV['MYSQL_HOST'];
$dbUser = $_ENV['MYSQL_USER'];
$dbPass = $_ENV['MYSQL_ROOT_PASSWORD'];
$dbName = $_ENV['MYSQL_DB_NAME'];

// Make a connection to your database
$dbConnection = new mysqli($dbHost, $dbUser, $dbPass, $dbName);

// Check the connection
if ($dbConnection->connect_error) {
    echo json_encode(array(
        "success" => false,
        "error" => $dbConnection->connect_error,
        "host" => $dbHost,
        "user" => $dbUser,
        "database" => $dbName,
        "directoryPath" => $directoryPath
    ));
} else {
    echo json_encode(array(
        "success" => true,
        "message" => "Connection successful",
        "host" => $dbHost,
        "user" => $dbUser,
        "database" => $dbName,
        "directoryPath" => $directoryPath
    ));
}

// Close the connection
$dbConnection->close();
?>
