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
#include <Ticker.h>

#define DEBUG true // switch to "false" for production
Ticker ticker;
WiFiClient espClient;
PubSubClient client(espClient);

#define sensorPower D7 // Sensor pins
#define sensorPin A0
#define NB_TRYWIFI 10 // amount of WiFi connection retries

int sensorData = 0;

// **************
void tick();
void loop();
void setup();
int readSensor();
void connectToHass();
void connectToWiFi();
// **************

void tick()
{
    // toggle state
    int state = digitalRead(LED_BUILTIN);  // get the current state of GPIO1 pin
    digitalWrite(LED_BUILTIN, !state);     // set pin to the opposite state
}

void connectToWiFi()
{
    int _try = 0;
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    if (DEBUG == true) {
        Serial.println("Connecting to Wi-Fi");
    }

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        _try++;
        if ( _try >= NB_TRYWIFI ) {
            if (DEBUG == true) {
                Serial.println("Impossible to connect WiFi, going to deep sleep");
            }
            ESP.deepSleep(0);
        }
    }
    if (DEBUG == true) {
        Serial.println("Connected to Wi-Fi");
    }
}

void connectToHass()
{
    client.setServer(MQTT_SERVER, 1883);

    // Loop until we're reconnected
    while (!client.connected()) {
        if (DEBUG == true) {
            Serial.print("Attempting MQTT connection...");
        }
        // Attempt to connect
        // If you do not want to use a username and password, change next line to
        // if (client.connect("ESP8266Client")) {
        if (client.connect("ESP8266Client", MQTT_USER, MQTT_PASSWORD)) {
            if (DEBUG == true) {
                Serial.println("connected");
            }
        } else {
            if (DEBUG == true) {
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.println(" try again in 5 seconds");
            }
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
    int start = millis();

    // Set D7 as an OUTPUT
    pinMode(sensorPower, OUTPUT);

    // Blink builtin LED during WiFi connection
    pinMode(LED_BUILTIN, OUTPUT);

    ticker.attach(0.5, tick);

	Serial.begin(115200);

    // restart reason
    if (DEBUG == true) {
        Serial.println(""); Serial.print("Reset Reason :"); Serial.println(ESP.getResetReason());
    }

    connectToWiFi();
    connectToHass();

    // get a reading
    int waterLevel = readSensor();
    if (DEBUG == true) {
        Serial.print("Water Level: ");
        Serial.println(waterLevel);
    }

    // publish the reading to Hass through MQTT
    if (waterLevel > 0) {
        client.publish(MQTT_PUBLISH_TOPIC, String(waterLevel).c_str(), true);
    }

    // Waking time
    if (DEBUG == true) {
        Serial.println("Waking time: "); Serial.print(millis()-start); Serial.println("ms");
    }

    ticker.detach();
    digitalWrite(LED_BUILTIN, LOW);

    // Set to LOW so no power flows through the sensor
    digitalWrite(sensorPower, LOW);

    if (DEBUG == true) {
        Serial.println("Go to deep sleep");
    }

    ESP.deepSleep(0);
}

void loop()
{

}
