<?php
class EmbeddedHandler
{
    private $dbConnection;

    public function __construct($dbConnection)
    {
        $this->dbConnection = $dbConnection;
    }

    // Handle temperature data (POST /temperature)
    public function handleTemperature()
    {
        $humidity = $_POST["humidity"] ?? null;
        $temperature = $_POST["temperature"] ?? null;
        
        // Check if the temperature and humidity values are valid
        if (is_numeric($temperature) && is_numeric($humidity)) {
        
            date_default_timezone_set('Europe/Amsterdam'); // Set the appropriate timezone
            $timestamp = date('Y-m-d H:i:s'); // Get the current timestamp in PHP
        
            // Insert temperature and humidity data
            $statement = $this->dbConnection->prepare("INSERT INTO InternalTemperature (timestamp, temperature, humidity) VALUES (?, ?, ?)");
            $statement->bind_param("ssd", $timestamp, $temperature, $humidity);
        
            // Execute the statement
            if ($statement->execute()) {
                echo json_encode(array("status" => "success"));
            } else {
                echo json_encode(array("status" => "error", "message" => $statement->error));
            }
        
            // Close the statement
            $statement->close();
        } else {
            echo json_encode(array("status" => "error", "message" => "Invalid temperature or humidity data"));
        }
    }

    // Handle card check-in/out (POST /card)
    public function handleCard()
    {
        $uid = $_POST["uid"] ?? null;
        $option = $_POST["option"] ?? null;

        if ($uid && $option) {
            $statement = $this->dbConnection->prepare("SELECT UserID FROM Cards WHERE RFID_Tag = ?");
            $statement->bind_param("s", $uid);
            $statement->execute();
            $statement->store_result();

            if ($statement->num_rows > 0) {
                $statement->bind_result($userID);
                $statement->fetch();
                $statement->close();

                // Call session handling based on option
                $this->handleSession($userID, $option);
            } else {
                echo json_encode(["status" => "error", "message" => "Card not found"]);
            }
        } else {
            echo json_encode(["status" => "error", "message" => "UID or option not provided"]);
        }
    }

    // Handle device address registration (POST /device)
    public function handleDeviceAddress()
    {
        $ip = $_POST["ip"] ?? null;
        $mac = $_POST["mac"] ?? null;

        if ($ip && $mac) {
            $timestamp = date("Y-m-d H:i:s");
            $statement = $this->dbConnection->prepare("INSERT INTO DeviceInformation (timestamp, ip, mac) VALUES (?, ?, ?)");
            $statement->bind_param("sss", $timestamp, $ip, $mac);

            if ($statement->execute()) {
                echo json_encode(["status" => "success"]);
            } else {
                echo json_encode(["status" => "error", "message" => $statement->error]);
            }
            $statement->close();
        } else {
            echo json_encode(["status" => "error", "message" => "Invalid IP or MAC data"]);
        }
    }

    // Handle heartbeat (POST /heartbeat)
    public function handleHeartbeat()
    {
        $status = $_POST["status"] ?? null;

        if ($status == 'Active') {
            $timestamp = date('Y-m-d H:i:s');
            $statement = $this->dbConnection->prepare("INSERT INTO Heartbeats (timestamp, status) VALUES (?, ?)");
            $statement->bind_param("ss", $timestamp, $status);

            if ($statement->execute()) {
                echo json_encode(["status" => "success"]);
            } else {
                echo json_encode(["status" => "error", "message" => $statement->error]);
            }
            $statement->close();
        } else {
            echo json_encode(["status" => "error", "message" => "Invalid status data"]);
        }
    }

    // Handle connection status (POST /connection)
    public function handleConnection()
    {
        $status = $_POST["status"] ?? null;

        if ($status === "connected" || $status === "disconnected") {
            $timestamp = date('Y-m-d H:i:s');
            $statement = $this->dbConnection->prepare("INSERT INTO ConnectionStatus (timestamp, status) VALUES (?, ?)");
            $statement->bind_param("ss", $timestamp, $status);

            if ($statement->execute()) {
                echo json_encode(["status" => "success"]);
            } else {
                echo json_encode(["status" => "error", "message" => $statement->error]);
            }
            $statement->close();
        } else {
            echo json_encode(["status" => "error", "message" => "Invalid connection status data"]);
        }
    }

    // Handle session operations (POST /session)
    public function handleSession()
    {
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
        $statement = $this->dbConnection->prepare("SELECT UserID FROM Cards WHERE RFID_Tag = ?");
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
                $statement = $this->dbConnection->prepare("SELECT SessionID, CheckOutTime FROM Sessions WHERE UserID = ? AND SessionDate = ?");
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
                    $statement = $this->dbConnection->prepare("INSERT INTO Sessions (UserID, SessionDate, CheckInTime) VALUES (?, ?, ?)");
                    $statement->bind_param("iss", $userID, $currentDate, $checkInTime);
                    $statement->execute();

                    echo json_encode(array("status" => "success", "action" => "checked_in", "message" => "User checked in", "name" => $this->getUserName($userID)));
                }
            } elseif ($option == "1") {
                // Check if a session already exists for today's date
                $statement = $this->dbConnection->prepare("SELECT SessionID, CheckOutTime FROM Sessions WHERE UserID = ? AND SessionDate = ?");
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
                        $statement = $this->dbConnection->prepare("UPDATE Sessions SET CheckOutTime = ? WHERE SessionID = ?");
                        $statement->bind_param("si", $checkOutTime, $sessionID);
                        $statement->execute();

                        echo json_encode(array("status" => "success", "action" => "checked_out", "message" => "User checked out", "name" => $this->getUserName($userID)));
                    } else {
                        echo json_encode(array("status" => "error", "message" => "User already checked out"));
                    }
                } else {
                    echo json_encode(array("status" => "error", "message" => "User not checked in"));
                }
            } elseif ($option == "2") {
                // Check if a session exists for today's date
                $statement = $this->dbConnection->prepare("SELECT SessionID, CheckOutTime, Pitchers FROM Sessions WHERE UserID = ? AND SessionDate = ?");
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
                        $statement = $this->dbConnection->prepare("UPDATE Sessions SET Pitchers = ? WHERE SessionID = ?");
                        $statement->bind_param("ii", $pitcherCount, $sessionID);
                        $statement->execute();

                        if ($statement->affected_rows > 0) {
                            echo json_encode(array("status" => "success", "action" => "added_pitcher", "message" => "Pitcher added", "name" => $this->getUserName($userID), "newPitcherCount" => $pitcherCount));
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

    }

    // Helper function to get the user's name
    private function getUserName($userID)
    {
        // Prepare the SQL statement using the passed UserID
        $statement = $this->dbConnection->prepare("SELECT Name FROM Users WHERE UserID = ?");
        $statement->bind_param("i", $userID); // Bind the userID parameter
        $statement->execute();
        $statement->store_result();

        if ($statement->num_rows > 0) {
            $statement->bind_result($userName); // Bind the result to $userName
            $statement->fetch();
            $statement->close(); // Close the statement
            return $userName; // Return the user's name
        }
        return null; // Return null if no user is found
    }
}
?>