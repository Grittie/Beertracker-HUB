<?php
$uid = $_POST["uid"] ?? null;
$option = $_POST["option"] ?? null; // Get the action option

// Check if the UID and option are provided
if ($uid && $option) {
    // Look for the card UID in the Cards table
    $statement = $dbConnection->prepare("SELECT UserID FROM Cards WHERE RFID_Tag = ?");
    $statement->bind_param("s", $uid);
    $statement->execute();
    $statement->store_result();

    // If the card UID is found
    if ($statement->num_rows > 0) {
        $statement->bind_result($userID);
        $statement->fetch();
        $statement->close();

        // Now look up the User Name from the Users table using the UserID
        $statement = $dbConnection->prepare("SELECT Name FROM Users WHERE UserID = ?");
        $statement->bind_param("i", $userID);
        $statement->execute();
        $statement->store_result();

        // If the UserID is found in the Users table
        if ($statement->num_rows > 0) {
            $statement->bind_result($userName);
            $statement->fetch();
            // Return the User Name to the ESP32
            echo json_encode(array("status" => "success", "name" => $userName, "option" => $option));
        } else {
            echo json_encode(array("status" => "error", "message" => "User not found"));
        }

        // Close the statement
        $statement->close();
    } else {
        // If the card is not found
        echo json_encode(array("status" => "error", "message" => "Card not found"));
    }
} else {
    echo json_encode(array("status" => "error", "message" => "UID or option not provided"));
}
?>
