<?php
require_once 'db_connect.php';

$statement = $dbConnection->prepare("
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
$dbConnection->close();
?>
