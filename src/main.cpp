/**
  Wakes up every 30sec to read data from the water sensor, when water is
  detected, it connects to WiFi, then sends an alarm to an MQTT topic
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

#define sensorPower D7 // Power pin
#define sensorPin A0 // Analog Sensor pins
#define durationSleep 30 // seconds
#define NB_TRYWIFI 20 // WiFi connection retries

int sensorData = 0;

// **************
void tick();
void loop();
void setup();
int readSensor();
void connectToHass();
void connectToWiFi();
void publishAlarmToHass(int waterLevel);
// **************

void tick()
{
    // toggle state
    int state = digitalRead(LED_BUILTIN);  // get the current state of GPIO1 pin
    digitalWrite(LED_BUILTIN, !state);     // set pin to the opposite state
}

/**
 * This is a function used to get the reading
 * @return
 */
int readSensor()
{
    // Step 1 : Turn the sensor ON by providing power to D7 pin
	digitalWrite(sensorPower, HIGH);
    // Step 2 : Wait for a just little bit
	delay(10);
    // Step 3 : Perform the reading
	sensorData = analogRead(sensorPin);

    // Step 4 : Turn the sensor OFF
	digitalWrite(sensorPower, LOW);

  return sensorData;
}

/**
 * Establishes WiFi connection
 * @return
 */
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
            ESP.deepSleep(durationSleep * 1000000);
        }
    }
    if (DEBUG == true) {
        Serial.println("Connected to Wi-Fi");
    }
}

/**
 * Establishes connection to Home Assistant MQTT Broker
 * @return
 */
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
 * Publishes notification to MQTT topic
 * @return
 */
void publishAlarmToHass(int waterLevel)
{
    // publish the reading to Hass through MQTT
    client.publish(MQTT_PUBLISH_TOPIC, String(waterLevel).c_str(), true);
    client.loop();
    if (DEBUG == true) {
        Serial.println("Alarm sent to Hass!");
    }
}

void setup()
{
    // only print debug messages to serial if we're in debug mode
    if (DEBUG == true) {
        Serial.print("Waking up ");
    }

    // Step 1: Set D7 pin as an output pin ready to receive power
    pinMode(sensorPower, OUTPUT);
    digitalWrite(sensorPower, LOW);

    ticker.attach(0.5, tick);

	Serial.begin(115200);

    // Step 2: Wake the sensor up & get a reading
    int waterLevel = readSensor();

    if (DEBUG == true) {
        Serial.print("Water Level: ");
        Serial.println(waterLevel);
    }

    // Step 3: If water is detected then
    if (waterLevel > 1) {
        connectToWiFi(); // 1- connect to WiFi
        connectToHass(); // 2- connect to Home Assistant MQTT broker
        publishAlarmToHass(waterLevel); // 3- publish the water level on the MQTT topic
    }

    if (DEBUG == true) {
        Serial.println("Going to deep sleep now");
    }

    // Step 4: Go back to sleep for the next 30 sec
    ESP.deepSleep(durationSleep * 1000000);
}

void loop()
{

}
