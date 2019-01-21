#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_Address 0x3C
Adafruit_SSD1306 oled(1);

#include <Wire.h>  

const char* ssid = "2.4 Ghz Rokudo"; //enter your SSID here
const char* password = "hitler321"; //enter your network password here

ESP8266WebServer server(80);

const int led = BUILTIN_LED;

// this function is what gets called if you enter the base ip address of your Wemos in the browser
void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

// this gets called if no function is for your address
void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/update", handleUpdateCall);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  oled.clearDisplay();

  // display the server ip address on the screen upon wifi connection
  oled.println("HTTP Server Started:\n" + WiFi.localIP().toString());
  oled.display();
}

void loop(void) {
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);
  server.handleClient();
}

// this gets called if you enter http://yourwemosaddress/update?message=SomeMessageText in a web browser
void handleUpdateCall() { 

  String message = "";
  
  //Message parameter wasn't found  
  if (server.arg("message")== ""){     
  
    message = "Message Argument not found";
  
  }else{
    // message was found, let's display it on the OLED
    message = "";
    message += server.arg("message");     //Gets the value of the query parameter
    oled.clearDisplay();
    oled.println( message);
    oled.display();
  
  }
  
  server.send(200, "text/plain", "OK");          //Returns the HTTP response
}
