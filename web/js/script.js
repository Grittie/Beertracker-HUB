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

    // Fetch latest check-in data and update the UI
    fetchLatestCheckin();

    // Fetch latest address data and update the UI
    fetchAddress();

    // Fetch leaderboard data and update the UI
    fetchLeaderboardData();

    // Fetch cumulative statistics and update the UI
    fetchCumulativeStatistics();

    // Fetch pitcher statistics and update the UI
    fetchPitcherStatistics();

    // Fetch pitcher consumption data and create a bar chart
    fetchPitcherConsumptionData();

    // Set intervals for periodic updates
    setInterval(fetchConnectionStatus, 5000); // Fetch connection status every 5 seconds
    setInterval(fetchUserData, 5000); // Fetch user data every 5 seconds
    setInterval(fetchTemperatureData, 5000); // Fetch temperature data every 5 seconds
    setInterval(fetchSessionData, 5000); // Fetch session data every 5 seconds
    setInterval(fetchTemperatureHumidityData, 5000); // Fetch temperature and humidity data every 5 seconds
    setInterval(fetchHeartbeatStatus, 5000); // Fetch heartbeat status every 5 seconds
    setInterval(fetchLatestCheckin, 5000); // Fetch latest check-in every 5 seconds
    setInterval(fetchAddress, 5000); // Fetch address every 5 seconds
    setInterval(fetchLeaderboardData, 5000); // Fetch leaderboard data every 5 seconds
    setInterval(fetchCumulativeStatistics, 5000); // Fetch cumulative statistics every 5 seconds
    setInterval(fetchPitcherStatistics, 5000); // Fetch pitcher statistics every 5 seconds
    setInterval(fetchPitcherConsumptionData, 5000); // Fetch pitcher consumption data every 5 seconds
    setInterval(createTemperatureHumidityChart, 5000); // Create temperature and humidity chart every 5 seconds
    setInterval(createPitcherConsumptionChart, 5000); // Create pitcher consumption chart every 5 seconds
});

// Global variables for session data
let sessionsByDate = {}; // Global variable to hold sessions grouped by date
let latestDate = ''; // To store the latest date for default selection

// Global variable for selected date
let selectedDate = null; // Variable to hold the selected date

// Global variables for charts
let temperatureHumidityChart; // Declare variable to hold chart instance
let pitcherConsumptionChart; // Declare variable to hold chart instance

// Global variable for ESP32 IP address
let esp32IpAddress = ''; // Variable to hold the ESP32 IP address

