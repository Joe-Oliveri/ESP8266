// Home Automation System - SENSOR Program
// w/ Auto-Reconnect
// ESP8266-01

// Import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>
#include "DHT.h"

// Pin
#define DHTPIN 5

// Use DHT11 sensor
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char* ssid = "xxxx";
const char* password = "xxxx";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables
float temperature;
float humidity;

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
    
  // Give name and ID to device
  rest.set_id("2");
  rest.set_name("sensor");
  
  // Expose variables to API
  rest.variable("temperature", &temperature);
  rest.variable("humidity", &humidity);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
  
  // Init DHT 
  dht.begin();
  
}

void loop() {
  
  // Reading temperature and humidity
  humidity = dht.readHumidity();
  // Read temperature as Celsius
  temperature = dht.readTemperature();
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
 
}
