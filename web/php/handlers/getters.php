<?php
class GettersHandler
{
    private $dbConnection;

    public function __construct($dbConnection)
    {
        $this->dbConnection = $dbConnection;
    }

    // GET request to check database connection
    public function handleConnection()
    {
        global $dbHost, $dbUser, $dbName, $directoryPath; // Assuming these are defined in db_connect.php
        echo json_encode(array(
            "success" => true,
            "message" => "Connection successful",
            "host" => $dbHost,
            "user" => $dbUser,
            "database" => $dbName,
            "directoryPath" => $directoryPath
        ));
    }

    // GET request to check the latest heartbeat status
    public function handleHeartbeat()
    {
        $statement = $this->dbConnection->prepare("SELECT timestamp FROM Heartbeats WHERE status = 'Active' ORDER BY timestamp DESC LIMIT 1");
        $statement->execute();
        $statement->bind_result($timestamp);

        if ($statement->fetch()) {
            $currentTime = time();
            $heartbeatTime = strtotime($timestamp);
            $timeDifference = $currentTime - $heartbeatTime;

            if ($timeDifference > 20) {
                echo json_encode(array("success" => true, "status" => "DISCONNECTED"));
            } else {
                echo json_encode(array("success" => true, "status" => "CONNECTED"));
            }
        } else {
            echo json_encode(array("success" => false, "message" => "No active heartbeat data available"));
        }

        $statement->close();
    }

    // GET request to fetch the latest device address
    public function handleDeviceAddress()
    {
        $statement = $this->dbConnection->prepare("SELECT ip, mac FROM DeviceInformation ORDER BY timestamp DESC LIMIT 1");
        $statement->execute();
        $statement->bind_result($ip, $mac);

        if ($statement->fetch()) {
            echo json_encode(array("success" => true, "address" => array("ip" => $ip, "mac" => $mac)));
        } else {
            echo json_encode(array("success" => false, "error" => "No recent address data found"));
        }

        $statement->close();
    }

    // GET request to fetch the latest checked-in user details
    public function handleLatestCheckin()
    {
        $query = "
            SELECT 
                users.UserID, 
                users.name, 
                users.role, 
                cards.RFID_Tag AS card_id, 
                sessions.SessionDate, 
                sessions.CheckInTime
            FROM sessions
            LEFT JOIN users ON sessions.UserID = users.UserID
            LEFT JOIN cards ON users.UserID = cards.userID
            WHERE sessions.CheckInTime IS NOT NULL
            ORDER BY sessions.SessionDate DESC, sessions.CheckInTime DESC
            LIMIT 1";

        $result = $this->dbConnection->query($query);

        if ($result && $result->num_rows > 0) {
            $latestCheckin = $result->fetch_assoc();
            echo json_encode(array("success" => true, "latest_checkin" => $latestCheckin));
        } else {
            echo json_encode(array("success" => false, "error" => "No recent check-in data found"));
        }
    }

    // GET request to check the latest connection status
    public function handleLatestConnection()
    {
        $threshold = 40;

        $query = "SELECT deviceId FROM ConnectionStatus WHERE timestamp < NOW() - INTERVAL ? SECOND AND status = 'connected'";
        $statement = $this->dbConnection->prepare($query);
        $statement->bind_param("i", $threshold);
        $statement->execute();

        $result = $statement->get_result();
        if ($result->num_rows > 0) {
            while ($row = $result->fetch_assoc()) {
                $deviceId = $row['deviceId'];

                $disconnectstatement = $this->dbConnection->prepare("INSERT INTO ConnectionStatus (deviceId, timestamp, status) VALUES (?, NOW(), 'disconnected')");
                $disconnectstatement->bind_param("s", $deviceId);
                $disconnectstatement->execute();
                $disconnectstatement->close();

                echo "Device " . $deviceId . " marked as disconnected.\n";
            }
        } else {
            echo "All devices are up-to-date.\n";
        }

        $statement->close();
    }

