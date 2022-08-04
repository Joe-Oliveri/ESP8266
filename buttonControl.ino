// Fourth Project from "Home Automation with the ESP8266"
// Web Server w/ Button Control
// w/ Auto-Reconnect
// ESP8266-01

#include <ESP8266WiFi.h>

const char* ssid = "Vincent LAN Gogh";
const char* password = "Acidburn1";

unsigned long previousMillis = 0;
unsigned long interval = 30000;

int LED = 0;

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

  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
  
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

  WiFiClient client = server.available();
  if (!client){
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
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

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  if (req.indexOf("/on") != -1){
    digitalWrite(LED, 1);
  }
  else if (req.indexOf("/off") != -1) {
    digitalWrite(LED, 0);
  }

  client.flush();
  
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<head>";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  s += "<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>";
  s += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">";
  s += "</head>";
  s += "<div class=\"container\">";
  s += "<h1>Lamp Control</h1>";
  s += "<div class=\"row\">";
  s += "<div class=\"col-md-2\"><input class=\"btn btn-block btn-lg btn-primary\" type=\"button\" value=\"On\" onclick=\"on()\"></div>";
  s += "<div class=\"col-md-2\"><input class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"Off\" onclick=\"off()\"></div>";
  s += "</div></div>";
  s += "<script>function on() {$.get(\"/on\");}</script>";
  s += "<script>function off() {$.get(\"/off\");}</script>";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disconnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
