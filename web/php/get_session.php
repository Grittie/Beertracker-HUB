<?php
require_once 'db_connect.php';

// Query to fetch session data along with user information
$query = "
    SELECT 
        sessions.SessionID,
        sessions.UserID,
        users.name,
        sessions.SessionDate,
        sessions.CheckInTime,
        sessions.CheckOutTime,
        sessions.TotalHours,
        sessions.Pitchers
    FROM sessions
    LEFT JOIN users ON sessions.UserID = users.UserID
    ORDER BY sessions.SessionDate ASC, sessions.CheckInTime ASC";

$result = $dbConnection->query($query);

// Check if the query was successful
if ($result->num_rows > 0) {
    $sessions = array();
    while ($row = $result->fetch_assoc()) {
        $sessions[] = $row;
    }
    echo json_encode(array(
        "success" => true,
        "sessions" => $sessions
    ));
} else {
    echo json_encode(array(
        "success" => false,
        "error" => "No session data found"
    ));
}

// Close the connection
$dbConnection->close();
?>
