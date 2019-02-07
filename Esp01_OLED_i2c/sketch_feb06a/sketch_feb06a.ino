/* ************************************
 Read the time from RTC and display on OLED
 with an ESP8266<br> sda=0, scl=2
* *************************************/

// Libraries
#include <Wire.h>
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
//Object declarations
SSD1306  display(0x3c, 0, 2);//0x3C being the usual address of the OLED



void setup() {
  Wire.pins(0, 2);// yes, see text
  Wire.begin(0,2);// 0=sda, 2=scl


  // Initialise the display.
  display.init();

}


void loop() {
  display.clear();
  //display.drawString(0, 10, t);//
  display.println("Da ?");
  display.display();
  delay(10);
}
