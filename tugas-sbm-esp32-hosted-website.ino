
//  Complete instructions at https://RandomNerdTutorials.com/esp32-web-server-gauges/


#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <DHTesp.h>
#include <PubSubClient.h>


// Replace with your network credentials
const char* ssid = "ESP32";
const char* password = "11111111";

//Mqtt server setup
const char* mqtt_server = "192.168.167.152";

//Topic setup
WiFiClient espClient;
PubSubClient client(espClient);
#define Temp_TOPIC             "ESP32/temp"
#define Humi_TOPIC             "ESP32/humi"
#define Poten_TOPIC            "ESP32/poten"

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
  
// Init DHT
void initDHT(){
  dhtSensor.setup(15, DHTesp::DHT22); // DHT22 connected to GPIO 4
}

//Init MQTT
void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    String clientId = "ESP32Client";
    if (client.connect(clientId.c_str())) {
      Serial.println(" Connected");
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

// Get Sensor Readings and return JSON object
String getSensorReadings(){
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  readings["temperature"] = String(data.temperature, 2);
  client.publish(Temp_TOPIC, readings["temperature"]);
  readings["humidity"] =  String(data.humidity, 1);
  client.publish(Humi_TOPIC, readings["humidity"]);
  readings["potensio"] = String(analogRead(35));
  client.publish(Poten_TOPIC, readings["potensio"]);
  String jsonString = JSON.stringify(readings);
  Serial.print(jsonString);
  return jsonString;
}

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  initDHT();
  initWiFi();
  initSPIFFS();
  client.setServer(mqtt_server, 1883);

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String();
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();
}

void loop() {
  if (!client.connected()) {
    mqttconnect();
  }
  client.loop();
  if ((millis() - lastTime) > timerDelay) {
    // Send Events to the client with the Sensor Readings Every 10 seconds
    events.send("ping", NULL, millis());
    events.send(getSensorReadings().c_str(), "new_readings", millis());
    ;lastTime = millis();
  }
}
