window.addEventListener('load', function () {
    // Fetch connection status and update the UI
    fetchConnectionStatus();

    // Fetch user data and update the UI
    fetchUserData();

    // Fetch sensor data and update the UI
    fetchTemperatureData();

    // Fetch session data and update the UI
    fetchSessionData();

    // Fetch historical temperature and humidity data
    fetchTemperatureHumidityData();

    // Fetch heartbeat status and update the UI
    fetchHeartbeatStatus();
});

// Global variables for session data
let sessionsByDate = {}; // Global variable to hold sessions grouped by date
let latestDate = ''; // To store the latest date for default selection

// Function to fetch the connection status and update the UI
function fetchConnectionStatus() {
    // Fetch the connection status from the server
    fetch('php/get_connection_status.php')
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
            const sessionsContainer = document.getElementById('sessions-body'); // Update to the new tbody ID
            sessionsContainer.innerHTML = ''; // Clear any existing session data
            const dateFilter = document.getElementById('dateFilter');
            dateFilter.innerHTML = ''; // Clear existing options

            if (data.success) {
                // Group sessions by date
                sessionsByDate = {}; // Reset the sessionsByDate object

                data.sessions.forEach(session => {
                    const sessionDate = session.SessionDate;

                    if (!sessionsByDate[sessionDate]) {
                        sessionsByDate[sessionDate] = [];
                    }
                    sessionsByDate[sessionDate].push(session);
                });

                // Populate the date dropdown
                Object.keys(sessionsByDate).forEach(date => {
                    const option = document.createElement('option');
                    option.value = date;
                    option.textContent = date;
                    dateFilter.appendChild(option);
                });

                // Set the latest date as default
                latestDate = Object.keys(sessionsByDate).sort().reverse()[0]; // Get the latest date
                dateFilter.value = latestDate;

                // Render sessions for the latest date by default
                renderSessions(latestDate);
            } else {
                const errorRow = document.createElement('tr');
                const errorCell = document.createElement('td');
                errorCell.colSpan = 8; // Update the colspan to match the new table
                errorCell.textContent = `Error: ${data.error}`;
                sessionsContainer.appendChild(errorRow);
            }
        })
        .catch(error => {
            console.error('Error:', error);
            const errorRow = document.createElement('tr');
            const errorCell = document.createElement('td');
            errorCell.colSpan = 8; // Update the colspan to match the new table
            errorCell.textContent = `Error: ${error}`;
            document.getElementById('sessions-body').appendChild(errorRow);
        });
}

// Function to render sessions based on the selected date
function renderSessions(selectedDate) {
    const sessionsContainer = document.getElementById('sessions-body');
    sessionsContainer.innerHTML = ''; // Clear existing session data

    if (sessionsByDate[selectedDate]) {
        // Create a row for the date (spanning the columns)
        const dateRow = document.createElement('tr');
        const dateCell = document.createElement('td');
        dateCell.colSpan = 8; // Adjust based on the number of columns
        dateCell.textContent = selectedDate;
        dateCell.style.fontWeight = 'bold'; // Make date row bold
        sessionsContainer.appendChild(dateRow);

        // Create rows for each session on that date
        sessionsByDate[selectedDate].forEach(session => {
            const row = document.createElement('tr');

            // Populate row with session data
            const idCell = document.createElement('td');
            idCell.textContent = session.SessionID;
            row.appendChild(idCell);

            const userCell = document.createElement('td');
            userCell.textContent = session.UserID; // Ensure to use UserID from your data
            row.appendChild(userCell);

            const nameCell = document.createElement('td');
            nameCell.textContent = session.name;
            row.appendChild(nameCell);

            const dateCell = document.createElement('td');
            dateCell.textContent = session.SessionDate;
            row.appendChild(dateCell);

            const checkInCell = document.createElement('td');
            checkInCell.textContent = session.CheckInTime;
            row.appendChild(checkInCell);

            const checkOutCell = document.createElement('td');
            checkOutCell.textContent = session.CheckOutTime || 'N/A'; // Handle empty check-out
            row.appendChild(checkOutCell);

            const durationCell = document.createElement('td');
            durationCell.textContent = session.TotalHours;
            row.appendChild(durationCell);

            const pitcherCell = document.createElement('td');
            pitcherCell.textContent = session.Pitchers;
            row.appendChild(pitcherCell);

            sessionsContainer.appendChild(row);
        });
    } else {
        const errorRow = document.createElement('tr');
        const errorCell = document.createElement('td');
        errorCell.colSpan = 8; // Update the colspan to match the new table
        errorCell.textContent = 'No sessions available for this date.';
        sessionsContainer.appendChild(errorRow);
    }
}

// Function to filter sessions based on selected date
function filterSessions() {
    const selectedDate = document.getElementById('dateFilter').value;
    renderSessions(selectedDate);
}

// New function to fetch historical temperature and humidity data
function fetchTemperatureHumidityData() {
    fetch('php/get_temperature_humidity.php')
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                const timestamps = data.data.map(entry => entry.timestamp);
                const temperatures = data.data.map(entry => entry.temperature);
                const humidities = data.data.map(entry => entry.humidity);

                // Call the function to create the chart with the retrieved data
                createTemperatureHumidityChart(timestamps, temperatures, humidities);
            } else {
                console.error("Error fetching temperature and humidity data:", data.message);
            }
        })
        .catch(error => {
            console.error('Error:', error);
        });
}


function createTemperatureHumidityChart(timestamps, temperatures, humidities) {
    const ctx = document.getElementById('temperatureHumidityChart').getContext('2d');
    const chart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: timestamps, // X-axis labels (timestamps)
            datasets: [
                {
                    label: 'Temperature (°C)',
                    data: temperatures,
                    borderColor: 'rgba(255, 99, 132, 1)',
                    fill: false,
                },
                {
                    label: 'Humidity (%)',
                    data: humidities,
                    borderColor: 'rgba(54, 162, 235, 1)',
                    fill: false,
                },
            ]
        },
        options: {
            responsive: true,
            scales: {
                x: {
                    title: {
                        display: true,
                        text: 'Timestamp'
                    }
                },
                y: {
                    title: {
                        display: true,
                        text: 'Value'
                    }
                }
            }
        }
    });
}

function fetchHeartbeatStatus() {
    fetch('php/get_heartbeat.php')
        .then(response => response.json())
        .then(data => {
            const heartbeatStatusDisplay = document.getElementById('heartbeat-status'); // Ensure this element exists in your HTML
            if (data.success) {
                const statusMessage = data.status === "CONNECTED" ? "Connected" : "Disconnected";
                heartbeatStatusDisplay.innerHTML = statusMessage;
                heartbeatStatusDisplay.className = data.status === "CONNECTED" ? "success" : "failed";
            } else {
                heartbeatStatusDisplay.innerHTML = `Error fetching heartbeat status: ${data.message}`;
                heartbeatStatusDisplay.className = "failed";
            }
        })
        .catch(error => {
            console.error('Error fetching heartbeat status:', error);
            const heartbeatStatusDisplay = document.getElementById('heartbeat-status');
            heartbeatStatusDisplay.innerHTML = `Error: ${error}`;
            heartbeatStatusDisplay.className = "failed";
        });
}