    // GET request to fetch the latest temperature and humidity data
    public function handleLatestTemperature()
    {
        $statement = $this->dbConnection->prepare("SELECT temperature, humidity FROM InternalTemperature ORDER BY timestamp DESC LIMIT 1");
        $statement->execute();
        $statement->bind_result($temperature, $humidity);

        if ($statement->fetch()) {
            echo json_encode(array("success" => true, "temperature" => $temperature, "humidity" => $humidity));
        } else {
            echo json_encode(array("success" => false, "message" => "No sensor data available"));
        }

        $statement->close();
    }

    // GET request to fetch all temperature and humidity data
    public function handleTemperatureHumidity()
    {
        $statement = $this->dbConnection->prepare("SELECT temperature, humidity, timestamp FROM InternalTemperature ORDER BY timestamp DESC LIMIT 60");
        $statement->execute();
        $statement->bind_result($temperature, $humidity, $timestamp);

        $data = array();

        while ($statement->fetch()) {
            $formattedTimestamp = date('d/m/Y H:i', strtotime($timestamp));

            $data[] = array(
                "timestamp" => $formattedTimestamp,
                "temperature" => $temperature,
                "humidity" => $humidity
            );
        }

        $data = array_reverse($data);

        if (!empty($data)) {
            echo json_encode(array("success" => true, "data" => $data));
        } else {
            echo json_encode(array("success" => false, "message" => "No sensor data available"));
        }

        $statement->close();
    }

