#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>

// Replace with your network credentials
const char* ssid = "Pixel";
const char* password = "22222222";

// Create an instance of the web server running on port 80
WebServer server(80);

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 18, 19);  // SERIAL_8N1, RX pin=16, TX pin=17

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount filesystem");
    return;
  }

  // Define the routes for the web server
  server.on("/", []() {
    server.send(200, "text/html", generateHTML());
  });
  server.on("/toggle", []() {
    handleToggle();
  });
  server.on("/background.png", HTTP_GET, []() {
    File file = SPIFFS.open("/background.jpg", "r");
    if (!file) {
      server.send(404, "text/plain", "File not found");
      return;
    }
    server.streamFile(file, "image/jpg");
    file.close();
  });

  // Start the server
  server.begin();
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
}

String generateHTML() {
  String buttonHTML = "";
  for (int i = 1; i <= 9; i++) {
    buttonHTML += "<button class=\"button\" id=\"button" + String(i) + "\" data-state=\"OFF\" onclick=\"sendToggle(" + String(i) + ")\">Lamp " + String(i) + " OFF</button>\n";
  }

  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>_FOT MEDIA AGM 2025_</title>
    <style>
      body {
        font-family: Arial, sans-serif;
        background-image: url('/background.jpg'); /* Reference to the image served by ESP32 */
        background-size: cover;
        background-position: center;
        color: white;
        margin: 0;
        padding: 20px;
      }
      h2 {
        color: #333;
        margin-bottom: 20px;
        text-align: center;
        
      }
      #buttons {
            display: grid;
            grid-template-columns: repeat(3, 1fr); /* Creates a 3x3 grid */
            gap: 15px;
            justify-items: center;
            max-width: 400px;
            margin: 0 auto; /* Center the grid */
            padding: 20px;
            background: rgba(0, 0, 0, 0.5);
            border-radius: 10px;
        }
      .button {
        padding: 15px 30px;
        font-size: 16px;
        background-color: #4CAF50;
        color: white;
        text-align: center;
        border: none;
        border-radius: 8px;
        cursor: pointer;
        transition: background-color 0.3s, transform 0.2s;
      }
      .button:hover {
        transform: scale(1.05);
      }
      .button[data-state="OFF"] {
        background-color: #f44336;
      }
      .button[data-state="ON"] {
        background-color: #4CAF50;
      }
      @media (max-width: 768px) {
        .button {
          padding: 12px 25px;
          font-size: 14px;
        }
      }
    </style>
  </head>
  <body>
    <h2>_FOT MEDIA AGM 2025_</h2>
    <div id="buttons">
      %BUTTONS%
    </div>
    <script>
      function sendToggle(toggleNumber) {
        var xhr = new XMLHttpRequest();
        var button = document.getElementById("button" + toggleNumber);
        var state = button.getAttribute("data-state") === "ON" ? 0 : 1;
        xhr.open("GET", "/toggle?state=" + state + "&toggle=" + toggleNumber, true);
        xhr.onreadystatechange = function() {
          if (xhr.readyState == 4 && xhr.status == 200) {
            button.setAttribute("data-state", state === 1 ? "ON" : "OFF");
            button.innerText = "Lamp " + toggleNumber + " " + (state === 1 ? "ON" : "OFF");
          }
        };
        xhr.send();
      }
    </script>
  </body>
  </html>
  )rawliteral";

  html.replace("%BUTTONS%", buttonHTML);
  return html;
}

void handleToggle() {
  if (server.hasArg("state") && server.hasArg("toggle")) {
    int toggleNumber = server.arg("toggle").toInt();  // Get the toggle number
    int state = server.arg("state").toInt();
    if (toggleNumber >= 1 && toggleNumber <= 9) {
      String toggleStateStr = state == 1 ? "ON" : "OFF";
      Serial1.println(String(toggleNumber) + ":" + toggleStateStr);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Invalid Toggle Number");
    }
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}
