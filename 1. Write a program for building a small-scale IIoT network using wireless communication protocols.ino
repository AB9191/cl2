#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid     = "yash";
const char* password = "123456789";

ESP8266WebServer server(80);
const int LED_PIN = D1;

void handleRoot() {
String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP8266 IIoT Network</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin-top: 80px;
    }
    button {
      padding: 15px 30px;
      font-size: 18px;
      color: white;
      border: none;
      border-radius: 5px;
      margin: 10px;
      cursor: pointer;
    }
    .on  { background: green; }
    .off { background: red; }
  </style>
</head>
<body>
  <h1>ESP8266 IIoT Network</h1>
  <h3>Control LED using Wi-Fi</h3>
  <a href="/on"><button class="on">LED ON</button></a>
  <a href="/off"><button class="off">LED OFF</button></a>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", page);
}

void handleOn() {
  digitalWrite(LED_PIN, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}