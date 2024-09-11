# Technical Documentation

This documentation outlines the implementation of the website for the Smart Time Registration project. The website serves as a front-end interface to interact with the time registration system, allowing users to view their registration details and check the database connection status.

## Components
### HTML Structure:

Provides the foundational structure of the web page.
Includes a table to display user data.
Uses external stylesheets and scripts for presentation and functionality.

### CSS:

Styles the page for a consistent and user-friendly appearance.

### JavaScript:

Handles data fetching and dynamic content updates.
Manages the interaction between the front end and the back end

### PHP

The PHP scripts handle the server-side operations for the Smart Time Registration project. They manage interactions with the database and provide data to the front end.

<br>

#### `db_connect.php`

**Purpose**

- This script establishes a connection to the MySQL database. It centralizes the database connection logic, making it easier to manage and update the connection settings.

**Functionality**:

  - Connects to the MySQL database using credentials defined in the `.env` file.
  - Checks for connection errors and returns an appropriate error message if the connection fails.
  - Provides a reusable connection object or resource that other PHP scripts can use to interact with the database.

<br>

#### `connection_status.php`

**Purpose**:

  - This script checks the status of the database connection and returns whether the connection is successful or not.

**Functionality**:

  - Uses the `db_connect.php` script to establish a database connection.
  - Performs a simple query or operation to verify that the connection is active.
  - Returns a JSON response indicating the connection status, which can be used by the front end to display messages about the database connection.

<br>

#### `get_time_registration.php`

**Purpose**: 

- This script retrieves user data from the database and provides it in JSON format to be used by the front end.

**Functionality**:

  - Connects to the database using `db_connect.php`.
  - Executes a SQL query to fetch user data, including fields such as user ID, name, role, and card ID.
  - Handles any errors that occur during data retrieval.
  - Encodes the results as a JSON object and returns it to the client-side JavaScript for dynamic updates to the webpage.