#  Host a website with ESP32 to show their data
## Apply Data
Use `setHumidity(value)`, `setTemperature(value)`, and `setResistance(value)` where value is normalized between 0 and 1 to set the data.
Use `randomizeHumidity(enabled)`, `randomizeResistance(enabled)`, and `randomizeTemperature(enabled)` where enabled is 0 or 1 to randomize the data every 2 seconds.
Use `randomizeAll(enabled)` where enabled is 0 or 1 to randomize all data every 2 seconds.
