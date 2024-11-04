<?php
require_once 'db/db_connect.php';
require_once 'handlers/embedded_handler.php'; // Include the new ESP32 request handler
require_once 'handlers/getters.php'; // Include the new getters handler
require_once 'handlers/setters.php'; // Include the new setters handler

$requestMethod = $_SERVER['REQUEST_METHOD'];
$requestUri = explode('/', trim($_SERVER['REQUEST_URI'], '/'));
$type = $requestUri[1] ?? null; // Get the first segment for the type

$EmbeddedHandler = new EmbeddedHandler($dbConnection);
$gettersHandler = new GettersHandler($dbConnection);
$settersHandler = new SettersHandler($dbConnection);

switch ($requestMethod) {
    case 'POST':
        switch ($type) {
            case 'card':
                $EmbeddedHandler->handleCard(); // Make sure this is the correct method name
                break;
            case 'address':
                $EmbeddedHandler->handleDeviceAddress();
                break;
            case 'heartbeat':
                $EmbeddedHandler->handleHeartbeat();
                break;
            case 'connection':
                $EmbeddedHandler->handleConnection();
                break;
            case 'session':
                $EmbeddedHandler->handleSession();
                break;
            case 'temperature':
                $EmbeddedHandler->handleTemperature();
                break;
            case 'add_user':
                $settersHandler->handleAddUser();
                break;
            case 'edit_user':
                $settersHandler->handleEditUser();
                break;
            case 'delete_user':
                $settersHandler->handleDeleteUser();
                break;
            default:
                echo json_encode(["status" => "error", "message" => "Invalid type or missing data"]);
        }
        break;

    case 'GET':
        switch ($type) {
            case 'connection':
                $gettersHandler->handleConnection();
                break;
            case 'heartbeat':
                $gettersHandler->handleHeartbeat();
                break;
            case 'address':
                $gettersHandler->handleDeviceAddress();
                break;
            case 'latest_checkin':
                $gettersHandler->handleLatestCheckin();
                break;
            case 'latest_connection':
                $gettersHandler->handleLatestConnection();
                break;
            case 'latest_temperature':
                $gettersHandler->handleLatestTemperature();
                break;
            case 'temperature_humidity':
                $gettersHandler->handleTemperatureHumidity();
                break;
            case 'leaderboard':
                $gettersHandler->handleLeaderboard();
                break;
            case 'session':
                $gettersHandler->handleSession();
                break;
            case 'time_registration':
                $gettersHandler->handleTimeRegistration();
                break;
            case 'users':
                $gettersHandler->handleUsers();
                break;
            default:
                echo json_encode(["status" => "error", "message" => "Invalid endpoint"]);
        }
        break;

    default:
        echo json_encode(["status" => "error", "message" => "Method not allowed"]);
}

$dbConnection->close();
?>