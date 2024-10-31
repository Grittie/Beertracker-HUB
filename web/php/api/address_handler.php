<?php
$ip = $_POST["ip"] ?? null;
$mac = $_POST["mac"] ?? null;

if (is_string($ip) && is_string($mac)) {

    // Insert temperature and humidity data
    $statement = $dbConnection->prepare("INSERT INTO DeviceInformation (timestamp, ip, mac) VALUES (?, ?, ?)");
    $statement->bind_param("sss", $timestamp, $ip, $mac);

    // Execute the statement
    if ($statement->execute()) {
        echo json_encode(array("status" => "success"));
    } else {
        echo json_encode(array("status" => "error", "message" => $statement->error));
    }

    // Close the statement
    $statement->close();
} else {
    echo json_encode(array("status" => "error", "message" => "Invalid ip or mac data"));
}
?>
