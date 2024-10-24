<?php
// Get the current date
$currentDate = date('Y-m-d');

// Get the UID and option from the POST request
$uid = $_POST["uid"] ?? null;
$option = $_POST["option"] ?? null; // Get the action option

// Print debug information
error_log("Received UID: " . json_encode($uid)); // Log the UID
error_log("Received Option: " . json_encode($option)); // Log the option

if ($uid && $option !== NULL) {
    // Step 1: Look for the card UID in the Cards table
    $stmt = $dbConnection->prepare("SELECT UserID FROM Cards WHERE RFID_Tag = ?");
    $stmt->bind_param("s", $uid);
    $stmt->execute();
    $stmt->store_result();

    if ($stmt->num_rows > 0) {
        $stmt->bind_result($userID);
        $stmt->fetch();
        $stmt->close();

        // Step 2: Check the user's session status based on the selected option
        if ($option == "0") {
            // Check if a session already exists for today's date
            $stmt = $dbConnection->prepare("SELECT SessionID, CheckOutTime FROM Sessions WHERE UserID = ? AND SessionDate = ?");
            $stmt->bind_param("is", $userID, $currentDate);
            $stmt->execute();
            $stmt->store_result();

            if ($stmt->num_rows > 0) {
                $stmt->bind_result($sessionID, $checkOutTime);
                $stmt->fetch();
                $stmt->close();

                if ($checkOutTime === null) {
                    echo json_encode(array("status" => "error", "message" => "User already checked in"));
                } else {
                    // User checked out previously, create a new session
                    $checkInTime = date('H:i:s'); // Get the current time
                    $stmt = $dbConnection->prepare("UPDATE Sessions SET CheckInTime = ? WHERE SessionID = ?");
                    $stmt->bind_param("si", $checkInTime, $sessionID);
                    $stmt->execute();

                    echo json_encode(array("status" => "success", "action" => "checked_in", "message" => "User checked in", "name" => getUserName($dbConnection, $userID)));
                }
            } else {
                // No session exists, create a new one and check the user in
                $checkInTime = date('H:i:s'); // Get the current time
                $stmt = $dbConnection->prepare("INSERT INTO Sessions (UserID, SessionDate, CheckInTime) VALUES (?, ?, ?)");
                $stmt->bind_param("iss", $userID, $currentDate, $checkInTime);
                $stmt->execute();

                echo json_encode(array("status" => "success", "action" => "checked_in", "message" => "User checked in", "name" => getUserName($dbConnection, $userID)));
            }
        } elseif ($option == "1") {
            // Check if a session already exists for today's date
            $stmt = $dbConnection->prepare("SELECT SessionID, CheckOutTime FROM Sessions WHERE UserID = ? AND SessionDate = ?");
            $stmt->bind_param("is", $userID, $currentDate);
            $stmt->execute();
            $stmt->store_result();

            if ($stmt->num_rows > 0) {
                $stmt->bind_result($sessionID, $checkOutTime);
                $stmt->fetch();
                $stmt->close();

                if ($checkOutTime === null) {
                    // If the user is not checked out, check them out now
                    $checkOutTime = date('H:i:s'); // Get the current time
                    $stmt = $dbConnection->prepare("UPDATE Sessions SET CheckOutTime = ? WHERE SessionID = ?");
                    $stmt->bind_param("si", $checkOutTime, $sessionID);
                    $stmt->execute();

                    echo json_encode(array("status" => "success", "action" => "checked_out", "message" => "User checked out", "name" => getUserName($dbConnection, $userID)));
                } else {
                    echo json_encode(array("status" => "error", "message" => "User already checked out"));
                }
            } else {
                echo json_encode(array("status" => "error", "message" => "User not checked in"));
            }
        } elseif ($option == "2") {
            // Check if a session exists for today's date
            $stmt = $dbConnection->prepare("SELECT SessionID, CheckOutTime, Pitchers FROM Sessions WHERE UserID = ? AND SessionDate = ?");
            $stmt->bind_param("is", $userID, $currentDate);
            $stmt->execute();
            $stmt->store_result();
        
            if ($stmt->num_rows > 0) {
                $stmt->bind_result($sessionID, $checkOutTime, $pitcherCount);
                $stmt->fetch();
                $stmt->close();
        
                // Check if the user is checked in (i.e., CheckOutTime should be null)
                if ($checkOutTime === null) {
                    // User is checked in, increment the pitcher count
                    $pitcherCount++; // Increment the current pitcher count
                    $stmt = $dbConnection->prepare("UPDATE Sessions SET Pitchers = ? WHERE SessionID = ?");
                    $stmt->bind_param("ii", $pitcherCount, $sessionID);
                    $stmt->execute();
        
                    if ($stmt->affected_rows > 0) {
                        echo json_encode(array("status" => "success", "action" => "added_pitcher", "message" => "Pitcher added", "name" => getUserName($dbConnection, $userID), "newPitcherCount" => $pitcherCount));
                    } else {
                        echo json_encode(array("status" => "error", "message" => "Failed to add pitcher"));
                    }
                } else {
                    echo json_encode(array("status" => "error", "message" => "User already checked out, cannot add pitcher"));
                }
            } else {
                echo json_encode(array("status" => "error", "message" => "User not checked in, cannot add pitcher"));
            }
        } else {
            echo json_encode(array("status" => "error", "message" => "Invalid option"));
        }
    } else {
        echo json_encode(array("status" => "error", "message" => "Card not found"));
    }
} else {
    var_dump($_POST["uid"], $_POST["option"]);

    // If UID or option is not provided, log the error
    echo json_encode(array("status" => "error", "message" => "UID or option not provided"));
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