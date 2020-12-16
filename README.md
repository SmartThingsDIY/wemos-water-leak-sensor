# DIY Water Leakage Sensor For Home Assistant...
Or any MQTT based system

About
=====
This article accompanies the "DIY Water Leakage Sensor" YouTube series.

It contains the code, libraries, diagrams, 3D print files and more information that I promised in the videos:

<iframe width="560" height="315" src="https://www.youtube.com/embed/6FVJRkGDwwI" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

BUY VS BUILD
========
You can always go ahead and buy a ready-to-use a solution like this [CONNECTIFY Smart Water Leakage Sensor](https://www.amazon.ca/gp/product/B07TDCTQDN/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) but these devices usually would only work with their respective apps, and some of them would require you to pay extra in order to receive push notifications. Now, this article and its video provide a step-by-step guide and links to everything you need to build the same device for **3x cheaper** than the cheapest option available.

<img align="right" src="https://images-na.ssl-images-amazon.com/images/I/51BVI1yCFPL._AC_SL1001_.jpg" style="max-width:100%;" height="400">

In addition to detecting water leakage and sending notifications. This device:
* Goes to deep sleep and only wakes up every 30sec (configurable) to save energy
* When water is detected, only then, the connection to WiFi is made, again, to save energy
* Has a removable LiPo battery for easy of charge
* Clever wiring of the water sensor to extend its life expectancy (more details in the wiring section)
* Sends notifications (and any type of data) to an MQTT topic, enlarging the possibilities of what you can do with the data

In my case, I will be sending the notifications to my [Home Assistant](https://www.home-assistant.io/) setup in order to trigger a pre-defined automation that:
* Pushes actionable notifications to our phone devices through [Home Assistant companion apps](https://companion.home-assistant.io/)
* Converts notification alarm to a text (Alexa TTS capabilities) and play it through the Amazon Echo devices around the house
* And just in case we managed to miss these, it turns the kitchen LED light to RED informing us of an emergency
* Repeats every 5min until someone explicitly acknowledged the issue.

The beauty of this is I can decide to trigger any type of notifications in the future such as playing an alarm sound or sending SMS notifications...

So without further ado, let's start with a list of all hardware components, Apps and Libraries I've used in this project:

⚡️ COMPONENTS AND SUPPLIES
=====

<img align="right" src="https://images-na.ssl-images-amazon.com/images/I/418BpihzO8L.jpg" style="max-width:100%;" height="400">

*   [WEMOS D1 R2 (ESP8266 ESP-12F)](https://www.amazon.com/IZOKEE-NodeMcu-Internet-Development-Compatible/dp/B076F52NQD/)
*   [Water Level Sensor](https://www.amazon.com/Sensor-Module-Detection-Surface-Arduino%EF%BC%8810pcs%EF%BC%89/dp/B07THDH7Y4/)
*   [5 LiPo Batteries And Charger](https://www.amazon.ca/gp/product/B0795F139D)
*   [Spade 2P Cable Lead Plug](https://www.amazon.ca/gp/product/B07YQY9V6F/)
*   [Solder Kit](https://www.amazon.ca/-/fr/gp/product/B01N46T138/)
*   [Helping Hands for soldering](https://www.amazon.ca/gp/product/B002PIA6Z4)
*   [Screwdriver Set](https://www.amazon.ca/Precision-Screwdriver-Lifegoo-Eyeglasses-Electronic/dp/B07XCWT3W8/)
*   [Breadboard](https://amzn.to/2Ei40tP) - [Jumper Wires](https://amzn.to/2Ehh2ru) - [Male to Male Jumper Wires + Tweezer](https://amzn.to/3jcf9eX)

🖥 APPS
=====

*   [VSCode](https://code.visualstudio.com/)
*   [AWS CLI](https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-install.html)
*   [Fritzing](https://fritzing.org/)
*   [PlatformIO](https://platformio.org/)

📦 Libraries
=====
*   [ESP8266WiFi](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)
*   [PubSubClient](https://www.arduinolibraries.info/libraries/pub-sub-client)
*   [Ticker](https://www.arduino.cc/reference/en/libraries/ticker/)


Hardware Overview: Water Sensor
===============
<img align="right" src="https://github.com/MecaHumArduino/wemos-water-leak-sensor/blob/main/doc/Water-Level-Sensor-Working.gif?raw=true" style="max-width:100%;" height="350">

This [Water Level Sensor Module](https://www.amazon.com/Sensor-Module-Detection-Surface-Arduino%EF%BC%8810pcs%EF%BC%89/dp/B07THDH7Y4/ref=sr_1_7?dchild=1&keywords=6+Pack+Water+Level+Sensor%2C+Droplet+Depth+Detection+Sensor+for+Arduino&sr=8-7) has a series of ten exposed copper traces, five of which are power traces and five are sense traces.
These traces are interlaced so that there is one sense trace between every two power traces.
Usually these traces are not connected but are bridged by water when submerged

The working of the water level sensor is pretty straightforward: The series of exposed parallel conductors, together acts as a variable resistor (just like a potentiometer) whose resistance varies according to the water level.
The change in resistance corresponds to the distance from the top of the sensor to the surface of the water.

The resistance is inversely proportional to the height of the water:
* The more water the sensor is immersed in, results in better conductivity and will result in a lower resistance.
* The less water the sensor is immersed in, results in poor conductivity and will result in a higher resistance.

The sensor produces an output voltage according to the resistance, which by measuring we can determine the water level.

Hardware Overview: WEMOS R1 D2
===============
<img align="right" src="https://images-na.ssl-images-amazon.com/images/I/61sU-gLZncL._AC_SL1001_.jpg" style="max-width:100%;" height="400">

I've decided to use the [WEMOS R1 D2](https://www.amazon.com/Sensor-Module-Detection-Surface-Arduino%EF%BC%8810pcs%EF%BC%89/dp/B07THDH7Y4/ref=sr_1_7?dchild=1&keywords=6+Pack+Water+Level+Sensor%2C+Droplet+Depth+Detection+Sensor+for+Arduino&sr=8-7) microchip for this project because it has a small size that saves space and can easily fit in a printable 3D case, but also because of a few reasons:
* It has an Analog pin
* It has WiFi capabilities
* It supports OTA online: Updating code Over-The-Air
* 4MB of memory which more than enough for our needs
* And because it's compatible with Arduino IDE, making the programming easy

THE WIRING
==========
The wiring is pretty straightforward. Please follow the diagram bellow. Also, here are some notes that might help you figure it out:

<img align="center" src="https://github.com/MecaHumArduino/wemos-water-leak-sensor/blob/main/doc/the_wiring.png?raw=true" style="max-width:100%;" height="411">


* **Blue** wire connecting RST and D0: Required to allow the WEMOS board to go to sleep.
* **Red** wire connecting water sensor to D7: You might be wondering why the water sensor is not directly connected to the 3v3 pin. Well, one commonly known issue with these sensors is their short lifespan when exposed to a moist environment. Having power applied to the probe constantly speeds the rate of corrosion significantly.
To overcome this, I recommend that you do not power the sensor constantly, but power it only when you take the readings.
An easy way to accomplish this is to connect the VCC pin to a digital pin of an Arduino and set it to HIGH or LOW as per requirement. So, we'll connect the VCC pin to the digital pin #7 of an Arduino.

Wiring source files are included under [wiring folder](https://github.com/MecaHumArduino/wemos-water-leak-sensor/tree/main/doc/wiring)

3D PRINTED CASE
==========
No one likes wires hanging around, and so I included the source and STL files for the 3D case I prepared for this project. All of it is under [3d folder](https://github.com/MecaHumArduino/wemos-water-leak-sensor/tree/main/doc/3d) folder and you can do whatever the heck you want with it.

THE CODE
========

The code within `main.cpp` file is well documented, but I'll try to explain the concepts and ideas behind the code in this section. But first of all, copy the file `secrets_copy.h` to `secrets.h` and edit its content with your details: WiFi credentials, Home Assistant details...

The sketch begins with the creation of a few objects we'll need along the way: `Ticker` that calls repeating actions and which we will use to put the board to sleep and then wake up. `WiFiClient` that we use to connect to Wifi and `PubSubClient` that we use to send data through MQTT

```cpp
Ticker ticker;
WiFiClient espClient;
PubSubClient client(espClient);
```

Then we declare a few variables like the Arduino pins for the sensor reading and power, as long as the sleep duration and number of tries we aim to do while connecting to WiFi because we want to avoid draining the battery trying to connect to WiFi indefinitely.

```cpp
#define sensorPower D7 // Power pin
#define sensorPin A0 // Analog Sensor pins
#define durationSleep 30 // seconds
#define NB_TRYWIFI 20 // WiFi connection retries
```

As you might have noticed, there is no `loop()` function in this sketch, only a `setup()` function, and that's because instead of executing some commands in a loop fashion like most Arduino projects, this board is programmed to go to sleep until its wake up moment arrives, in which case, it will executing everything until `setup()` function before going back to sleep again. Here is how it's programmed:

```cpp
void setup()
{
    // only print debug messages to serial if we're in debug mode
    if (DEBUG == true) {
        Serial.print("Waking up ");
    }

    // Step 1: Set D7 pin as an output pin ready to receive power
    pinMode(sensorPower, OUTPUT); // Set D7 as an OUTPUT
    digitalWrite(sensorPower, LOW);

    ...

    // Step 2: Wake the sensor up & get a reading
    int waterLevel = readSensor();

    ...

    // Step 3: If water is detected then
    if (waterLevel > 1) {
        connectToWiFi(); // 1- connect to WiFi
        connectToHass(); // 2- connect to Home Assistant MQTT broker
        publishAlarmToHass(waterLevel); // 3- publish the water level on the MQTT topic
    }

    // Step 4: Go back to sleep for the next 30 sec
    ESP.deepSleep(durationSleep * 1000000);

}
```

The function that reads the water level is straightforward and well documented as well:

```cpp
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
```

Finally
======
All contribution to this project is appreciated
