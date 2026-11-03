#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT11

const char* ssid     = "nikhil";
const char* password = "12345678";

ESP8266WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

const int ledPin = D1;

float temperature = 0.0;
float humidity    = 0.0;
bool ledState     = false;

void readSensor() {
  float newT = dht.readTemperature();
  float newH = dht.readHumidity();

  if (!isnan(newT)) {
    temperature = newT;
  } else {
    Serial.println("Failed to read temperature from DHT11!");
  }

  if (!isnan(newH)) {
    humidity = newH;
  } else {
    Serial.println("Failed to read humidity from DHT11!");
  }
}

void handleRoot() {
  readSensor();

  String page = "<!DOCTYPE html><html><head>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<title>CPS Monitoring</title></head>";
  page += "<body style='font-family: Arial, sans-serif; text-align: center; margin-top: 50px;'>";
  page += "<h1>CPS Monitoring and Control</h1>";
  page += "<h2>Temperature: " + String(temperature, 1) + " &deg;C</h2>";
  page += "<h2>Humidity: " + String(humidity, 1) + " %</h2>";
  page += "<h2>LED Status: " + String(ledState ? "ON" : "OFF") + "</h2>";
  page += "<p><a href='/on'><button style='padding: 12px 24px; font-size: 16px;'>LED ON</button></a></p>";
  page += "<p><a href='/off'><button style='padding: 12px 24px; font-size: 16px;'>LED OFF</button></a></p>";
  page += "<p><a href='/'><button style='padding: 12px 24px; font-size: 16px;'>Refresh Readings</button></a></p>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}

void handleOn() {
  ledState = true;
  digitalWrite(ledPin, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  ledState = false;
  digitalWrite(ledPin, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("Open this IP address: ");
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