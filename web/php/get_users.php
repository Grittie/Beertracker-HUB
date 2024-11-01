<?php
require_once 'db_connect.php';

// Query to get users and their associated RFID tags
$statement = $dbConnection->prepare("
    SELECT u.UserID, u.name, u.role, c.RFID_Tag 
    FROM users u 
    LEFT JOIN cards c ON u.UserID = c.UserID
");
$statement->execute();
$statement->bind_result($userID, $name, $role, $rfidTag);

$users = [];
while ($statement->fetch()) {
    $users[] = [
        'UserID' => $userID,
        'name' => $name,
        'role' => $role,
        'RFID_Tag' => $rfidTag
    ];
}

echo json_encode(['success' => true, 'users' => $users]);

$statement->close();
$dbConnection->close();
?>
