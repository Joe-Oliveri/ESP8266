// Fifth Project from "Home Automation with the ESP8266"
// Simple WiFi Motion Sensor
// w/ Auto-Reconnect
// ESP8266-01

#include <ESP8266WiFi.h>

const char* ssid = "xxxx";
const char* password = "xxxx";

// Host PC
const char* host = "192.168.4.20";

unsigned long previousMillis = 0;
unsigned long interval = 30000;

int sigPin = 3;
int old_motion_state = 0;

WiFiServer server(80);

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
  Serial.begin(115200);
  initWiFi();
  delay(10);

  pinMode(sigPin, INPUT);
  
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server Started");

  Serial.println(WiFi.localIP());
}

void loop() {

  unsigned long currentMillis = millis();

  int new_motion_state = digitalRead(sigPin);

  // If different, send to server
  if (new_motion_state != old_motion_state) {

    // Set new state
    old_motion_state = new_motion_state;

    Serial.print("Connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 3000;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    // This will send the request to the server
    client.print(String("POST /motion?state=") + String(new_motion_state) + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
    delay(10);

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
  }
  
  // If WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    Serial.println(WiFi.localIP());
    //Alternatively, you can restart your board
    //ESP.restart();
    Serial.println(WiFi.RSSI());
    previousMillis = currentMillis;
  }
}
