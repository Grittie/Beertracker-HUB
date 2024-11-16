<?php
class SettersHandler
{
    private $dbConnection;

    public function __construct($dbConnection)
    {
        $this->dbConnection = $dbConnection;
    }

    // POST request to add a new user
    public function handleAddUser()
    {
        $response = ['success' => false, 'message' => ''];

        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            $name = $_POST['name'] ?? '';
            $role = $_POST['role'] ?? '';
            $rfid_tag = $_POST['rfid_tag'] ?? '';

            // Validate inputs here (e.g., check for empty fields)
            if (empty($name) || empty($role) || empty($rfid_tag)) {
                $response['message'] = 'All fields are required.';
            } else {
                // Prepare and execute the SQL statement for the users table
                $userStatement = $this->dbConnection->prepare("INSERT INTO Users (name, role) VALUES (?, ?)");
                if ($userStatement) {
                    $userStatement->bind_param("ss", $name, $role);
                    if ($userStatement->execute()) {
                        // Get the last inserted UserID
                        $userId = $userStatement->insert_id;

                        // Now insert the RFID tag into the cards table
                        $cardStatement = $this->dbConnection->prepare("INSERT INTO Cards (UserID, RFID_Tag) VALUES (?, ?)");
                        if ($cardStatement) {
                            $cardStatement->bind_param("is", $userId, $rfid_tag);
                            if ($cardStatement->execute()) {
                                $response['success'] = true;
                            } else {
                                $response['message'] = 'Failed to add RFID tag.';
                            }
                            $cardStatement->close();
                        } else {
                            $response['message'] = 'Database statement preparation for RFID tag failed.';
                        }
                    } else {
                        $response['message'] = 'Failed to add user.';
                    }
                    $userStatement->close();
                } else {
                    $response['message'] = 'Database statement preparation for user failed.';
                }
            }
        }

        echo json_encode($response);
    }

    // POST request to edit an existing user
    public function handleEditUser()
    {
        // Get the UserID and RFID_Tag from the request
        $userID = $_POST['userID'] ?? null;
        $name = $_POST['name'] ?? null;
        $role = $_POST['role'] ?? null;
        $rfidTag = $_POST['rfid_tag'] ?? null;

        if ($userID && $name && $role && $rfidTag) {
            // Prepare the statement to update the user and RFID tag
            $statement = $this->dbConnection->prepare("
                UPDATE Users u
                LEFT JOIN Cards c ON u.UserID = c.UserID
                SET u.name = ?, u.role = ?, c.RFID_Tag = ?
                WHERE u.UserID = ?
            ");
            $statement->bind_param('sssi', $name, $role, $rfidTag, $userID);

            if ($statement->execute()) {
                echo json_encode(['success' => true]);
            } else {
                echo json_encode(['success' => false, 'message' => 'Failed to update user']);
            }

            $statement->close();
        } else {
            echo json_encode(['success' => false, 'message' => 'User ID, name, role, and RFID tag are required']);
        }
    }

    // POST request to delete a user
    public function handleDeleteUser()
    {
        // Get the UserID from the request
        $userID = $_POST['userID'] ?? null;

        if ($userID) {
            // Prepare the statement to delete the user
            $statement = $this->dbConnection->prepare("DELETE FROM Users WHERE UserID = ?");
            $statement->bind_param('i', $userID);

            if ($statement->execute()) {
                echo json_encode(['success' => true]);
            } else {
                echo json_encode(['success' => false, 'message' => 'Failed to delete user']);
            }

            $statement->close();
        } else {
            echo json_encode(['success' => false, 'message' => 'User ID is required']);
        }
    }
}