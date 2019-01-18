#include <ESP8266WiFi.h>

const char* ssid     = "Rokudo";
const char* password = "hitler321";

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA); //We don't want the ESP to act as an AP
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1);
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected.");  
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    delay(500);
  }
}
