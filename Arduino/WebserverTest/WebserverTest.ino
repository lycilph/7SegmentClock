#include <WiFi.h>
#include <WebServer.h>

// From here: 
// https://randomnerdtutorials.com/esp32-web-server-beginners-guide/
// https://www.espboards.dev/esp32/esp32-s3-super-mini/

// Replace with your network credentials
const char* ssid = "VoresWifi";
const char* password = "FDS19&lejk";

// Create a web server object
WebServer server(80);

// Function to handle the root URL and show the current states
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 { background-color: #555555; }</style></head>";
  html += "<body><h1>ESP32 Web Server</h1>";

  // Display GPIO 26 controls
  html += "<p>GPIO 26 - State Off</p>";
  html += "<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>";

  // Display GPIO 27 controls
  html += "<p>GPIO 27 - State Off</p>";
  html += "<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up the web server to handle different routes
  server.on("/", handleRoot);

  // Start the web server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
   // Handle incoming client requests
  server.handleClient();
}
