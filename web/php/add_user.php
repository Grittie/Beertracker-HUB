<?php

include 'db_connect.php'; // Include your database connection file

$response = ['success' => false, 'message' => ''];

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $name = $_POST['name'] ?? '';
    $role = $_POST['role'] ?? '';
    $rfid_tag = $_POST['rfid_tag'] ?? '';

    // Validate inputs here (e.g., check for empty fields)
    if (empty($name) || empty($role) || empty($rfid_tag)) {
        $response['message'] = 'All fields are required.';
    } else {
        // Prepare and execute the SQL statement for the users table
        $userStatement = $dbConnection->prepare("INSERT INTO users (name, role) VALUES (?, ?)");
        if ($userStatement) {
            $userStatement->bind_param("ss", $name, $role);
            if ($userStatement->execute()) {
                // Get the last inserted UserID
                $userId = $userStatement->insert_id;

                // Now insert the RFID tag into the cards table
                $cardStatement = $dbConnection->prepare("INSERT INTO cards (UserID, RFID_Tag) VALUES (?, ?)");
                if ($cardStatement) {
                    $cardStatement->bind_param("is", $userId, $rfid_tag);
                    if ($cardStatement->execute()) {
                        $response['success'] = true;
                    } else {
                        $response['message'] = 'Failed to add RFID tag.';
                    }
                    $cardStatement->close();
                } else {
                    $response['message'] = 'Database statement preparation for RFID tag failed.';
                }
            } else {
                $response['message'] = 'Failed to add user.';
            }
            $userStatement->close();
        } else {
            $response['message'] = 'Database statement preparation for user failed.';
        }
    }
}

$dbConnection->close();
echo json_encode($response);
?>
