#include <Arduino.h>
#include "secrets.h"
#include <time.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <WiFiClientSecure.h>

const int read = A0;
int value;

uint8_t DST = 0;
WiFiClientSecure net;

unsigned long lastMillis = 0;
time_t now;
time_t nowish = 1510592825;

void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");

  configTime(TIME_ZONE * 3600, DST * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);

  while (now < nowish) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }

  Serial.println(" done!");

  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);

  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void connectToWiFi(String initString)
{
  Serial.print(initString);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println(" ok!");
}

void setup()
{
	// Begin serial communication
	Serial.begin(9600);
  	Serial.println("starting setup");

	connectToWiFi(String("Attempting to connect to SSID: ") + String(ssid));

  	NTPConnect();
}

void loop()
{
	value = analogRead(read); // Read data from analog pin and store it to value variable

	if (value <= 480) {
		Serial.println("Water level: 0mm - Empty!");
	}
	else if (value > 480 && value <= 530) {
		Serial.println("Water level: 0mm to 5mm");
	}
	else if (value > 530 && value <= 615) {
		Serial.println("Water level: 5mm to 10mm");
	}
	else if (value > 615 && value <= 660) {
		Serial.println("Water level: 10mm to 15mm");
	}
	else if (value > 660 && value <= 680) {
		Serial.println("Water level: 15mm to 20mm");
	}
	else if (value > 680 && value <= 690) {
		Serial.println("Water level: 20mm to 25mm");
	}
	else if (value > 690 && value <= 700) {
		Serial.println("Water level: 25mm to 30mm");
	}
	else if (value > 700 && value <= 705) {
		Serial.println("Water level: 30mm to 35mm");
	}
	else if (value > 705) {
		Serial.println("Water level: 35mm to 40mm");
	}

	delay(1000);
}
