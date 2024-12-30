# Authentication

## Configuration Header File
This project uses a `config.h` file to store sensitive credentials such as API usernames and passwords. This file is required for the project to build and run successfully.

### Step 1: Copy the Example File
To set up the authentication file:

1. Navigate to the `src` folder of the PlatformIO project.
2. Locate the file named `config.h.example`.
3. Copy and rename it to `config.h` in the same directory.

### Step 2: Edit the `config.h` File
Open the newly created `config.h` file and fill in the required credentials:

```cpp
#ifndef CONFIG_H
#define CONFIG_H

// API username for authenticating with the server
#define API_USERNAME "your_actual_username"

// API password for authenticating with the server
#define API_PASSWORD "your_actual_password"

// API server address
#define API_PASSWORD "your_actual_address"

#endif
```