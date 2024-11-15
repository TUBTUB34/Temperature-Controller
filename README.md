# ESP8266 Temperature State Controller

I have this controller attached to my thermostat. Each setting (Warn, Hot, Cold) adjusts a servo motor position and changes the temperature on the thermostat.

## Features

- **Web Server**: Hosts a simple web interface accessible via an IP address assigned to the ESP8266.
- **OLED Display**: Shows different visual patterns for each temperature setting and tracks the number of times each setting is selected.
- **Thermostat Control**: Adjusts a connected servo motor's position for each setting (Warn, Hot, Cold), updating the temperature on the thermostat accordingly.
- **State Count Tracking**: Tracks and displays the count of each state on both the web interface and the OLED display.

## Hardware Requirements

- ESP8266 microcontroller
- SSD1306 OLED Display (128x64 resolution)
- Internet connection (WiFi)
- Servo motor
- 3D printer

## Libraries Required

To compile this project, you will need to install the following libraries in your Arduino IDE:

1. **ESP8266WiFi** - Included with ESP8266 core (https://github.com/esp8266/Arduino)
2. **ESP8266WebServer** - Included with ESP8266 core (https://github.com/esp8266/Arduino)
3. **ESP8266mDNS** - Included with ESP8266 core (https://github.com/esp8266/Arduino)
4. **U8g2** - Install from Arduino Library Manager or [here](https://github.com/olikraus/u8g2)

## Installation and Setup

1. Clone this repository or download the code as a ZIP file and open it in the Arduino IDE.

2. Upload Code: Connect the ESP8266 to your computer, select the appropriate board (e.g., "NodeMCU 1.0 (ESP-12E Module)"), and upload the code.

3. In a future push i will upload the STL's for mine, But your thermostat will probably not be the same dimensions 

## Usage

1. Once the ESP8266 is connected to WiFi, open the Serial Monitor to view the IP address assigned to the device.
2. Open a web browser and enter `http://<ESP_IP_ADDRESS>` to access the interface.
3. Click on **Warn**, **Hot**, or **Cold** to change the temperature setting on the thermostat. Each setting adjusts the servo motor position to update the thermostat temperature accordingly:
   - **Warn**: Sets the servo motor to a specific position for a low temperature warning.
   - **Hot**: Adjusts the motor to a position for a higher temperature setting.
   - **Cold**: Moves the motor to a position for a colder temperature setting.

The selected setting count is displayed on the OLED screen and updated in real-time on the web interface.


