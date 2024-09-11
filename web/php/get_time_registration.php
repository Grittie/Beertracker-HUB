<?php
require_once 'db_connect.php';

// Query the database to get user ID, name, role, and cardID
$query = "
    SELECT 
        users.UserID, 
        users.name, 
        users.role, 
        cards.RFID_Tag 
    FROM users 
    LEFT JOIN cards ON users.UserID = cards.userID";
$result = $dbConnection->query($query);

// Check if the query was successful
if ($result->num_rows > 0) {
    $users = array();
    while ($row = $result->fetch_assoc()) {
        $users[] = $row;
    }
    echo json_encode(array(
        "success" => true,
        "users" => $users
    ));
} else {
    echo json_encode(array(
        "success" => false,
        "error" => "No users found"
    ));
}

// Close the connection
$dbConnection->close();
?>
