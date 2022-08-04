//Web Server Weather Station w/ DHT11 Module
//And OpenWeatherAPI
//w/ Auto-Reconnect
//ESP8266-01

#include "DHT.h"
#include <ESP8266WiFi.h>

#define DHTPIN 0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE, 15);

const char* ssid = "xxxx";
const char* password = "xxxx";

unsigned long previousMillis = 0;
unsigned long interval = 30000;

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
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

  dht.begin();

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

  WiFiClient client = server.available();
  if (!client){
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
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
  s += "<div class=\"col-md-3\">Basement Temperature: </div><div class=\"col-md-3\">" + String(temp) + "*F" + "</div>";
  s += "<div class=\"col-md-3\">Basement Humidity: </div><div class=\"col-md-3\">" + String(h) + "%" + "</div>";
  s += "</div>";

  s += "<div class=\"row voffset\">";
  s += "<div class=\"col-md-3\">Exterior Temperature: </div><div id=\"ext_temp\" class=\"col-md-3\"></div>";
  s += "<div class=\"col-md-3\">Exterior Humidity: </div><div id=\"ext_humidity\" class=\"col-md-3\"></div>";
  s += "</div>";

  s += "</div>";

  s += "<script>$.ajax({url: \"http://api.openweathermap.org/data/2.5/weather?id=5106298&APPID=07f5b7fcfa144b0eba32d2b317203b76&units=imperial\", crossDomain: true})";
  s += ".done(function(result) {$(\"#ext_temp\").html((result.main.temp).toFixed(2)); $(\"#ext_humidity\").html(result.main.humidity);})";
  s += "</script>";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disconnected");

}
