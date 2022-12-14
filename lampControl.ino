// Sixth Project from "Home Automation with the ESP8266"
// Home Automation System - LAMP Program
// w/ Auto-Reconnect
// ESP8266-01

#include <aREST.h>
#include <ESP8266WiFi.h>
#include <creds.h>

// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char *mySSID = ssid;
const char *myPassword = pass;

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
    
  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("lamp_control");
  
  // Connect to WiFi
  WiFi.begin(mySSID, myPassword);
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
  
}

void loop() {
  
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
