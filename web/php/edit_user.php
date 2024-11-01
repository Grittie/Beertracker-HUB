<?php
require_once 'db_connect.php';

// Get the UserID and RFID_Tag from the request
$userID = $_POST['userID'] ?? null;
$name = $_POST['name'] ?? null;
$role = $_POST['role'] ?? null;
$rfidTag = $_POST['rfid_tag'] ?? null;

if ($userID && $name && $role && $rfidTag) {
    // Prepare the statement to update the user and RFID tag
    $statement = $dbConnection->prepare("
        UPDATE users u
        LEFT JOIN cards c ON u.UserID = c.UserID
        SET u.name = ?, u.role = ?, c.RFID_Tag = ?
        WHERE u.UserID = ?
    ");
    $statement->bind_param('sssi', $name, $role, $rfidTag, $userID);

    if ($statement->execute()) {
        echo json_encode(['success' => true]);
    } else {
        echo json_encode(['success' => false, 'message' => 'Failed to update user']);
    }

    $statement->close();
} else {
    echo json_encode(['success' => false, 'message' => 'User ID, name, role, and RFID tag are required']);
}

$dbConnection->close();
?>