    // GET request to fetch the leaderboard
    public function handleLeaderboard()
    {
        $statement = $this->dbConnection->prepare("
            SELECT u.UserID, u.name, SUM(s.Pitchers) AS total_pitchers, COUNT(s.SessionID) AS sessions_attended
            FROM Users u
            LEFT JOIN Sessions s ON u.UserID = s.UserID
            GROUP BY u.UserID, u.name
            ORDER BY total_pitchers DESC, sessions_attended DESC
            LIMIT 10
        ");
        $statement->execute();
        $statement->bind_result($userID, $name, $totalPitchers, $sessionsAttended);

        $leaderboard = [];
        while ($statement->fetch()) {
            $leaderboard[] = [
                "UserID" => $userID,
                "name" => $name,
                "totalPitchers" => $totalPitchers,
                "sessionsAttended" => $sessionsAttended
            ];
        }

        echo json_encode(["success" => true, "leaderboard" => $leaderboard]);

        $statement->close();
    }

    // GET request to fetch all session data
    public function handleSession()
    {
        $query = "
            SELECT 
                sessions.SessionID,
                sessions.UserID,
                users.name,
                DATE_FORMAT(sessions.SessionDate, '%d/%m/%Y') AS SessionDate,
                DATE_FORMAT(sessions.CheckInTime, '%H:%i') AS CheckInTime,
                DATE_FORMAT(sessions.CheckOutTime, '%H:%i') AS CheckOutTime,
                sessions.TotalHours,
                sessions.Pitchers
            FROM sessions
            LEFT JOIN users ON sessions.UserID = users.UserID
            ORDER BY sessions.SessionDate ASC, sessions.CheckInTime ASC";

        $result = $this->dbConnection->query($query);

        if ($result->num_rows > 0) {
            $sessions = array();
            while ($row = $result->fetch_assoc()) {
                $sessions[] = $row;
            }
            echo json_encode(array(
                "success" => true,
                "sessions" => $sessions
            ));
        } else {
            echo json_encode(array(
                "success" => false,
                "error" => "No session data found"
            ));
        }
    }

    // GET request to fetch all time registration data
    public function handleTimeRegistration()
    {
        $query = "
            SELECT 
                users.UserID, 
                users.name, 
                users.role, 
                cards.RFID_Tag 
            FROM users 
            LEFT JOIN cards ON users.UserID = cards.userID";
        $result = $this->dbConnection->query($query);

        if ($result->num_rows > 0) {
            $users = array();
            while ($row = $result->fetch_assoc()) {
                $users[] = $row;
            }
            echo json_encode(array(
                "success" => true,
                "users" => $users
            ));
        } else {
            echo json_encode(array(
                "success" => false,
                "error" => "No users found"
            ));
        }
    }
    
    // GET request to fetch all user data
    public function handleUsers()
    {
        $statement = $this->dbConnection->prepare("
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
    }

    // GET request to fetch cumulative session data with total amount of sessions, total hour of sessions, longest session, average session length, and most frequently user in sessions
    public function handleCumulativeStatistics()
    {
        $query = "
            SELECT 
                COUNT(SessionID) AS total_sessions,
                SUM(TotalHours) AS total_hours,
                MAX(TotalHours) AS longest_session,
                AVG(TotalHours) AS average_session,
                (SELECT name FROM users WHERE UserID = (
                    SELECT UserID FROM sessions GROUP BY UserID ORDER BY COUNT(UserID) DESC LIMIT 1
                )) AS most_frequent_user
            FROM sessions";

        $result = $this->dbConnection->query($query);

        if ($result->num_rows > 0) {
            $cumulativeData = $result->fetch_assoc();
            echo json_encode(array("success" => true, "cumulative_data" => $cumulativeData));
        } else {
            echo json_encode(array("success" => false, "error" => "No cumulative session data found"));
        }
    }

    // Fetch pitcher statistics with total amount of pitchers ordered, total money spent (pitchers * 14), average pitchers per session (SessionDate, Pitchers), most pitchers in one session (SessionDate, Pitchers), person who has the most pitchers, person with the least pitchers, person with the least pitchers but the most sessions
    public function handlePitcherStatistics()
    {
        $query = "
            SELECT 
                SUM(Pitchers) AS total_pitchers,
                SUM(Pitchers) * 14 AS total_money_spent,
                AVG(Pitchers) AS pitchers_per_session,
                (SELECT SessionDate FROM sessions ORDER BY Pitchers DESC LIMIT 1) AS most_pitchers_session_date,
                (SELECT Pitchers FROM sessions ORDER BY Pitchers DESC LIMIT 1) AS most_pitchers_in_session,
                (SELECT name FROM users WHERE UserID = (
                    SELECT UserID FROM sessions GROUP BY UserID ORDER BY SUM(Pitchers) DESC LIMIT 1
                )) AS most_pitchers,
                (SELECT name FROM users WHERE UserID = (
                    SELECT UserID FROM sessions GROUP BY UserID ORDER BY SUM(Pitchers) ASC LIMIT 1
                )) AS least_pitchers,
                (SELECT name FROM users WHERE UserID = (
                    SELECT UserID FROM sessions GROUP BY UserID ORDER BY COUNT(UserID) ASC LIMIT 1
                )) AS least_pitchers_most_sessions
            FROM sessions";

        $result = $this->dbConnection->query($query);

        if ($result->num_rows > 0) {
            $pitcherData = $result->fetch_assoc();
            echo json_encode(array("success" => true, "pitcher_data" => $pitcherData));
        } else {
            echo json_encode(array("success" => false, "error" => "No pitcher statistics found"));
        }
    }

    // Fetch pitcher consumption data for users and how many pitchers they have consumed
    public function handlePitcherConsumption()
    {
        $query = "
            SELECT 
            users.name,
            SUM(s.Pitchers) AS total_pitchers,
            COUNT(s.SessionID) AS total_sessions
            FROM users
            LEFT JOIN sessions s ON users.UserID = s.UserID
            GROUP BY users.UserID, users.name
            ORDER BY total_pitchers DESC";

        $result = $this->dbConnection->query($query);

        if ($result->num_rows > 0) {
            $pitcherConsumption = array();
            while ($row = $result->fetch_assoc()) {
                $pitcherConsumption[] = $row;
            }
            echo json_encode(array("success" => true, "pitcher_consumption" => $pitcherConsumption));
        } else {
            echo json_encode(array("success" => false, "error" => "No pitcher consumption data found"));
        }
    }
}
?>
