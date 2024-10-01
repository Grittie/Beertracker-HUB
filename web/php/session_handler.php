<?php
// Get the current date
$currentDate = date('Y-m-d');

// Get the UID from the POST request
$uid = $_POST["uid"] ?? null;

if ($uid) {
    // Step 1: Look for the card UID in the Cards table
    $stmt = $dbConnection->prepare("SELECT UserID FROM Cards WHERE RFID_Tag = ?");
    $stmt->bind_param("s", $uid);
    $stmt->execute();
    $stmt->store_result();

    if ($stmt->num_rows > 0) {
        $stmt->bind_result($userID);
        $stmt->fetch();
        $stmt->close();

        // Step 2: Check if a session already exists for today's date
        $stmt = $dbConnection->prepare("SELECT SessionID, CheckOutTime FROM Sessions WHERE UserID = ? AND SessionDate = ?");
        $stmt->bind_param("is", $userID, $currentDate);
        $stmt->execute();
        $stmt->store_result();

        if ($stmt->num_rows > 0) {
            // If the session exists, check if the user is checked out (CheckOutTime is not null)
            $stmt->bind_result($sessionID, $checkOutTime);
            $stmt->fetch();
            $stmt->close();

            if ($checkOutTime === null) {
                // If the user is not checked out, check them out now
                $checkOutTime = date('H:i:s'); // Get the current time
                $stmt = $dbConnection->prepare("UPDATE Sessions SET CheckOutTime = ?, TotalHours = TIMESTAMPDIFF(HOUR, CheckInTime, ?) WHERE SessionID = ?");
                $stmt->bind_param("ssi", $checkOutTime, $checkOutTime, $sessionID);
                $stmt->execute();

                echo json_encode(array("status" => "success", "action" => "checked_out", "message" => "User checked out", "name" => getUserName($dbConnection, $userID)));
            } else {
                echo json_encode(array("status" => "error", "message" => "User already checked out"));
            }
        } else {
            // Step 3: If no session exists, create a new one and check the user in
            $checkInTime = date('H:i:s'); // Get the current time
            $stmt = $dbConnection->prepare("INSERT INTO Sessions (UserID, SessionDate, CheckInTime) VALUES (?, ?, ?)");
            $stmt->bind_param("iss", $userID, $currentDate, $checkInTime);
            $stmt->execute();

            echo json_encode(array("status" => "success", "action" => "checked_in", "message" => "User checked in", "name" => getUserName($dbConnection, $userID)));
        }

    } else {
        echo json_encode(array("status" => "error", "message" => "Card not found"));
    }
} else {
    echo json_encode(array("status" => "error", "message" => "UID not provided"));
}

// Helper function to get the user's name
function getUserName($dbConnection, $userID) {
    $stmt = $dbConnection->prepare("SELECT Name FROM Users WHERE UserID = ?");
    $stmt->bind_param("i", $userID);
    $stmt->execute();
    $stmt->store_result();

    if ($stmt->num_rows > 0) {
        $stmt->bind_result($userName);
        $stmt->fetch();
        $stmt->close();
        return $userName;
    }
    return null;
}
?>
