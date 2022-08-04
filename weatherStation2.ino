//Second Project from "Home Automation with the ESP8266"
//Web Server Weather Station w/ DHT11 Module
//ESP8266-01

#include "DHT.h"
#include <ESP8266WiFi.h>

#define DHTPIN 0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE, 15);

const char* ssid = "xxxx";
const char* password = "xxxx";

WiFiServer server(80);

void setup() {
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  delay(10);

  dht.begin();

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server Started");

  Serial.println(WiFi.localIP());

}

void loop() {

  WiFiClient client = server.available();
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  float temp = (t*1.8) + 32;

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<head>";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  s += "<meta http-equiv=\"refresh\" content=\"60\" />";
  s += "<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>";
  s += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">";
  s += "<style>body{font-size: 24px;} .voffset {margin-top: 30px;}</style>";
  s += "</head>";

  s += "<div class=\"container\">";
  s += "<h1>Basement Weather Station</h1>";

  s += "<div class=\"row voffset\">";
  s += "<div class=\"col-md-3\">Temperature: </div><div class=\"col-md-3\">" + String(temp) + "F" + "</div>";
  s += "<div class=\"col-md-3\">Humidity: </div><div class=\"col-md-3\">" + String(h) + "%" + "</div>";
  s += "</div>";

  s += "</div>";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disconnected");

}
