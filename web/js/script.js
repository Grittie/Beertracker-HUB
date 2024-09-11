window.addEventListener('load', function() {
    // Fetch connection status and update the UI
    fetchConnectionStatus();

    // Fetch user data and update the UI
    fetchUserData();
});

// Function to fetch the connection status and update the UI
function fetchConnectionStatus() {
    // Fetch the connection status from the server
    fetch('php/connection_status.php')
        .then(response => response.json())
        .then(data => {
            const connectionStatus = document.getElementById('database-connection');
            if (data.success) {
                connectionStatus.innerHTML = `successful (Fetching .env from ${data.directoryPath})`;
                connectionStatus.className = "success";
            } else {
                connectionStatus.innerHTML = `failed: ${data.error} (Host: ${data.host}, User: ${data.user}, Database: ${data.database}, Fetching .env from ${data.directoryPath})`;
                connectionStatus.className = "failed";
            }
        })
        // Catch any errors and log them to the console
        .catch(error => {
            console.error('Error:', error);
            const connectionStatus = document.getElementById('database-connection');
            connectionStatus.innerHTML = `failed: ${error}`;
            connectionStatus.className = "failed";
        });
}

// Function to fetch user data and put it in the user table
function fetchUserData() {
    // Fetch user data from the server
    fetch('php/get_time_registration.php')
        .then(response => response.json())
        .then(data => {
            const usersTableBody = document.getElementById('users');
            usersTableBody.innerHTML = '';

            if (data.success) {
                // Create and append user rows
                data.users.forEach(user => {
                    const row = document.createElement('tr');
                    
                    const idCell = document.createElement('td');
                    idCell.textContent = user.UserID;
                    row.appendChild(idCell);
                    
                    const nameCell = document.createElement('td');
                    nameCell.textContent = user.name;
                    row.appendChild(nameCell);
                    
                    const roleCell = document.createElement('td');
                    roleCell.textContent = user.role;
                    row.appendChild(roleCell);
                    
                    const cardIdCell = document.createElement('td');
                    cardIdCell.textContent = user.RFID_Tag;
                    row.appendChild(cardIdCell);
                    
                    usersTableBody.appendChild(row);
                });
            } else {
                // Display an error message
                const row = document.createElement('tr');
                const cell = document.createElement('td');
                cell.colSpan = 4;
                cell.textContent = `Error: ${data.error}`;
                row.appendChild(cell);
                usersTableBody.appendChild(row);
            }
        })
        // Catch any errors and log them to the console
        .catch(error => {
            console.error('Error:', error);
            const row = document.createElement('tr');
            const cell = document.createElement('td');
            cell.colSpan = 4;
            cell.textContent = `Error: ${error}`;
            row.appendChild(cell);
            document.getElementById('users').appendChild(row);
        });
}

// Call the function to fetch user data
fetchUserData();
