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

