
# API Reference

This documentation provides a detailed overview of all available API endpoints in the system. Each endpoint's purpose, method, URL, parameters, and response structure are described to facilitate integration and usage.

---

## API Endpoints

### POST Endpoints

1. **Handle Card Session**
    - **Endpoint**: `/api/card`
    - **Method**: POST
    - **Parameters**:
      - `RFID_Tag` (string): Unique identifier for the RFID card.
    - **Description**: Processes session data associated with a card (RFID).
    - **Response Example**:
      ```json
      {
          "status": "success",
          "message": "Session processed successfully."
      }
      ```

2. **Handle Device Address**
    - **Endpoint**: `/api/address`
    - **Method**: POST
    - **Parameters**:
      - `device_id` (string): Identifier for the device.
      - `address` (string): Address details to be stored.
    - **Description**: Stores or updates the device address information.
    - **Response Example**:
      ```json
      {
          "status": "success",
          "message": "Device address updated successfully."
      }
      ```

3. **Handle Heartbeat**
    - **Endpoint**: `/api/heartbeat`
    - **Method**: POST
    - **Parameters**:
      - `device_id` (string): Identifier of the device sending the heartbeat.
    - **Description**: Logs a heartbeat signal from a device.
    - **Response Example**:
      ```json
      {
          "status": "success",
          "message": "Heartbeat recorded successfully."
      }
      ```

4. **Handle Device Connection**
    - **Endpoint**: `/api/connection`
    - **Method**: POST
    - **Parameters**:
      - `device_id` (string): Identifier of the connected/disconnected device.
      - `status` (string): Status of the device (e.g., "connected", "disconnected").
    - **Description**: Registers the connection status of a device.
    - **Response Example**:
      ```json
      {
          "status": "success",
          "message": "Device connection status recorded."
      }
      ```

5. **Add Session Data**
    - **Endpoint**: `/api/session`
    - **Method**: POST
    - **Parameters**:
      - `UserID` (int): User ID for session entry.
      - `CheckInTime` (datetime): Time of check-in.
      - `CheckOutTime` (datetime): Time of check-out.
    - **Description**: Records session data including check-in and check-out times for users.
    - **Response Example**:
      ```json
      {
          "status": "success",
          "message": "Session data recorded successfully."
      }
      ```

6. **Handle Temperature Data**
    - **Endpoint**: `/api/temperature`
    - **Method**: POST
    - **Parameters**:
      - `temperature` (float): Temperature reading from the device.
      - `humidity` (float): Humidity reading from the device.
    - **Description**: Stores temperature readings from devices.
    - **Response Example**:
      ```json
      {
          "status": "success",
          "message": "Temperature data recorded."
      }
      ```


7. **Add New User**
    - **Endpoint**: `/api/add_user`
    - **Method**: POST
    - **Parameters**:
      - `name` (string): Name of the new user.
      - `role` (string): Role of the user (e.g., "admin", "user").
      - `rfid_tag` (string): RFID tag associated with the user.
    - **Description**: Adds a new user with associated RFID tag information.
    - **Response Example**:
      ```json
      {
          "success": true,
          "message": "User added successfully."
      }
      ```

8. **Edit User**
    - **Endpoint**: `/api/edit_user`
    - **Method**: POST
    - **Parameters**:
      - `userID` (int): ID of the user to edit.
      - `name` (string): Updated name of the user.
      - `role` (string): Updated role of the user.
      - `rfid_tag` (string): Updated RFID tag.
    - **Description**: Edits user details including name, role, and RFID tag.
    - **Response Example**:
      ```json
      {
          "success": true,
          "message": "User details updated."
      }
      ```

9. **Delete User**
    - **Endpoint**: `/api/delete_user`
    - **Method**: POST
    - **Parameters**:
      - `userID` (int): ID of the user to delete.
    - **Description**: Deletes a user from the database.
    - **Response Example**:
      ```json
      {
          "success": true,
          "message": "User deleted."
      }
      ```

---

### GET Endpoints

1. **Get Connection Status**
    - **Endpoint**: `/api/connection`
    - **Method**: GET
    - **Description**: Retrieves connection status of all devices.
    - **Response Example**:
      ```json
      {
          "success": true,
          "connections": [
              { "deviceId": "ESP32-001", "status": "connected" },
              { "deviceId": "ESP32-002", "status": "disconnected" }
          ]
      }
      ```

2. **Get Heartbeat Status**
    - **Endpoint**: `/api/heartbeat`
    - **Method**: GET
    - **Description**: Fetches latest heartbeat data from devices.
    - **Response Example**:
      ```json
      {
          "success": true,
          "heartbeat": "Device heartbeat status retrieved."
      }
      ```