// Function to fetch the connection status and update the UI
function fetchConnectionStatus() {
    // Fetch the connection status from the server
    fetch('api/connection')
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

function fetchTemperatureData() {
    fetch('api/latest_temperature')
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
    // Save the current selected date before fetching new data
    const dateFilter = document.getElementById('dateFilter');
    selectedDate = dateFilter.value;

    fetch('api/session')
        .then(response => response.json())
        .then(data => {
            const sessionsContainer = document.getElementById('sessions-body'); // Update to the new tbody ID
            sessionsContainer.innerHTML = ''; // Clear any existing session data
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

// Function to convert date from DD/MM/YYYY to a Date object
function parseDate(dateStr) {
    const parts = dateStr.split('/');
    return new Date(parts[2], parts[1] - 1, parts[0]); // Year, Month (0-indexed), Day
}

// Set the latest date as default if no previous selection exists
if (!selectedDate || !sessionsByDate[selectedDate]) {
    // Create an array of dates from the sessionsByDate keys, parsed as Date objects
    const datesArray = Object.keys(sessionsByDate).map(dateStr => ({
        original: dateStr,
        date: parseDate(dateStr)
    }));

    // Sort the array based on the Date objects in descending order
    datesArray.sort((a, b) => b.date - a.date); // Sort descending

    // Get the latest date in the original format
    selectedDate = datesArray[0].original;
}

// Set the selected date back to the dropdown
dateFilter.value = selectedDate;

// Render sessions for the selected date
renderSessions(selectedDate);

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
    fetch('api/temperature_humidity')
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                const timestamps = data.data.map(entry => entry.timestamp);
                const temperatures = data.data.map(entry => entry.temperature);
                const humidities = data.data.map(entry => entry.humidity);

                // Call the function to create or update the chart with the retrieved data
                updateTemperatureHumidityChart(timestamps, temperatures, humidities);
            } else {
                console.error("Error fetching temperature and humidity data:", data.message);
            }
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

// Create or update the temperature and humidity chart
function updateTemperatureHumidityChart(timestamps, temperatures, humidities) {
    if (!temperatureHumidityChart) {
        // Create the chart if it doesn't exist
        const ctx = document.getElementById('temperatureHumidityChart').getContext('2d');
        temperatureHumidityChart = new Chart(ctx, {
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
    } else {
        // Update the chart data if it already exists
        temperatureHumidityChart.data.labels = timestamps;
        temperatureHumidityChart.data.datasets[0].data = temperatures;
        temperatureHumidityChart.data.datasets[1].data = humidities;
        temperatureHumidityChart.update(); // Call update to redraw the chart
    }
}

function fetchHeartbeatStatus() {
    fetch('api/heartbeat')
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

function fetchLatestCheckin() {
    fetch('api/latest_checkin')
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                // Update the HTML elements with the latest check-in data
                document.getElementById('latest-check-in-name').textContent = data.latest_checkin.name;
                document.getElementById('latest-check-in-role').textContent = data.latest_checkin.role;
                document.getElementById('latest-check-in-card-id').textContent = data.latest_checkin.card_id;
                document.getElementById('latest-check-in-date').textContent = data.latest_checkin.SessionDate;
                document.getElementById('latest-check-in-time').textContent = data.latest_checkin.CheckInTime;
            } else {
                console.error("Error fetching latest check-in:", data.message);
            }
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

function fetchAddress() {
    fetch('api/address') // Update the URL to your actual PHP file
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                // Update the HTML elements with the latest address information
                document.getElementById('ip-address').textContent = data.address.ip;
                document.getElementById('mac-address').textContent = data.address.mac;

                // Store the IP address in the global variable
                esp32IpAddress = data.address.ip;
            } else {
                console.error("Error fetching address:", data.error);
            }
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

function fetchLeaderboardData() {
    fetch('api/leaderboard')
        .then(response => response.json())
        .then(data => {
            const leaderboardBody = document.getElementById('leaderboard-body');
            leaderboardBody.innerHTML = ''; // Clear existing leaderboard data

            if (data.success) {
                data.leaderboard.forEach((user, index) => {
                    const row = document.createElement('tr');

                    const rankCell = document.createElement('td');
                    rankCell.textContent = index + 1;
                    row.appendChild(rankCell);

                    const userIDCell = document.createElement('td');
                    userIDCell.textContent = user.UserID;
                    row.appendChild(userIDCell);

                    const nameCell = document.createElement('td');
                    nameCell.textContent = user.name;
                    row.appendChild(nameCell);

                    const totalPitchersCell = document.createElement('td');
                    totalPitchersCell.textContent = user.totalPitchers;
                    row.appendChild(totalPitchersCell);

                    const sessionsAttendedCell = document.createElement('td');
                    sessionsAttendedCell.textContent = user.sessionsAttended;
                    row.appendChild(sessionsAttendedCell);

                    leaderboardBody.appendChild(row);
                });
            } else {
                const errorRow = document.createElement('tr');
                const errorCell = document.createElement('td');
                errorCell.colSpan = 5;
                errorCell.textContent = `Error: ${data.message}`;
                errorRow.appendChild(errorCell);
                leaderboardBody.appendChild(errorRow);
            }
        })
        .catch(error => {
            console.error('Error:', error);
            const errorRow = document.createElement('tr');
            const errorCell = document.createElement('td');
            errorCell.colSpan = 5;
            errorCell.textContent = `Error: ${error}`;
            leaderboardBody.appendChild(errorRow);
        });
}

// Function to fetch user data and put it in the user table
function fetchUserData() {
    fetch('api/users')
        .then(response => response.json())
        .then(data => {
            const usersTableBody = document.getElementById('account-management-body');
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

                    // Create an edit button
                    const editButton = document.createElement('button');
                    editButton.textContent = 'Edit';
                    editButton.className = 'btn btn-warning btn-sm';
                    editButton.style.marginRight = '5px';
                    editButton.onclick = function() {
                        openEditModal(user);
                    };
                    const actionsCell = document.createElement('td');
                    actionsCell.appendChild(editButton);
                    row.appendChild(actionsCell);

                    // Create a delete button
                    const deleteButton = document.createElement('button');
                    deleteButton.textContent = 'Delete';
                    deleteButton.className = 'btn btn-danger btn-sm';
                    deleteButton.onclick = function() {
                        deleteUser(user.UserID);
                    };
                    actionsCell.appendChild(deleteButton);

                    usersTableBody.appendChild(row);
                });
            } else {
                const row = document.createElement('tr');
                const cell = document.createElement('td');
                cell.colSpan = 5; // Update for the number of columns
                cell.textContent = `Error: ${data.message}`;
                row.appendChild(cell);
                usersTableBody.appendChild(row);
            }
        })
        .catch(error => {
            console.error('Error:', error);
            const row = document.createElement('tr');
            const cell = document.createElement('td');
            cell.colSpan = 5; // Update for the number of columns
            cell.textContent = `Error: ${error}`;
            row.appendChild(cell);
            document.getElementById('account-management-body').appendChild(row);
        });
}

document.getElementById('addUserForm').addEventListener('submit', function(event) {
    event.preventDefault();
    
    const name = document.getElementById('newUserName').value;
    const role = document.getElementById('newUserRole').value;
    const rfidTag = document.getElementById('newRFID').value;

    fetch('api/add_user', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: new URLSearchParams({
            name: name,
            role: role,
            rfid_tag: rfidTag // Include the RFID Tag in the body
        })
    })
    .then(response => response.json())
    .then(data => {
        if (data.success) {
            fetchUserData(); // Refresh the user data after adding
            const modal = bootstrap.Modal.getInstance(document.getElementById('addUserModal'));
            modal.hide(); // Hide the modal
            document.getElementById('addUserForm').reset(); // Reset the form
        } else {
            alert(`Error: ${data.message}`);
        }
    })
    .catch(error => {
        console.error('Error:', error);
        alert(`Error adding user: ${error}`);
    });
});


// Function to open the edit modal with user data
function openEditModal(user) {
    document.getElementById('editUserID').value = user.UserID;
    document.getElementById('editName').value = user.name;
    document.getElementById('editRole').value = user.role;
    document.getElementById('editRFID').value = user.RFID_Tag; // Populate RFID Tag

    const modal = new bootstrap.Modal(document.getElementById('editUserModal'));
    modal.show();
}

// Event listener for the edit form submission
document.getElementById('editUserForm').addEventListener('submit', function(event) {
    event.preventDefault();
    
    const userID = document.getElementById('editUserID').value;
    const name = document.getElementById('editName').value;
    const role = document.getElementById('editRole').value;
    const rfidTag = document.getElementById('editRFID').value; // Get the RFID Tag value

    fetch('api/edit_user', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: new URLSearchParams({
            userID: userID,
            name: name,
            role: role,
            rfid_tag: rfidTag // Include the RFID Tag in the body
        })
    })
    .then(response => response.json())
    .then(data => {
        if (data.success) {
            fetchUserData(); // Refresh the user data after editing
            const modal = bootstrap.Modal.getInstance(document.getElementById('editUserModal'));
            modal.hide(); // Hide the modal
        } else {
            alert(`Error: ${data.message}`);
        }
    })
    .catch(error => {
        console.error('Error:', error);
        alert(`Error updating user: ${error}`);
    });
});


// Function to delete a user
function deleteUser(userID) {
    if (confirm("Are you sure you want to delete this user?")) {
        fetch('api/delete_user', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: new URLSearchParams({
                userID: userID
            })
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                fetchUserData(); // Refresh the user data after deletion
            } else {
                alert(`Error: ${data.message}`);
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert(`Error deleting user: ${error}`);
        });
    }
}

