# Web Code

## Introduction
The BeerTracker HUB project is a modern web-based dashboard built to facilitate the tracking and management of user sessions and beer pitcher consumption. This documentation provides a comprehensive overview of how the project is structured, highlighting the various technologies used and how they adhere to the latest standards in web development. It is aimed at developers who wish to understand the codebase or contribute to future development.

### Repository
You can access the code repository [here](https://gitlab.fdmci.hva.nl/IoT/2024-2025-semester-1/individual-project/zuuliiyiizoo76/-/tree/main).

## Front-End Overview
The front-end of BeerTracker HUB is built using HTML, CSS, and JavaScript, following modern web standards to create a responsive and accessible user experience. The front-end is structured into HTML files for the layout, CSS for styling, and JavaScript for interactions.

### HTML
The main structure of the website is contained in the `index.html` file. The HTML is organized to display various dashboard components like session statistics, pitcher activity, the leaderboard, and account management features.

Key features:
- Use of Bootstrap for styling and responsiveness.
- Custom components for different parts of the dashboard, such as sessions, accounts, and the leaderboard.
- Utilization of HTML in PHP files for including common components like navigation and layout elements.

File: [`index.html`](#)

### CSS
The styling is managed through a separate CSS file, `style.css`, which helps ensure a clean separation of concerns. The CSS includes:
- Custom styling for the database connection states, navbar, tables, and various cards used throughout the dashboard.
- The color scheme primarily uses shades of yellow and gold to maintain brand consistency.
- Imported Google Fonts to enhance the visual appeal of the text.

File: [`style.css`](#)

```css
/* Colours for database connection states */
#database-connection.success {
    color: green;
}
#database-connection.failed {
    color: red;
}
```

This snippet demonstrates how different states are highlighted visually for better user experience【14†source】.

### JavaScript
The JavaScript is organized into a separate file, `script.js`, which contains all the logic for interacting with the back-end API. The JavaScript primarily uses the Fetch API to get and send data, ensuring that all data interactions are done asynchronously for better performance.

Key features:
- Functions like `fetchSessionData()`, `fetchTemperatureData()`, and `fetchLeaderboardData()` make API calls to fetch data and update the UI accordingly.
- The JavaScript code adheres to the convention of defining functions to handle specific tasks and grouping data by date, sessions, and other attributes for easier UI updates.
- JSON is used as the primary data format for all requests and responses, following REST API best practices.

File: [`script.js`](#)

```javascript
// Fetch connection status and update the UI
function fetchConnectionStatus() {
    fetch('api/connection')
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
            connectionStatus.innerHTML = `failed: ${error}`;
            connectionStatus.className = "failed";
        });
}
```

The above function is responsible for fetching and displaying the connection status to the back-end, updating the UI with the appropriate success or failure message【13†source】.

## Back-End Overview
The back-end of the BeerTracker HUB project is written in PHP, following an object-oriented programming (OOP) structure. This helps keep the code modular and maintainable.

### API
The API is designed to handle requests from the front-end, interacting with the database to read, insert, update, or delete data. All routes return JSON-formatted responses to ensure consistency and ease of use on the front-end.

Key features:
- Separate handler files for different functionalities (`embedded_handler.php`, `getters.php`, `setters.php`).
- Each route in the API handles specific tasks, such as user management, session tracking, or temperature data retrieval.
- Utilization of PHP's OOP features to create modular, reusable components.

### PHP Conventions
The code follows PHP conventions strictly, ensuring readability and maintainability:
- **Separation of Concerns**: Different aspects of the application logic are handled by separate files.
- **Error Handling**: Errors are caught and returned in JSON format to avoid breaking the application with unexpected HTML output.
- **Object-Oriented Programming**: Classes are used to encapsulate related functionality, making the code more modular.

File: [`db_connect.php`](#)

## Database Integration
The database is connected to the back-end using PHP, and all interactions with it are done through the API. Key features of the integration:
- **Insertions and Updates**: For example, new users can be added through the `add_user` route, and user details can be edited using the `edit_user` route.
- **Data Fetching**: Routes like `get_temperature`, `get_sessions`, and `get_leaderboard` allow the front-end to fetch up-to-date information.
- **Error Handling**: All database interactions are wrapped in try-catch blocks, with errors returned as JSON.

## API Endpoints
The API provides multiple endpoints that the front-end uses to interact with the database. Some of these are:
- `api/session` (GET): Fetches all sessions grouped by date.
- `api/users` (GET): Fetches user data for the account management tab.
- `api/edit_user` (POST): Edits the details of a specific user.
- `api/add_user` (POST): Adds a new user to the database.
- `api/temperature_humidity` (GET): Fetches the historical temperature and humidity data for charting.

All API routes are designed to return JSON-formatted responses, adhering to RESTful standards, and ensuring a clean, consistent interface between the front and back ends.

## Summary
The BeerTracker HUB project follows modern web development standards by leveraging HTML, CSS, and JavaScript for a responsive front-end, while PHP and MySQL are used for a modular, object-oriented back-end. The code is structured to ensure a clear separation of concerns, enhancing maintainability and scalability.

The entire project is aimed at providing users with a seamless experience in tracking sessions, managing users, and analyzing consumption data. All aspects, from front-end UI interactions to back-end database management, are implemented following the latest best practices in the industry.

