#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>  
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_Address 0x3C
Adafruit_SSD1306 oled(1);

ESP8266WebServer server;

char* ssid = "2.4 Ghz Rokudo";
char* password = "hitler321";


void setup() {  
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);    
  oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/",[](){
    server.send(200,"text/plain","Heyya Meit !");
    });
  server.on("/update", handleUpdateCall);
  server.begin();
  
  // display the server ip address on the screen upon wifi connection
  oled.println("HTTP Server Started:\n" + WiFi.localIP().toString());
  oled.display();
}

void loop() {
  
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
  
  server.send(200, "text/plain", message);          //Returns the HTTP response
}
