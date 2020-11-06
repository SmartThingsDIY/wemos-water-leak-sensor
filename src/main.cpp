/**
  Reads data from a water leakage sensor, and sends it
  through MQTT to Home Assistant
  @author MecaHumArduino
  @version 1.0
*/

#include <Arduino.h>
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DEBUG true // switch to "false" for production
WiFiClient espClient;
PubSubClient client(espClient);

// Sensor pins
#define sensorPower D7
#define sensorPin A0

int sensorData = 0;

// **************
void connectToHass();
void connectToWiFi();
int readSensor();
void setup();
void loop();
// **************

void connectToWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    if (DEBUG == true) {
        Serial.println("Connecting to Wi-Fi");
    }

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    if (DEBUG == true) {
        Serial.println("Connected to Wi-Fi");
    }
}

void connectToHass()
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/**
 * This is a function used to get the reading
 * @param level
 * @return
 */
int readSensor()
{
    // Turn the sensor ON
	digitalWrite(sensorPower, HIGH);
	delay(10);
    // Perform the reading
	sensorData = analogRead(sensorPin);

    // Turn the sensor OFF
	digitalWrite(sensorPower, LOW);

  return sensorData;
}

void setup()
{
	Serial.begin(115200);
    connectToWiFi();
    client.setServer(MQTT_SERVER, 1883);

    // Set D7 as an OUTPUT
    pinMode(sensorPower, OUTPUT);

    // Set to LOW so no power flows through the sensor
    digitalWrite(sensorPower, LOW);
}

void loop()
{
    if (!client.connected()) {
        connectToHass();
    }

    // get the reading from the function below and print it
	int waterLevel = readSensor();

    if (DEBUG == true) {
        Serial.print("Water Level: ");
        Serial.println(waterLevel);
    }

    if (waterLevel > 0) {
        client.loop();
        client.publish(MQTT_PUBLISH_TOPIC, String(waterLevel).c_str(), true);
    }

    delay(10000);
}
