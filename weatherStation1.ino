//First Project from "Home Automation with the ESP8266"
//Serial Line Weather Station w/ DHT11 Module
//ESP8266-01

#include "DHT.h"

#define DHTPIN 0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {
  Serial.begin(115200);

  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  float temp = (t*1.8) + 32;

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%\t");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("*F ");

  delay(2000);

}
