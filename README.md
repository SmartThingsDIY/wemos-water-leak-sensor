# Connected Weather Station with Arduino to measure Temperature, Humidity, Heat Index and UV light

PROJECT ARCHITECTURE
--------------------------

<img align="center" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/architecture-diagram.png?raw=true" style="max-width:100%;" height="350">


This repo accompanies the "Connected Weather Station" YouTube series. it contains the code, library, diagrams, and more information that I promised in the videos.

[**Video 1: Building an IoT Weather Station with Arduino, DHT11, and LCD Screen**](https://youtu.be/vL6TKYhjwQc)
This episode shows you how to use the [DHT11](https://amzn.to/2Qs9fcV) temperature and humidity sensors with the [Arduino Uno](https://amzn.to/2RnHhPY) board to build a weather station that displays current temperature and humidity on an [LCD](https://amzn.to/2FzJdT6) screen.

[**Video 2: Connecting the IoT Weather Station to AWS IoT using ESP8266**](https://youtu.be/aR3pAWPbU1Q)
This episode builds on the previous one and continues forward by pushing the temperature & humidity data received through the [DHT11](https://amzn.to/2Qs9fcV) sensor, onto an AWS IoT account using an [ESP8266](https://amzn.to/3hqqWom) WiFi module.

[**Video 3: Adding a UV sensor to the weather station**](https://youtu.be/YlprYKokXfs)
This episode builds on the previous two and shows how to add a [UV Light](https://amzn.to/3jP4JCA) sensor to the weather station.


⚡️ COMPONENTS AND SUPPLIES
--------------------------

<img align="right" src="https://store-cdn.arduino.cc/usa/catalog/product/cache/1/image/1000x750/f8876a31b63532bbba4e781c30024a0a/a/0/a000066_iso_1_3.jpg" style="max-width:100%;" height="300">

*   [Arduino Uno](https://amzn.to/2RnHhPY)
*   [LCD Screen](https://amzn.to/2FzJdT62Ehh2ru)
*   [Breadboard](https://amzn.to/2Ei40tP) - [Jumper Wires](https://amzn.to/2Ehh2ru) - [Male to Male Jumper Wires + Tweezer](https://amzn.to/3jcf9eX)
*   [DHT11 Temp & Humidity Sensor](https://amzn.to/2Qs9fcV)
*   [UV Sensor GUVA-S12SD](https://amzn.to/3jP4JCA)
*   [10k Ohm resistor](https://amzn.to/2Qo7vkW)
*   [Hardware / Storage Cabinet Drawer](https://amzn.to/36ehDpB)
*   [ESP8266 ESP-01 WiFi Module](https://amzn.to/30fUWNS)
*   [ESP8266 ESP-01 programmable USB](https://amzn.to/345egi6)
*   [ESP8266 ESP-01 Breadboard Adapter](https://amzn.to/3kSFVcP)

🖥 APPS
------

*   [VSCode](https://code.visualstudio.com/)
*   [AWS CLI](https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-install.html)
*   [Fritzing](https://fritzing.org/)
*   [PlatformIO](https://platformio.org/)

📦 Libraries
---------
*   [DHT](https://github.com/adafruit/DHT-sensor-library)
*   [LiquidCrystal](https://www.arduino.cc/en/Reference/LiquidCrystal)
*   [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
*   [SoftwareSerial](https://www.arduino.cc/en/Reference.SoftwareSerial)


What is humidity?
-----------------
Humidity is the water vapour around you mixed with air. It is measured in %. So, if the humidity is 60%, then 60% of the air around you is water vapour. If it is 100%, then it means either the sensor is not correct, the sensor is broken/damaged, the Arduino crashed, the Arduino can't receive any signal, there's an error in the code or you're underwater(1). If it's 0%, it means all the reasons above except the last one, you're in space or you're in the middle of a desert(2).

(1) Correction: it means the air cannot hold any more water.
(2) The air in a desert does contain some water but it is a very little amount compared to a normal place. The Sahara Desert has a mean humidity of 25%.

<img align="right" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/dht11.jpeg?raw=true" style="max-width:100%;" height="250">

How to measure Temperature & Humidity
-------------------------------------

To the right, is a [DHT11](https://amzn.to/2Qs9fcV) sensor, it is a temperature and humidity combined sensor with calibrated digital signal output. It's is a reliable sensor for determining temperature and humidity. There is a newer and more accurate module on the market called DHT22 but we will stick with the DHT11 for this demo<br><br>

<img align="left" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/uvsensor.jpg?raw=true" style="max-width:100%;" height="100">

How to measure UV Light
-------------------------------------

UV sensors like the [GUVA-S12SD](https://amzn.to/3jP4JCA) to the left, are Ultraviolet testers and outdoor ultraviolet detectors that use calibrated light sensing algorithms that can calculate UV Index. This type of sensors don't contain an actual UV sensing element, instead they approximate it based on visible & IR light from the sun. This particular sensor measures ambient ultraviolet intensity in the 200 to 370 nanometer range, including solar ultraviolet UVA, UVB, and UVC light. This UV Sensor also provides a built-in amplifier with adjustable levels

LCD Screen - Schematic Diagram
------------------------------

The LCD needs six Arduino pins, all set to be digital outputs. It also needs 5V and GND connections.

There are several connections to be made. Lining up the display with the top of the breadboard helps to identify its pins without too much counting, especially if the breadboard has its rows numbered with row 1 as the top row of the board. Do not forget, the long yellow lead that links the slider of the pot to pin 3 of the display. The potentiometer is used to control the contrast of the display.

<img align="center" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/lcd-schema.png?raw=true" style="max-width:100%;" height="600">

Adding the DHT11 to the Schematic Diagram
-------------------------

Wire the [DHT11](https://amzn.to/2Qs9fcV) sensor to the [Arduino Uno](https://amzn.to/2RnHhPY) development board as shown in the following schematic diagram.

<img align="center" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/dht11-wiring.png?raw=true" style="max-width:100%;" height="600">


Adding the ESP8266 ESP01 to the Schematic Diagram
------------------------------

The ESP8266 WiFi Module is a self-contained stack that can give any microcontroller access to your WiFi network. The ESP8266 is capable of either hosting an application or offloading all Wi-Fi networking functions from another application processor. The ESP8266 module is an extremely cost-effective board with a huge, and ever-growing, community.

<img align="center" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/esp8266-schema.png?raw=true" style="max-width:100%;" height="600">

Adding the UV Sensor to the Schematic Diagram
------------------------------

<img align="center" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/uvsensor-wiring.png?raw=true" style="max-width:100%;" height="600">


Code Walkthrough
--------------------

The code itself is straightforward and well commented (help is of course welcome) but I wanted to explain something I struggled with when I first started learning my way through the Arduino world, and that is the difference between `Software Serial` and `Hardware Serial`.

Let's go through the function `sendDataToWiFiBoard()`. This function allows the Arduino board (the logic board) to communicate with the ESP8266 board (The WiFi board)

```cpp
String sendDataToWiFiBoard(String command, const int timeout, boolean debug)
```

So after connecting the two boards following the wiring diagram above, and initializing a new Software Serial object called `wifi`
```cpp
// ESP TX => Uno Pin 2
// ESP RX => Uno Pin 3
SoftwareSerial wifi(2, 3);
```

Anything passed to the `print()` function will be sent through the pin 2 to the WiFi board.
```cpp
  wifi.print(command); // send the read character to the esp8266
```

This is different than calling the same `print()` function on the `Serial` object, which sends the string through the Serial monitor.

Pin 3 is then used to read any response coming back from the WiFi board through the `read()` function.
```cpp
  while((time+timeout) > millis()) {
    while(wifi.available()) {
      // The esp has data so display its output to the serial window
      char c = wifi.read(); // read the next character.
      response+=c;
    }
  }

  return response;
}
```

Next Step
---------
For code that goes into the WiFi board (ESP8266 ESP01) and more explanation, please head out to this repo: https://github.com/MecaHumArduino/esp8266-01-aws-mqtt