<?php
// Debugging: Print all POST data
error_log(print_r($_POST, true));  // Logs POST data to the server's error log

// Check if the 'type' key is set in the POST data
if (!isset($_POST["type"])) {
    echo json_encode(array(
        "error" => "Missing 'type' parameter"
    ));
    exit();
}

// Get the request type to determine which data is being sent
$type = $_POST["type"];

switch($type) {
    case 'sensor_data':
        // Check if 'humidity' and 'temperature' are set
        if (isset($_POST["humidity"]) && isset($_POST["temperature"])) {
            $humidity = $_POST["humidity"];
            $temperature = $_POST["temperature"];
            echo json_encode(array(
                "temperature" => $temperature,
                "humidity" => $humidity
            ));
        } else {
            echo json_encode(array(
                "error" => "Missing 'humidity' or 'temperature' parameter"
            ));
        }
        break;

    case 'time_registration':
        // Check if 'uid', 'time', and 'action' are set
        if (isset($_POST["uid"]) && isset($_POST["time"]) && isset($_POST["action"])) {
            $uid = $_POST["uid"];
            $time = $_POST["time"];
            $action = $_POST["action"];
            echo json_encode(array(
                "uid" => $uid,
                "time" => $time,
                "action" => $action
            ));
        } else {
            echo json_encode(array(
                "error" => "Missing 'uid', 'time', or 'action' parameter"
            ));
        }
        break;

    default:
        // If no valid type is given, return an error
        echo json_encode(array(
            "error" => "Invalid request type"
        ));
        break;
}
?>