3. **Get Device Address**
    - **Endpoint**: `/api/address`
    - **Method**: GET
    - **Description**: Retrieves the address information of a device.
    - **Response Example**:
      ```json
      {
          "success": true,
          "address": "Device address retrieved."
      }
      ```

4. **Get Latest Check-in**
    - **Endpoint**: `/api/latest_checkin`
    - **Method**: GET
    - **Description**: Returns the most recent check-in data.
    - **Response Example**:
      ```json
      {
          "success": true,
          "latest_checkin": { "UserID": "1", "CheckInTime": "2024-11-06 08:00" }
      }
      ```

5. **Get Latest Connection**
    - **Endpoint**: `/api/latest_connection`
    - **Method**: GET
    - **Description**: Retrieves the latest connection status updates.
    - **Response Example**:
      ```json
      {
          "success": true,
          "latest_connection": { "deviceId": "ESP32-001", "status": "connected" }
      }
      ```

6. **Get Latest Temperature**
    - **Endpoint**: `/api/latest_temperature`
    - **Method**: GET
    - **Description**: Fetches the latest temperature and humidity reading.
    - **Response Example**:
      ```json
      {
          "success": true,
          "temperature": "23.5",
          "humidity": "60"
      }
      ```

7. **Get Temperature and Humidity Logs**
    - **Endpoint**: `/api/temperature_humidity`
    - **Method**: GET
    - **Description**: Retrieves the last 60 temperature and humidity records.
    - **Response Example**:
      ```json
      {
          "success": true,
          "data": [
              { "timestamp": "2024-11-05 14:00", "temperature": "23.5", "humidity": "60" },
              { "timestamp": "2024-11-05 13:00", "temperature": "22.8", "humidity": "58" }
          ]
      }
      ```

8. **Get Leaderboard**
    - **Endpoint**: `/api/leaderboard`
    - **Method**: GET
    - **Description**: Provides a leaderboard based on user session data.
    - **Response Example**:
      ```json
      {
          "success": true,
          "leaderboard": [
              { "UserID": "1", "name": "Alice", "totalPitchers": "5" },
              { "UserID": "2", "name": "Bob", "totalPitchers": "4" }
          ]
      }
      ```

9. **Get All Sessions**
    - **Endpoint**: `/api/session`
    - **Method**: GET
    - **Description**: Lists all session entries with details for each user.
    - **Response Example**:
      ```json
      {
          "success": true,
          "sessions": [
              { "SessionID": "1", "UserID": "1", "CheckInTime": "08:00", "CheckOutTime": "17:00" }
          ]
      }
      ```

10. **Get Time Registration Data**
    - **Endpoint**: `/api/time_registration`
    - **Method**: GET
    - **Description**: Retrieves time registration data for users.
    - **Response Example**:
      ```json
      {
          "success": true,
          "registrations": [
              { "UserID": "1", "name": "Alice", "role": "admin" }
          ]
      }
      ```

11. **Get Users**
    - **Endpoint**: `/api/users`
    - **Method**: GET
    - **Description**: Lists all users with role and RFID data.
    - **Response Example**:
      ```json
      {
          "success": true,
          "users": [
              { "UserID": "1", "name": "Alice", "role": "admin", "RFID_Tag": "123456" }
          ]
      }
      ```

12. **Get Cumulative Statistics**
    - **Endpoint**: `/api/cumulative_statistics`
    - **Method**: GET
    - **Description**: Provides cumulative statistics on sessions and users.
    - **Response Example**:
      ```json
      {
          "success": true,
          "cumulative_data": { "total_sessions": "10", "total_hours": "50" }
      }
      ```

13. **Get Pitcher Statistics**
    - **Endpoint**: `/api/pitcher_statistics`
    - **Method**: GET
    - **Description**: Provides statistics about pitcher consumption, including total pitchers, money spent, and top consumers.
    - **Response Example**:
      ```json
      {
          "success": true,
          "pitcher_data": {
              "total_pitchers": "100",
              "total_money_spent": "1400",
              "pitchers_per_session": "2.5",
              "most_pitchers_session_date": "2024-11-01",
              "most_pitchers_in_session": "5"
          }
      }
      ```

14. **Get Pitcher Consumption per User**
    - **Endpoint**: `/api/pitcher_consumption`
    - **Method**: GET
    - **Description**: Retrieves consumption data showing pitchers consumed per user.
    - **Response Example**:
      ```json
      {
          "success": true,
          "pitcher_consumption": [
              { "name": "Alice", "total_pitchers": "10", "total_sessions": "5" },
              { "name": "Bob", "total_pitchers": "8", "total_sessions": "4" }
          ]
      }
      ```

---
