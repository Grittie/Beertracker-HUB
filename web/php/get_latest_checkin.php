<?php
require_once 'db_connect.php';

// Query to fetch the latest checked-in user details based on SessionDate and CheckInTime
$query = "
    SELECT 
        users.UserID, 
        users.name, 
        users.role, 
        cards.RFID_Tag AS card_id, 
        sessions.SessionDate, 
        sessions.CheckInTime
    FROM sessions
    LEFT JOIN users ON sessions.UserID = users.UserID
    LEFT JOIN cards ON users.UserID = cards.userID
    WHERE sessions.CheckInTime IS NOT NULL
    ORDER BY sessions.SessionDate DESC, sessions.CheckInTime DESC
    LIMIT 1";

$result = $dbConnection->query($query);

// Check if the query was successful and returned a result
if ($result && $result->num_rows > 0) {
    $latestCheckin = $result->fetch_assoc();
    echo json_encode(array(
        "success" => true,
        "latest_checkin" => $latestCheckin
    ));
} else {
    echo json_encode(array(
        "success" => false,
        "error" => "No recent check-in data found"
    ));
}

// Close the connection
$dbConnection->close();
?>
