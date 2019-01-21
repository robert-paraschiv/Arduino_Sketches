#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_Address 0x3C
Adafruit_SSD1306 oled(1);

void setup() {
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
}

void loop() {
  oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);
  oled.println("Fuck to ywwwwwwwwwwwwwwww !");
  oled.display();
}
