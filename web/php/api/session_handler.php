<?php
// Get the current date
$currentDate = date('Y-m-d');

// Get the UID and option from the POST request
$uid = $_POST["uid"] ?? null;
$option = $_POST["option"] ?? null; // Get the action option

// Print debug information
error_log("Received UID: " . json_encode($uid)); // Log the UID
error_log("Received Option: " . json_encode($option)); // Log the option

// Check if UID and option are provided
if (!$uid || $option === null) {
    // If UID or option is not provided, log the error and exit
    error_log("Error: UID or option not provided");
    echo json_encode(array("status" => "error", "message" => "UID or option not provided"));
    exit; // Exit the script to prevent further execution
}

// Step 1: Look for the card UID in the Cards table
$statement = $dbConnection->prepare("SELECT UserID FROM Cards WHERE RFID_Tag = ?");
$statement->bind_param("s", $uid);
$statement->execute();
$statement->store_result();

if ($statement->num_rows > 0) {
    $statement->bind_result($userID);
    $statement->fetch();
    $statement->close();

    // Step 2: Check the user's session status based on the selected option
    if ($option == "0") {
        // Check if a session already exists for today's date
        $statement = $dbConnection->prepare("SELECT SessionID, CheckOutTime FROM Sessions WHERE UserID = ? AND SessionDate = ?");
        $statement->bind_param("is", $userID, $currentDate);
        $statement->execute();
        $statement->store_result();
    
        if ($statement->num_rows > 0) {
            $statement->bind_result($sessionID, $checkOutTime);
            $statement->fetch();
            $statement->close();
    
            if ($checkOutTime === null) {
                // User already checked in
                echo json_encode(array("status" => "error", "message" => "User already checked in"));
            } else {
                // User has checked out, create a new session
                $checkInTime = date('H:i:s'); // Get the current time
                echo json_encode(array("status" => "error", "message" => "User already checked out"));
            }
        } else {
            // No session exists, create a new one and check the user in
            $checkInTime = date('H:i:s'); // Get the current time
            $statement = $dbConnection->prepare("INSERT INTO Sessions (UserID, SessionDate, CheckInTime) VALUES (?, ?, ?)");
            $statement->bind_param("iss", $userID, $currentDate, $checkInTime);
            $statement->execute();
    
            echo json_encode(array("status" => "success", "action" => "checked_in", "message" => "User checked in", "name" => getUserName($dbConnection, $userID)));
        }
    } elseif ($option == "1") {
        // Check if a session already exists for today's date
        $statement = $dbConnection->prepare("SELECT SessionID, CheckOutTime FROM Sessions WHERE UserID = ? AND SessionDate = ?");
        $statement->bind_param("is", $userID, $currentDate);
        $statement->execute();
        $statement->store_result();

        if ($statement->num_rows > 0) {
            $statement->bind_result($sessionID, $checkOutTime);
            $statement->fetch();
            $statement->close();

            if ($checkOutTime === null) {
                // If the user is not checked out, check them out now
                $checkOutTime = date('H:i:s'); // Get the current time
                $statement = $dbConnection->prepare("UPDATE Sessions SET CheckOutTime = ? WHERE SessionID = ?");
                $statement->bind_param("si", $checkOutTime, $sessionID);
                $statement->execute();

                echo json_encode(array("status" => "success", "action" => "checked_out", "message" => "User checked out", "name" => getUserName($dbConnection, $userID)));
            } else {
                echo json_encode(array("status" => "error", "message" => "User already checked out"));
            }
        } else {
            echo json_encode(array("status" => "error", "message" => "User not checked in"));
        }
    } elseif ($option == "2") {
        // Check if a session exists for today's date
        $statement = $dbConnection->prepare("SELECT SessionID, CheckOutTime, Pitchers FROM Sessions WHERE UserID = ? AND SessionDate = ?");
        $statement->bind_param("is", $userID, $currentDate);
        $statement->execute();
        $statement->store_result();

        if ($statement->num_rows > 0) {
            $statement->bind_result($sessionID, $checkOutTime, $pitcherCount);
            $statement->fetch();
            $statement->close();

            // Check if the user is checked in (i.e., CheckOutTime should be null)
            if ($checkOutTime === null) {
                // User is checked in, increment the pitcher count
                $pitcherCount++; // Increment the current pitcher count
                $statement = $dbConnection->prepare("UPDATE Sessions SET Pitchers = ? WHERE SessionID = ?");
                $statement->bind_param("ii", $pitcherCount, $sessionID);
                $statement->execute();

                if ($statement->affected_rows > 0) {
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

// Helper function to get the user's name
function getUserName($dbConnection, $userID) {
    $statement = $dbConnection->prepare("SELECT Name FROM Users WHERE UserID = ?");
    $statement->bind_param("i", $userID);
    $statement->execute();
    $statement->store_result();

    if ($statement->num_rows > 0) {
        $statement->bind_result($userName);
        $statement->fetch();
        $statement->close();
        return $userName;
    }
    return null;
}
?>
