# Water Leakage Sensor For Home Assistant

This repo accompanies the "Connected Weather Station" YouTube series. it contains the code, library, diagrams, and more information that I promised in the videos.

[**Video: Building an IoT Weather Station with Arduino, DHT11, and LCD Screen**](https://youtu.be/vL6TKYhjwQc)
This episode shows you how to use the [DHT11](https://amzn.to/2Qs9fcV) temperature and humidity sensors with the [Arduino Uno](https://amzn.to/2RnHhPY) board to build a weather station that displays current temperature and humidity on an [LCD](https://amzn.to/2FzJdT6) screen.


⚡️ COMPONENTS AND SUPPLIES
--------------------------

<img align="right" src="https://images-na.ssl-images-amazon.com/images/I/61sU-gLZncL._AC_SL1001_.jpg" style="max-width:100%;" height="400">

*   [ESP8266 ESP-12F](https://www.amazon.com/IZOKEE-NodeMcu-Internet-Development-Compatible/dp/B076F52NQD/ref=sr_1_5?crid=1DNDEPDE0UUY8&dchild=1&keywords=esp12f&sprefix=esp12%2Caps%2C170&sr=8-5)
*   [Water Level Sensor](https://www.amazon.com/Sensor-Module-Detection-Surface-Arduino%EF%BC%8810pcs%EF%BC%89/dp/B07THDH7Y4/ref=sr_1_7?dchild=1&keywords=6+Pack+Water+Level+Sensor%2C+Droplet+Depth+Detection+Sensor+for+Arduino&sr=8-7)
*   [100pcs Double Sided PCB Board Kit](https://www.amazon.com/Smraza-Soldering-Electronic-Compatible-Prototype/dp/B07NM68FXK/ref=sr_1_2?dchild=1&keywords=smraza+100pcs+pcb+board&sr=8-2)
*   [Breadboard](https://amzn.to/2Ei40tP) - [Jumper Wires](https://amzn.to/2Ehh2ru) - [Male to Male Jumper Wires + Tweezer](https://amzn.to/3jcf9eX)

🖥 APPS
------

*   [VSCode](https://code.visualstudio.com/)
*   [AWS CLI](https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-install.html)
*   [Fritzing](https://fritzing.org/)
*   [PlatformIO](https://platformio.org/)

📦 Libraries
---------
*   [ESP8266WiFi](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)
*   [PubSubClient](https://www.arduinolibraries.info/libraries/pub-sub-client)


Hardware Overview
-----------------
<img align="right" src="https://lastminuteengineers.com/wp-content/uploads/arduino/Water-Level-Sensor-Hardware-Overview.jpg" style="max-width:100%;" height="350">

This [Water Level Sensor Module](https://www.amazon.com/Sensor-Module-Detection-Surface-Arduino%EF%BC%8810pcs%EF%BC%89/dp/B07THDH7Y4/ref=sr_1_7?dchild=1&keywords=6+Pack+Water+Level+Sensor%2C+Droplet+Depth+Detection+Sensor+for+Arduino&sr=8-7) has a series of ten exposed copper traces, five of which are power traces and five are sense traces.
These traces are interlaced so that there is one sense trace between every two power traces.
Usually these traces are not connected but are bridged by water when submerged

The working of the water level sensor is pretty straightforward: The series of exposed parallel conductors, together acts as a variable resistor (just like a potentiometer) whose resistance varies according to the water level.
The change in resistance corresponds to the distance from the top of the sensor to the surface of the water.

The resistance is inversely proportional to the height of the water:
* The more water the sensor is immersed in, results in better conductivity and will result in a lower resistance.
* The less water the sensor is immersed in, results in poor conductivity and will result in a higher resistance.

The sensor produces an output voltage according to the resistance, which by measuring we can determine the water level.

