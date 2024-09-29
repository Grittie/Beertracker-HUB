<?php
$uid = $_POST["uid"] ?? null;

// Check if the UID is provided
if ($uid) {
    // Look for the card UID in the Cards table
    $stmt = $dbConnection->prepare("SELECT UserID FROM Cards WHERE RFID_Tag = ?");
    $stmt->bind_param("s", $uid);
    $stmt->execute();
    $stmt->store_result();

    // If the card UID is found
    if ($stmt->num_rows > 0) {
        $stmt->bind_result($userID);
        $stmt->fetch();
        $stmt->close();

        // Now look up the User Name from the Users table using the UserID
        $stmt = $dbConnection->prepare("SELECT Name FROM Users WHERE UserID = ?");
        $stmt->bind_param("i", $userID);
        $stmt->execute();
        $stmt->store_result();

        // If the UserID is found in the Users table
        if ($stmt->num_rows > 0) {
            $stmt->bind_result($userName);
            $stmt->fetch();

            // Return the User Name to the ESP32
            echo json_encode(array("status" => "success", "name" => $userName));
        } else {
            echo json_encode(array("status" => "error", "message" => "User not found"));
        }

        // Close the statement
        $stmt->close();
    } else {
        // If the card is not found
        echo json_encode(array("status" => "error", "message" => "Card not found"));
    }
} else {
    echo json_encode(array("status" => "error", "message" => "UID not provided"));
}
?>
