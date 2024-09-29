window.addEventListener('load', function() {
    // Fetch connection status and update the UI
    fetchConnectionStatus();

    // Fetch user data and update the UI
    fetchUserData();

    // Fetch sensor data and update the UI
    fetchTemperatureData();

    // Fetch session data and update the UI
    fetchSessionData();
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

function fetchTemperatureData() {
    fetch('php/get_latest_temp.php')
        .then(response => response.json())
        .then(data => {
            const temperatureDisplay = document.getElementById('temperature-display');
            const humidityDisplay = document.getElementById('humidity-display');
            if (data.success) {
                temperatureDisplay.innerHTML = `${data.temperature}`;
                humidityDisplay.innerHTML = `${data.humidity}`;
            } else {
                temperatureDisplay.innerHTML = "Error fetching sensor data";
                humidityDisplay.innerHTML = "Error fetching sensor data";
                console.error("Error fetching sensor data:", data.message);
            }
        })
        .catch(error => {
            console.error('Error:', error);
        });
}


function fetchSessionData() {
    fetch('php/get_session.php')
        .then(response => response.json())
        .then(data => {
            const sessionsContainer = document.getElementById('sessions');
            sessionsContainer.innerHTML = ''; // Clear any existing session data

            if (data.success) {
                const sessionsByDate = {};

                // Group sessions by date
                data.sessions.forEach(session => {
                    const sessionDate = session.SessionDate;

                    if (!sessionsByDate[sessionDate]) {
                        sessionsByDate[sessionDate] = [];
                    }
                    sessionsByDate[sessionDate].push(session);
                });

                // Populate the table with grouped sessions
                Object.keys(sessionsByDate).forEach(date => {
                    // Create a row for the date (spanning the columns)
                    const dateRow = document.createElement('tr');
                    const dateCell = document.createElement('td');
                    dateCell.colSpan = 7; // Adjust based on the number of columns
                    dateCell.textContent = date;
                    dateCell.className = 'session-date'; // Optional: add a class for styling
                    dateRow.appendChild(dateCell);
                    sessionsContainer.appendChild(dateRow);

                    // Create rows for each session on that date
                    sessionsByDate[date].forEach(session => {
                        const row = document.createElement('tr');

                        // Populate row with session data
                        const idCell = document.createElement('td');
                        idCell.textContent = session.SessionID;
                        row.appendChild(idCell);

                        const userCell = document.createElement('td');
                        userCell.textContent = session.name;
                        row.appendChild(userCell);

                        const dateCell = document.createElement('td');
                        dateCell.textContent = session.SessionDate;
                        row.appendChild(dateCell);

                        const checkInCell = document.createElement('td');
                        checkInCell.textContent = session.CheckInTime;
                        row.appendChild(checkInCell);

                        const checkOutCell = document.createElement('td');
                        checkOutCell.textContent = session.CheckOutTime;
                        row.appendChild(checkOutCell);

                        const durationCell = document.createElement('td');
                        durationCell.textContent = session.TotalHours;
                        row.appendChild(durationCell);

                        const pitcherCell = document.createElement('td');
                        pitcherCell.textContent = session.Pitchers;
                        row.appendChild(pitcherCell);

                        sessionsContainer.appendChild(row);
                    });
                });
            } else {
                const errorRow = document.createElement('tr');
                const errorCell = document.createElement('td');
                errorCell.colSpan = 7;
                errorCell.textContent = `Error: ${data.error}`;
                sessionsContainer.appendChild(errorCell);
            }
        })
        .catch(error => {
            console.error('Error:', error);
            const errorRow = document.createElement('tr');
            const errorCell = document.createElement('td');
            errorCell.colSpan = 7;
            errorCell.textContent = `Error: ${error}`;
            document.getElementById('sessions').appendChild(errorRow);
        });
}
