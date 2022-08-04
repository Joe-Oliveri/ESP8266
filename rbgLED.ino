// RGB LED mood light
// Controlled by Mobile Device
// w/ Blynk

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "xxxxxxxxxxx";

// Your WiFi credentials.
char ssid[] = "xxxx";
char pass[] = "xxxx";

// ESP8266-01 Output Pins
int bluePin = 0;
int greenPin = 2;
int redPin = 3;

void setup()
{
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);

  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  digitalWrite(redPin, LOW);

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
