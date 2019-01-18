/********************************************************************************************
  "Blink"
  Turns onboard Relay on for two seconds, then off for two seconds, repeatedly.

  Modified arduino native Blink scetch to test the 'ESP8266 5V Wifi Relay Module' by
  LC Technology (www.lctech-inc.com) bought from Ebay. http://www.ebay.com/itm/291971732400
  First tested with Arduino without the ESP8266, then programmed ESP8266 with this scetch
  for standalone operation.                                                     - janaaage
  
*********************************************************************************************

* Onboard ESP8266 WIFI module, AP mode 5 client can be connected at the same time;
* Module has two work modes: 
  (1) cell phones carry on the WiFi moduledirectly;
  (2) cell phone andwifi modulecarryon the same router;
* Transmission distance: 
  (1) the open environment, the mobile phone when carrying on the WIFI module maximum 
      transmission distance of 400m;
  (2) when the WiFI module and cell phone carrying on thesamerouter,the transmission distance 
      depend on the router’s signal intensity;
* Onboard 5v, 10 A / 250 v AC 10 A / 30 v DC relay, absorb 100000 times continuously;
  Module with diode effusion protection, short response time;
* Module baud rate: 9600,8,1,0,0.
* Introduced the hardware and instructions
  Size: 45*28mm
* The board function description:
  IN +,IN-: 5Vpower input;
  TX ,RX and GND:  serial port debug pins

* If you want to use a computer to control relay, you can unplug the ESP8266 WiFi 
  module,and TX ,RX ,GND pin of USB to TTL module connect to TX ,RX ,GND pin 
  of ESP8266 relay module, IN+ and IN- connect to DC5V power,
  Send serial command(A00101A2 open relay; A00100A1 closed relay,command 
  format must be hex) with debugging software on the computer to control the relay.  

                      TX ON LC-RELAY GOES TO TX ON ARDUINO!!!
                      
*********************************************************************************************/

byte relON[] = {0xA0, 0x01, 0x01, 0xA2};  //Hex command to send to serial for open relay
byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1}; //Hex command to send to serial for close relay

int ledState = false;
unsigned long previousMillis = 0;
const long interval = 2000; //  2 seconds

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial:
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop()
{
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   
    if (ledState == true) {
      Serial.write(relON, sizeof(relON));     // turns the relay ON
      ledState = false;
    } else {
      Serial.write(relOFF, sizeof(relOFF));   // turns the relay OFF
      ledState = true;
    }    
  }
}
