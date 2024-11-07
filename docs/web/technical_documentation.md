# Technical Documentation

This documentation outlines the implementation of the website for the Smart Time Registration project. The website serves as a front-end interface to interact with the time registration system, allowing users to view their registration details and check the database connection status.

## Design Decisions

This project was designed with modularity, scalability, and user-friendliness in mind. The key design decisions include:

- **Separation of Concerns**: HTML, CSS, and JavaScript are kept in separate files, which helps in better maintainability and follows best practices in web development.
- **Object-Oriented PHP**: The use of OOP in PHP ensures that different parts of the back-end code are reusable and easy to extend.
- **Use of REST API**: The decision to use REST API endpoints allows for a clear separation between the front-end and back-end, making the application scalable and easy to maintain.
- **Bootstrap Framework**: Bootstrap was used to create a responsive user interface, ensuring that the application works well on different screen sizes.

### Target Audience Considerations
The website was designed specifically for the students of my class. Given this target audience, the interface needed to be intuitive and straightforward, enabling users to easily track sessions, view statistics, and manage accounts without requiring extensive technical knowledge. By incorporating familiar UI elements and clear navigation, the design aims to provide a seamless user experience that suits the needs of the student group.

## Challenges and Solutions

- **Data Synchronization**: Keeping the front-end updated with real-time data from the back-end was a challenge. This was solved by using JavaScript's `setInterval()` function to periodically fetch data and update the UI.
- **Database Connection Issues**: There were initial issues with establishing a stable connection to the database, which were solved by implementing a robust error-handling mechanism in `db_connect.php` to log and return errors in a consistent format.
- **Ensuring Consistent UI Updates**: To avoid performance issues, data fetching was modularized into specific functions that only update relevant parts of the UI, minimizing reflows and improving the user experience.

## Technologies Used

- **HTML, CSS, JavaScript**: These were used to build the front-end and provide interactivity.
- **PHP**: Used for the back-end to create API routes and interact with the database.
- **MySQL**: The relational database used to store user sessions, temperature data, and other related information.
- **Bootstrap**: Used for styling and responsive design.
- **Chart.js**: Utilized to create charts for visualizing temperature and pitcher consumption data.
- **Docker**: The deployment environment was containerized using Docker, ensuring consistency across different stages of development, testing, and production.
- **GitLab**: Version control was managed through GitLab, providing effective collaboration and CI/CD pipelines for efficient deployment.

### Contemporary Developments in Web Development
The project incorporates contemporary developments in web development to ensure it is up to date with modern practices:
- **Asynchronous Data Fetching**: The use of JavaScript's Fetch API and periodic updates with `setInterval()` align with modern standards of creating interactive, real-time web applications.
- **Responsive Design**: Bootstrap was used to ensure the website is fully responsive, providing an optimal user experience across devices of various screen sizes, from desktops to smartphones.
- **RESTful API Design**: Following RESTful principles ensures that the front-end and back-end are loosely coupled, making it easier to maintain and extend functionality in the future.

## Website Structure

The website is organized into distinct tabs for easy navigation:

- **Dashboard Tab**: Displays cumulative session data and pitcher activity statistics.
- **Sessions Tab**: Allows users to view detailed session records.
- **Account Management Tab**: Administer user accounts, including adding, editing, or removing users.
- **Leaderboard Tab**: Shows top users based on pitcher consumption and attendance.

Each of these components interacts with the back-end via the API, ensuring that the front-end always displays the latest data from the database.

## API Structure
The back-end API is split into multiple components for better organization and maintainability:

- **Getters**: These are responsible for fetching data from the database, such as user information, session details, temperature, and humidity data. The getters are designed to handle read-only operations, ensuring that the correct data is served to the front-end.
- **Setters**: These handle the creation and updating of data in the database. Examples include adding new users, editing user information, or logging new session entries. The setters follow best practices in data validation to ensure data integrity.
- **Embedded Handlers**: These provide specific functionalities related to hardware integration, such as fetching data from IoT devices. These handlers are responsible for processing the embedded device requests and updating the database accordingly.

## Testing

The application was tested thoroughly to ensure functionality and usability:

- **Unit Testing**: Individual functions in the back-end were tested to ensure they worked correctly in isolation.
- **Integration Testing**: The interaction between the front-end and back-end was tested by simulating API requests from JavaScript.
- **User Testing**: The interface was tested by several users to ensure that it was intuitive and met their expectations.
- **Error Handling**: Extensive error handling was implemented to make sure that all API routes returned appropriate JSON responses even in case of failures.

## Deployment

The BeerTracker HUB was deployed on a cloud server to ensure availability and scalability. Key steps for deployment included:

- **Docker Containers**: Docker was used to containerize the application, ensuring that all dependencies were consistently configured across environments. This made it easier to deploy, scale, and manage the application.
- **Server Setup**: A Linux server with Apache was set up to host the Docker containers.
- **Database Configuration**: MySQL was used for storing data, and its instance was also containerized within Docker for ease of management.
- **Version Control**: GitLab was used for version control, providing CI/CD pipelines that facilitated seamless integration and deployment of new features.