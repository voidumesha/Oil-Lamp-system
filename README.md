Connect both arduino and esp32 via Arduino mega RX0 and TXO to ESO D18 and D19 pins.
setup esp board libraries.
install necessary libraires which mentioned in the code.
connect common GND (Arduino and Esp32).
add your wifi name and password to code.
install both codes to board ,mentioned code in that file name.

RUN THE PROJECT

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Image Grid</title>
    <style>
        .image-container {
            display: flex;
            justify-content: center;
            align-items: center;
            gap: 10px;
        }
        .image-container img {
            width: 300px; /* Adjust width as needed */
            height: auto; /* Maintain aspect ratio */
            border-radius: 10px; /* Optional styling */
            object-fit: cover; /* Ensures proper image fitting */
        }
    </style>
</head>
<body>
    <div class="image-container">
        <img src="https://github.com/user-attachments/assets/8541359d-1a36-4183-9bab-319c15fb6015" alt="Image 1">
        <img src="https://github.com/user-attachments/assets/e819b459-bd0f-46b7-bac5-b7c7198839ca" alt="Image 2">
    </div>
</body>
</html>



