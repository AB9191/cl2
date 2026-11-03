#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>

const char* ssid        = "nikhil";
const char* password    = "12345678";
const char* firebaseURL = "https://iiot-77fe4-default-rtdb.firebaseio.com";

#define DHT_PIN D4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected");
    delay(5000);
    return;
  }

  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT11 reading failed");
    delay(5000);
    return;
  }

  String json = "{";
  json += "\"temperature\":" + String(temperature, 1) + ",";
  json += "\"humidity\":"    + String(humidity, 1) + ",";
  json += "\"uptime_ms\":"   + String(millis());
  json += "}";

  WiFiClientSecure client;
  client.setInsecure(); // Bypass SSL certificate verification

  HTTPClient https;
  String url = String(firebaseURL) + "/sensorData/readings.json";

  if (https.begin(client, url)) {
    https.addHeader("Content-Type", "application/json");

    int code = https.POST(json);

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Firebase HTTP response: ");
    Serial.println(code);

    if (code == HTTP_CODE_OK) {
      Serial.println("Data stored successfully");
      Serial.println(https.getString());
    } else {
      Serial.println("Failed response payload:");
      Serial.println(https.getString());
    }

    https.end();
  } else {
    Serial.println("Could not connect to Firebase");
  }

  delay(20000);
}