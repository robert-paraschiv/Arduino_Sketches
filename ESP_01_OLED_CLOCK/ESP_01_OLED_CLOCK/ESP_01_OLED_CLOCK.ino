#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>
#include <Fonts/FreeMono12pt7b.h>

// not available on ESP-01, but needed in the code
#define OLED_RESET 4
#define pin_led 1
Adafruit_SSD1306 display(OLED_RESET);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ro.pool.ntp.org", 7200, 60000);

ESP8266WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);

#ifndef STASSID
#define STASSID "2.4 Ghz Rokudo"
#define STAPSK  "hitler321"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;


void setup() {
  // set gpio0 and gpio2 as sda, scl
  Wire.begin(0,2);
  Serial.begin(115200);
  // initialize with the I2C addr 0x3C (for the 128x32)(initializing the display)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);  
  //display.setFont(&FreeMono12pt7b);
  display.setTextSize(2.5);
  display.setCursor(0,0);  
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  
  timeClient.begin();
  // Port defaults to 8266
   //ArduinoOTA.setPort(81);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  
  // display the server ip address on the screen upon wifi connection
  display.println("HTTP Server Started:\n" + WiFi.localIP().toString());
  display.display();

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    display.clearDisplay();
    display.println("OTA ENDED");
    display.display();
    delay(3000);
    ESP.restart();    
    
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  webSocket.loop();
  server.handleClient();
  // If you enter anything in the serial monitor it will display it in the WebPage TextArea
  if(Serial.available() > 0){
    char c[] = {(char)Serial.read()};
    webSocket.broadcastTXT(c, sizeof(c));
  }
  timeClient.update();
  display.println(timeClient.getFormattedTime());
  display.display();
  delay(1000);
}
