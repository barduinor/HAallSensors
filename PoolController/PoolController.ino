/**
   The MySensors Arduino library handles the wireless radio link and protocol
   between your home built sensors/actuators and HA controller of choice.
   The sensors forms a self healing radio network with optional repeaters. Each
   repeater and gateway builds a routing tables in EEPROM which keeps track of the
   network topology allowing messages to be routed to nodes.

   Documentation: http://www.mysensors.org
   Support Forum: http://forum.mysensors.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   version 2 as published by the Free Software Foundation.

 *******************************

   REVISION HISTORY
   Version 1.0 - Barduino

   DESCRIPTION
   Garage door operations
   Button to trigger oprn door or light based on relay open delay
   Garage door position based on a distance sensor
*/

// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

//#define MY_REPEATER_FEATURE

#define MY_NODE_ID 11 // 10,11,12,...,19 Garage sensors

#include <SPI.h>
#include <MySensors.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

bool metric;


void setup()
{
  metric = getControllerConfig().isMetric;
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();


}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Pool", "1.0");


}

void loop()
{

    // draw the first ~12 characters in the font
  mockDisplay();
  display.display();
  delay(2000);
  display.clearDisplay();
  
}

void receive(const MyMessage &message) {

}

void mockDisplay(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.print("Mon 07:07p");

  display.setTextSize(1);
  display.setCursor(5,0);
  display.print("AUTO");

  display.display();
  delay(1);
  
  }

void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
  delay(1);
}


