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
//#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#define MY_REPEATER_FEATURE

#define MY_NODE_ID 20 // 20,21,22,...,29 House Ground Floor sensons
#include <SPI.h>
#include <MySensors.h>

#define CHILD_INDICATOR_ID 1
#define LED_PIN 3 // Pin for led indicator

void setup()
{
  //metric = getControllerConfig().isMetric;
  pinMode( LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void presentation() {
  present(CHILD_INDICATOR_ID, S_BINARY, "Garage Door Indicator");
}

void loop()
{
}

void receive(const MyMessage &message) {
  bool isOpen = 0;
  isOpen = message.getBool();

//  Serial.print("Got msg:");
//  Serial.print(message.type);
//  Serial.print("/");
//  Serial.print(isOpen);
//  
  
  
  if (message.isAck()) {
//    Serial.println("This is an ack from gateway");
  }

  if (message.type == V_STATUS) {
    activateLight(isOpen);
  }

  
  Serial.println();
}

void activateLight(bool isOpen) {
//  Serial.print(" Activate:");
//  Serial.print(isOpen);
  digitalWrite(LED_PIN, isOpen);
}
