<?php
    $humidity = $_POST["humidity"];
    $temperature = $_POST["temperature"];

    echo json_encode(array(
        "temperature" => $temperature,
        "humidity" => $humidity
    ));
?>