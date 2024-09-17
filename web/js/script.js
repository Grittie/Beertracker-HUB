window.addEventListener('load', function() {
    // Fetch connection status and update the UI
    fetchConnectionStatus();

    // Fetch user data and update the UI
    fetchUserData();

    // Fetch sensor data and update the UI
    fetchSensorData();
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
        .catch(error => {
            console.error('Error:', error);
            const connectionStatus = document.getElementById('database-connection');
            connectionStatus.innerHTML = `failed: ${error}`;
            connectionStatus.className = "failed";
        });
}

// Function to fetch user data and put it in the user table
function fetchUserData() {
    fetch('php/get_time_registration.php')
        .then(response => response.json())
        .then(data => {
            const usersTableBody = document.getElementById('users');
            usersTableBody.innerHTML = '';

            if (data.success) {
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
                const row = document.createElement('tr');
                const cell = document.createElement('td');
                cell.colSpan = 4;
                cell.textContent = `Error: ${data.error}`;
                row.appendChild(cell);
                usersTableBody.appendChild(row);
            }
        })
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

// Function to fetch sensor data and update the UI
function fetchSensorData() {
    // Create a POST request to send the 'type' as 'sensor_data'
    fetch('php/sensor_data.php', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: new URLSearchParams({
            type: 'sensor_data',
            humidity: 55,  // These are example values, replace with actual sensor data
            temperature: 24
        })
    })
    .then(response => response.json())
    .then(data => {
        const temperatureDisplay = document.getElementById('temperature-display');
        const humidityDisplay = document.getElementById('humidity-display');
        
        if (data.temperature && data.humidity) {
            temperatureDisplay.textContent = data.temperature;
            humidityDisplay.textContent = data.humidity;
        } else {
            temperatureDisplay.textContent = 'N/A';
            humidityDisplay.textContent = 'N/A';
            console.error('Error fetching sensor data:', data.error);
        }
    })
    .catch(error => {
        console.error('Error:', error);
        document.getElementById('temperature-display').textContent = 'N/A';
        document.getElementById('humidity-display').textContent = 'N/A';
    });
}