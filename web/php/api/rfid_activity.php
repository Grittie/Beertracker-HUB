<?php
    $uid = $_POST["uid"];
    $time = $_POST["time"];
    $action = $_POST["time"];

    echo json_encode(array(
        "uid" => $uid,
        "time" => $time,
        "action" => $action
    ));
?>