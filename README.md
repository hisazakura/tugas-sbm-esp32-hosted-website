#  ESP32 Hosted Data Viewer Website
Host a website in ESP32 to view their data in real-time using websocket.
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

## Accessing Website
1. Run ESP32
2. Connect to the same WiFi as the board
3. In serial monitor, find the local IP the board is using to host the website (if not found try resetting the board)
4. Connect to the IP using a browser

### Miscellaneous
You can forcibly set the data at current time or randomize it.
Use `setHumidity(value)`, `setTemperature(value)`, and `setResistance(value)` where value is normalized between 0 and 1 to set the data.
Use `randomizeHumidity(enabled)`, `randomizeResistance(enabled)`, and `randomizeTemperature(enabled)` where enabled is 0 or 1 to randomize the data every 2 seconds.
Use `randomizeAll(enabled)` where enabled is 0 or 1 to randomize all data every 2 seconds.
