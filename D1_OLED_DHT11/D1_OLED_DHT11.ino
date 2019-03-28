#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>

#define DHTPIN D4
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);
#define OLED_Address 0x3C

//DHT S Pin = D4
//OLED sck D1, sda D2

Adafruit_SSD1306 oled(1);

void setup() {
  Serial.begin(9600);
  dht.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
}

void loop() {
  delay(500);
  oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setTextSize(2);
  oled.setCursor(0, 0);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  //temp in c
  oled.println("temp");
  oled.print(t);
  oled.println(" c");
  oled.println("humidity");
  oled.print(h);
  oled.println(" %");
  oled.display();
}
