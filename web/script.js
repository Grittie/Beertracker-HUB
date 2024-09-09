window.addEventListener('load', function() {
    fetch('php/connection.php')
        .then(response => response.json())
        .then(data => {
            const connectionStatus = document.getElementById('database-connection');
            if (data.success) {
                connectionStatus.innerHTML = "successful";
                connectionStatus.className = "success";
            } else {
                connectionStatus.innerHTML = "failed: " + data.error;
                connectionStatus.className = "failed";
            }
        })
        .catch(error => {
            console.error('Error:', error);
            const connectionStatus = document.getElementById('database-connection');
            connectionStatus.innerHTML = "failed: " + error;
            connectionStatus.className = "failed";
        });
});
