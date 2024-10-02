# API Reference

## Overview
This API handles temperature, card, and session data for the clock-in/out system. Depending on the `type` parameter, requests are delegated to specific handlers for processing.

### Base URL
`/api.php`

---

## Endpoints

### Submit Temperature Data
**URL:** `/api.php`  
**Method:** `POST`  
**Handler:** `temperature_handler.php`

#### Parameters
| Name        | Type   | Required | Description                        |
|-------------|--------|----------|------------------------------------|
| `type`      | string | Yes      | Must be set to `'temperature'`.    |
| `temperature` | float | Yes      | Temperature value to record.       |
| `humidity`  | float  | Yes      | Humidity value to record.          |

#### Example Request (JSON)
```json
{
  "type": "temperature",
  "temperature": 22.5,
  "humidity": 60.1
}
```

#### Response
- **Success:**
    ```json
    {
      "status": "success"
    }
    ```
- **Error (invalid data):**
    ```json
    {
      "status": "error",
      "message": "Invalid temperature or humidity data"
    }
    ```

---

### Submit Card Data
**URL:** `/api.php`  
**Method:** `POST`  
**Handler:** `card_handler.php`

#### Parameters
| Name   | Type   | Required | Description                      |
|--------|--------|----------|----------------------------------|
| `type` | string | Yes      | Must be set to `'card'`.         |
| `uid`  | string | Yes      | Unique identifier for the card.  |

#### Example Request (JSON)
```json
{
  "type": "card",
  "uid": "04A224FF"
}
```

#### Response
- **Success (User found):**
    ```json
    {
      "status": "success",
      "name": "John Doe"
    }
    ```
- **Error (Card not found):**
    ```json
    {
      "status": "error",
      "message": "Card not found"
    }
    ```
- **Error (UID not provided):**
    ```json
    {
      "status": "error",
      "message": "UID not provided"
    }
    ```

---

### Handle Session Check-in/Check-out
**URL:** `/api.php`  
**Method:** `POST`  
**Handler:** `session_handler.php`

#### Parameters
| Name   | Type   | Required | Description                      |
|--------|--------|----------|----------------------------------|
| `type` | string | Yes      | Must be set to `'card'`.         |
| `uid`  | string | Yes      | Unique identifier for the card.  |

#### Example Request (Check-in/Check-out)
```json
{
  "type": "card",
  "uid": "04A224FF"
}
```

#### Response
- **Success (Check-in):**
    ```json
    {
      "status": "success",
      "action": "checked_in",
      "message": "User checked in",
      "name": "John Doe"
    }
    ```
- **Success (Check-out):**
    ```json
    {
      "status": "success",
      "action": "checked_out",
      "message": "User checked out",
      "name": "John Doe"
    }
    ```
- **Error (Already checked out):**
    ```json
    {
      "status": "error",
      "message": "User already checked out"
    }
    ```
- **Error (Card not found):**
    ```json
    {
      "status": "error",
      "message": "Card not found"
    }
    ```