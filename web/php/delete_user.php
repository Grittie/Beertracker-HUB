<?php
require_once 'db_connect.php';

// Get the UserID from the request
$userID = $_POST['userID'] ?? null;

if ($userID) {
    // Prepare the statement to delete the user
    $statement = $dbConnection->prepare("DELETE FROM users WHERE UserID = ?");
    $statement->bind_param('i', $userID);

    if ($statement->execute()) {
        echo json_encode(['success' => true]);
    } else {
        echo json_encode(['success' => false, 'message' => 'Failed to delete user']);
    }

    $statement->close();
} else {
    echo json_encode(['success' => false, 'message' => 'User ID is required']);
}

$dbConnection->close();
?>
