#  ESP32 Hosted Data Viewer Website
Host a website in ESP32 to view their data in real-time using websocket and monitoring with [MQTT](https://mqtt.org/) protocol.
## Uploading
1. Install dependencies
2. Modify `const char *ssid` and `const char *password` to your WiFi credentials.
3. Upload sketch data through `Tools > ESP32 Sketch Data Upload`
4. Upload to board

### Installing Dependencies
Use Arduino IDE to install dependencies through `Sketch > Include Library > Manage Libraries...` or `Sketch > Include Library > Add .ZIP Library...`
### Dependencies
- [Arduino_JSON](https://github.com/arduino-libraries/Arduino_JSON)
- [WiFi](https://github.com/arduino-libraries/WiFi)
- [AsyncTCP](https://github.com/dvarrel/AsyncTCP)
- [DHT sensor library for ESPx](https://github.com/beegee-tokyo/DHTesp)
- [ESPAsyncTCP](https://github.com/dvarrel/ESPAsyncWebSrv)
- PubSubClient

## Accessing Website
1. Run ESP32
2. Connect to the same WiFi as the board
3. In serial monitor, find the local IP the board is using to host the website (if not found try resetting the board)
4. Connect to the IP using a browser

## Connectiong node-red to mqtt
1. Install Node-RED: Ensure that you have Node-RED installed on your laptop. You can follow the official Node-RED documentation to install it based on your operating system.

2. Install MQTT Nodes: Open the Node-RED interface in your browser by navigating to http://localhost:1880 (assuming you have installed Node-RED locally). On the left side of the screen, click on the menu icon (three horizontal lines) and select "Manage palette." In the "Palette" tab, search for "node-red-contrib-mqtt-broker" and install it. This package provides MQTT nodes for Node-RED.

3. Configure MQTT Broker Node: Drag and drop an MQTT Broker node from the "Network" section in the Node-RED palette onto the flow canvas. Double-click on the node to open its configuration. Add port (1883 as default)


4. Configure MQTT Subscribe Nodes: Drag and drop an MQTT subscribe node from the "Network" section onto the flow canvas. Double-click on the node to open its configuration. In the "Add new mqtt-subscribe" section, enter the following details:
In the "Add new mqtt-broker" section, enter the following details:      
    Server: Enter the IP address of your laptop.
    Port: Enter the MQTT broker port (1883 as default).
Topic: Enter the topic to subscribe to. For example, "ESP32/Temp" or any other topic used in your code.
MQTT Broker: Select the MQTT broker node you configured in the previous step.
Repeat this step for each topic you want to subscribe to.


5. Connect the Nodes: Connect the MQTT subscribe nodes to the desired downstream nodes in your flow. These downstream nodes could be debug nodes, function nodes, or any other nodes based on your requirements. Similarly, connect the upstream nodes (e.g., sensors, input nodes) to the MQTT publish nodes.

6. Deploy the Flow: Once you have configured the MQTT nodes and connected them appropriately, click on the "Deploy" button in the top-right corner of the Node-RED interface. This will activate your flow and establish the MQTT connections.

### Miscellaneous
You can forcibly set the data at current time or randomize it.
Use `setHumidity(value)`, `setTemperature(value)`, and `setResistance(value)` where value is normalized between 0 and 1 to set the data.
Use `randomizeHumidity(enabled)`, `randomizeResistance(enabled)`, and `randomizeTemperature(enabled)` where enabled is 0 or 1 to randomize the data every 2 seconds.
Use `randomizeAll(enabled)` where enabled is 0 or 1 to randomize all data every 2 seconds.