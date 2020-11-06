/**
  Reads data from a water leakage sensor, and sends it
  through pins 2 and 3 to a connected ESP-01 WiFi module
  @author MecaHumArduino
  @version 1.0
*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define DEBUG true

// ESP TX => Uno Pin 2
// ESP RX => Uno Pin 3
SoftwareSerial wifiBoardSerial(2, 3);

// Sensor pins
#define sensorPower 7
#define sensorPin A0

// Value for storing water level
int val = 0;

// **************
int readSensor();
String sendDataToWiFiBoard(String command, const int timeout, boolean debug);
String prepareDataForWiFi(int level);
void setup();
void loop();
// **************

/**
 * Build and return a JSON document from the sensor data
 * @param level
 * @return
 */
String prepareDataForWiFi(int level)
{
  StaticJsonDocument<200> doc;

  doc["water_level"] = String(level);

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  return jsonBuffer;
}

/**
 * Send data through Serial to ESP8266 module
 * @param command
 * @param timeout
 * @param debug
 * @return
 */
String sendDataToWiFiBoard(String command, const int timeout, boolean debug)
{
  String response = "";

  wifiBoardSerial.print(command); // send the read character to the esp8266

  long int time = millis();

  while((time+timeout) > millis()) {
    while(wifiBoardSerial.available()) {
      // The esp has data so display its output to the serial window
      char c = wifiBoardSerial.read(); // read the next character.
      response+=c;
    }
  }

  if (debug) {
    Serial.print(response);
  }

  return response;
}


/**
 * This is a function used to get the reading
 * @param level
 * @return
 */
int readSensor() {
  // Turn the sensor ON
	digitalWrite(sensorPower, HIGH);
	delay(10);
  // Perform the reading
	val = analogRead(sensorPin);

  // Turn the sensor OFF
	digitalWrite(sensorPower, LOW);

  return val;
}

void setup() {
  Serial.begin(9600);
  wifiBoardSerial.begin(9600);

	// Set D7 as an OUTPUT
	pinMode(sensorPower, OUTPUT);

	// Set to LOW so no power flows through the sensor
	digitalWrite(sensorPower, LOW);

	Serial.begin(9600);
}

void loop() {

  if (DEBUG == true) {
    Serial.print("buffer: ");
    if (wifiBoardSerial.available()) {
      String espBuf;
      long int time = millis();

      while((time+1000) > millis()) {
        while (wifiBoardSerial.available()) {
          // The esp has data so display its output to the serial window
          char c = wifiBoardSerial.read(); // read the next character.
          espBuf += c;
        }
      }
      Serial.print(espBuf);
    }
    Serial.println(" endbuffer");
  }

	// get the reading from the function below and print it
	int level = readSensor();

  if (DEBUG == true) {
    Serial.print("Water level: ");
    Serial.println(level);
  }

  String preparedData = prepareDataForWiFi(level);
  sendDataToWiFiBoard(preparedData, 1000, DEBUG);

	delay(5000);
}
