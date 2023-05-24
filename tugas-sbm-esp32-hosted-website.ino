#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <DHTesp.h>

#define DHT_PIN 15
#define POT_PIN 35

// Debug function
void log(String message) {
	unsigned int time = millis();
	Serial.print("DEBUG [");
	Serial.print(String(time/60000)+"m ");
	Serial.print(String(time/1000 % 60)+"s ");
	Serial.print(String(time % 1000)+"ms] : ");
	Serial.println(message);
}

// Replace with your network credentials
const char *ssid = "Kopi Sebaya";
const char *password = "americano";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 500;

// Create a sensor object
DHTesp dhtSensor;

// Initialize DHT
void initDHT()
{
	log("Initializing DHT Sensor...");
	dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
	log("Initialized DHT Sensor.");
}

// Get Sensor Readings and return JSON object
String getSensorReadings()
{
	log("Retrieving sensor data...");
	TempAndHumidity data = dhtSensor.getTempAndHumidity();
	readings["temperature"] = String(data.temperature, 2);
	readings["humidity"] = String(data.humidity, 1);
	readings["resistance"] = String(analogRead(POT_PIN));
	String jsonString = JSON.stringify(readings);
	log("Retrieved sensor data.");
	return jsonString;
}

// Initialize SPIFFS
void initSPIFFS()
{
	if (!SPIFFS.begin())
	{
		Serial.println("An error has occurred while mounting SPIFFS");
	}
	Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi()
{
	log("Initializing WiFi...");
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	Serial.print("Connecting to WiFi ..");
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print('.');
		delay(1000);
	}
	log("Initialized WiFi...");
	Serial.println();
	Serial.print("Local IP started at: ");
	Serial.println(WiFi.localIP());
}

void setup()
{
	// Serial port for debugging purposes
	Serial.begin(115200);
	initDHT();
	initWiFi();
	initSPIFFS();

	// Web Server Root URL
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(SPIFFS, "/index.html", "text/html"); 
	});

	server.serveStatic("/", SPIFFS, "/");

	// Request for the latest sensor readings
	server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request) {
		String json = getSensorReadings();
		request->send(200, "application/json", json);
		json = String(); 
	});

	events.onConnect([](AsyncEventSourceClient *client) {
		if (client->lastId()) {
			Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
		}
		// send event with message "hello!", id current millis
		// and set reconnect delay to 1 second
		// client->send("hello!", NULL, millis(), 10000); 
	});
	server.addHandler(&events);

	// Start server
	server.begin();
}

void loop()
{
	/*
	if ((millis() - lastTime) > timerDelay)
	{
		// Send Events to the client with the Sensor Readings Every 10 seconds
		events.send("ping", NULL, millis());
		// events.send(getSensorReadings().c_str(), "new_readings", millis());
		lastTime = millis();
	}
	*/
	delay(10);
}
