#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h> 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>


#define WHITE           0xFFFF
 
char ssid[] = "2.4 Ghz Rokudo";                // your network SSID (name)
char pass[] = "hitler321";             // your network password
 
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);
 
WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,2,104);        // remote IP of your computer
const unsigned int outPort = 9001;          // remote port to receive OSC
const unsigned int localPort = 8001;        // local port to listen for OSC packets (actually not used for sending)
 
// Waveshare SSD1331 96x64 .95inch
char OLEDbuffer[50];
 
void setup(void) {
  Serial.begin(9600);
  Serial.print("hello!");
  display.begin();
 
  Serial.println("init");
  printHome();
  // Dynamic Text
  drawtext("Connecting to",WHITE,1,5,24,0); drawtext(ssid,WHITE,1,5,33,0);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
 
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
   
  printHome();
  // Dynamic Text
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
   
  drawtext("Connected to",WHITE,1,5,24,0);
  sprintf(OLEDbuffer,"%s",WiFi.localIP().toString().c_str());
  drawtext(OLEDbuffer,WHITE,1,5,33,0);
  sprintf(OLEDbuffer,"Udp: %d",(int) Udp.localPort());  
  drawtext(OLEDbuffer,WHITE,1,5,42,0);
  delay(1000);
  printPage();
  delay(500);
  drawtext("OK",WHITE,2,35,23,0);
}
 
void loop() {
}
 
void printHome() {
  drawtext("ZeroBot",WHITE,2,5,0,1); delay(500);
  display.drawFastHLine(0, 20, display.width()-1, WHITE); delay(500);
  drawtext("Joystick v1.0",WHITE,1,18,display.height()-8,0);delay(500);
}
 
void printPage() {
  drawtext("ZeroBot",WHITE,1,0,0,1); delay(500);
  display.drawFastHLine(0, 10, display.width()-1, WHITE); delay(500);
  drawtext("Joystick v1.0",WHITE,1,18,display.height()-8,0);delay(500);
}
 
void drawtext(char *text, uint16_t color, uint8_t fontsize, uint8_t x, uint8_t y, boolean clean) {
  if ( clean ) display.fillScreen(WHITE);
  display.setTextSize(fontsize);
  display.setTextColor(color);
  display.setCursor(x,y);
  display.println(text);
}