// function to fetch cumulative session data
function fetchCumulativeStatistics() {
    fetch('api/cumulative_statistics')
        .then(response => response.json())
        .then(data => {
            const totalSessions = document.getElementById('total-sessions');
            const totalHours = document.getElementById('total-hours');
            const longestSession = document.getElementById('longest-session');
            const averageSession = document.getElementById('average-session');
            const mostFreqUser = document.getElementById('most-frequent-user');

            if (data.success) {
                totalSessions.innerHTML = data.cumulative_data.total_sessions;
                totalHours.innerHTML = parseFloat(data.cumulative_data.total_hours).toFixed(2);
                longestSession.innerHTML = parseFloat(data.cumulative_data.longest_session).toFixed(2);
                averageSession.innerHTML = parseFloat(data.cumulative_data.average_session).toFixed(2);
                mostFreqUser.innerHTML = data.cumulative_data.most_frequent_user;
            } else {
                console.error("Error fetching cumulative statistics:", data.message);
            }
        })
        .catch(error => {
            console.error('Error:', error);
        });
}


// function to fetch pitcher statistics
function fetchPitcherStatistics() {
    fetch('api/pitcher_statistics')
        .then(response => response.json())
        .then(data => {
            const totalPitchers = document.getElementById('total-pitchers');
            const totalMoneySpent = document.getElementById('total-money-spent');
            const pitchersPerSession = document.getElementById('pitchers-per-session');
            const mostPitchers = document.getElementById('most-pitchers');
            const leastPitchers = document.getElementById('least-pitchers');
            const nextPitchers = document.getElementById('next-pitcher');

            if (data.success) {
                totalPitchers.innerHTML = data.pitcher_data.total_pitchers;
                totalMoneySpent.innerHTML = parseFloat(data.pitcher_data.total_money_spent).toFixed(2);
                pitchersPerSession.innerHTML = parseFloat(data.pitcher_data.pitchers_per_session).toFixed(2);
                mostPitchers.innerHTML = data.pitcher_data.most_pitchers;
                leastPitchers.innerHTML = data.pitcher_data.least_pitchers;
                nextPitchers.innerHTML = data.pitcher_data.least_pitchers_most_sessions;
            } else {
                console.error("Error fetching pitcher statistics:", data.message);
            }
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

function fetchPitcherConsumptionData() {
    fetch('api/pitcher_consumption')
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                const labels = data.pitcher_consumption.map(entry => entry.name);
                const consumption = data.pitcher_consumption.map(entry => parseInt(entry.total_pitchers, 10));
                const sessions = data.pitcher_consumption.map(entry => parseInt(entry.total_sessions, 10));
                
                // Update the chart with new data
                updatePitcherConsumptionChart(labels, consumption, sessions);
            } else {
                console.error("Error fetching pitcher consumption data:", data.message);
            }
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

// Create the chart if it doesn't exist, or update it if it does
function updatePitcherConsumptionChart(labels, consumption, sessions) {
    if (!pitcherConsumptionChart) {
        // Create the chart if it doesn't exist
        const ctx = document.getElementById('pitcherConsumptionChart').getContext('2d');
        pitcherConsumptionChart = new Chart(ctx, {
            type: 'bar',
            data: {
                labels: labels,
                datasets: [{
                    label: 'Total Pitchers',
                    data: consumption,
                    backgroundColor: 'rgba(255, 206, 86, 0.2)',
                    borderColor: 'rgba(255, 206, 86, 1)',
                    borderWidth: 1
                },
                {
                    label: 'Total Sessions',
                    data: sessions,
                    backgroundColor: 'rgba(54, 162, 235, 0.2)',
                    borderColor: 'rgba(54, 162, 235, 1)',
                    borderWidth: 1
                }]
            },
            options: {
                responsive: true,
                scales: {
                    x: {
                        title: {
                            display: true,
                            text: 'User'
                        }
                    },
                    y: {
                        beginAtZero: true,
                        title: {
                            display: true,
                            text: 'Count'
                        },
                        ticks: {
                            stepSize: 1,
                            precision: 0
                        }
                    }
                }
            }
        });
    } else {
        // Update the chart data if it already exists
        pitcherConsumptionChart.data.labels = labels;
        pitcherConsumptionChart.data.datasets[0].data = consumption;
        pitcherConsumptionChart.data.datasets[1].data = sessions;
        pitcherConsumptionChart.update(); // Call update to redraw the chart
    }
}

// Function to create the temperature and humidity chart
document.getElementById('reset-btn').addEventListener('click', function() {
    if (esp32IpAddress) {
        fetch(`http://${esp32IpAddress}/reset`, { method: 'POST' })
            .then(response => {
                if (response.ok) {
                    alert("ESP32 is resetting...");
                } else {
                    alert("Failed to send reset command.");
                }
            })
            .catch(error => {
                console.error("Error:", error);
                alert("Failed to connect to ESP32.");
            });
    } else {
        alert("ESP32 IP address is not available.");
    }
});