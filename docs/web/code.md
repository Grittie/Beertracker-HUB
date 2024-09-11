# Web Code

### `index.html`
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Time Registration</title>
    <link rel="stylesheet" href="css/style.css">
    <script src="js/script.js" defer></script>
</head>
<body>
    <p>Database <span id="database-connection"></span> connection.</p>
    <h1>Smart Time Registration</h1>
    <h2>Users Table</h2>
    <table id="users-table">
        <thead>
            <tr>
                <th>ID</th>
                <th>Name</th>
                <th>Role</th>
                <th>Card ID</th>
            </tr>
        </thead>
        <tbody id="users"></tbody>
    </table>
</body>
</html>
```

Sets up the HTML structure with a table to display user information and a paragraph to show the database connection status. It links to an external CSS stylesheet and JavaScript file.


### `script.js`
```javascript
window.addEventListener('load', function() {
    fetchConnectionStatus();
    fetchUserData();
});

function fetchConnectionStatus() {
    fetch('php/connection_status.php')
        .then(response => response.json())
        .then(data => {
            const connectionStatus = document.getElementById('database-connection');
            if (data.success) {
                connectionStatus.innerHTML = `successful (Fetching .env from ${data.directoryPath})`;
                connectionStatus.className = "success";
            } else {
                connectionStatus.innerHTML = `failed: ${data.error}`;
                connectionStatus.className = "failed";
            }
        })
        .catch(error => {
            console.error('Error:', error);
            const connectionStatus = document.getElementById('database-connection');
            connectionStatus.innerHTML = `failed: ${error}`;
            connectionStatus.className = "failed";
        });
}

function fetchUserData() {
    fetch('php/get_time_registration.php')
        .then(response => response.json())
        .then(data => {
            const usersTableBody = document.getElementById('users');
            usersTableBody.innerHTML = '';
            if (data.success) {
                data.users.forEach(user => {
                    const row = document.createElement('tr');
                    row.innerHTML = `<td>${user.UserID}</td><td>${user.name}</td><td>${user.role}</td><td>${user.RFID_Tag}</td>`;
                    usersTableBody.appendChild(row);
                });
            } else {
                const row = document.createElement('tr');
                row.innerHTML = `<td colspan="4">Error: ${data.error}</td>`;
                usersTableBody.appendChild(row);
            }
        })
        .catch(error => {
            console.error('Error:', error);
            const row = document.createElement('tr');
            row.innerHTML = `<td colspan="4">Error: ${error}</td>`;
            document.getElementById('users').appendChild(row);
        });
}
```

JavaScript functions to fetch and display the database connection status and user data. Updates the HTML elements with the status and user information.


### `connection_status.php`
```php
<?php
require_once 'db_connect.php';
echo json_encode(array(
    "success" => true,
    "message" => "Connection successful",
    "host" => $dbHost,
    "user" => $dbUser,
    "database" => $dbName,
    "directoryPath" => $directoryPath
));
$dbConnection->close();
?>
```

Returns a JSON response with the connection status of the database, including host, user, and database information.


### `db_connect.php`
```php
<?php
require_once '/var/www/html/vendor/autoload.php';
use Dotenv\Dotenv;
$directoryPath = __DIR__ . '/../../';
$dotenv = Dotenv::createImmutable($directoryPath);
$dotenv->load();
$dbHost = $_ENV['MYSQL_HOST'];
$dbUser = $_ENV['MYSQL_USER'];
$dbPass = $_ENV['MYSQL_ROOT_PASSWORD'];
$dbName = $_ENV['MYSQL_DB_NAME'];
$dbConnection = new mysqli($dbHost, $dbUser, $dbPass, $dbName);
if ($dbConnection->connect_error) {
    die(json_encode(array(
        "success" => false,
        "error" => $dbConnection->connect_error,
        "host" => $dbHost,
        "user" => $dbUser,
        "database" => $dbName,
        "directoryPath" => $directoryPath
    )));
}
?>
```

Connects to the MySQL database using credentials from a `.env` file. Returns an error message if the connection fails.


### `get_time_registration.php`
```php
<?php
require_once 'db_connect.php';
$query = "SELECT users.UserID, users.name, users.role, cards.RFID_Tag FROM users LEFT JOIN cards ON users.UserID = cards.userID";
$result = $dbConnection->query($query);
if ($result->num_rows > 0) {
    $users = array();
    while ($row = $result->fetch_assoc()) {
        $users[] = $row;
    }
    echo json_encode(array("success" => true, "users" => $users));
} else {
    echo json_encode(array("success" => false, "error" => "No users found"));
}
$dbConnection->close();
?>
```

Fetches user data from the database, including user ID, name, role, and card ID, and returns it as a JSON response.