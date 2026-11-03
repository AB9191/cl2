#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define BUZZER_PIN 8
const float TEMPERATURE_THRESHOLD = 23.0;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(1000);

  float temperatureC = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperatureC) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" % | Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  if (temperatureC > TEMPERATURE_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("ALERT: Temperature is too high!");
  } 
  else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}