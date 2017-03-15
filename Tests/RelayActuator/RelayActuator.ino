/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 * 
 * DESCRIPTION
 * Example sketch showing how to control physical relays. 
 * This example will remember relay state after power failure.
 * http://www.mysensors.org/build/relay
 */ 
// Define Node ID
#define MY_NODE_ID 15


// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Enable repeater functionality for this node
#define MY_REPEATER_FEATURE

#include <SPI.h>
#include <MySensor.h>

#define RELAY_1  3  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
#define NUMBER_OF_RELAYS 4 // Total number of attached relays
#define RELAY_ON HIGH  // GPIO value to write to turn on attached relay
#define RELAY_OFF LOW // GPIO value to write to turn off attached relay

bool isFirstTime = true;

bool currState;

void setup()  
{ 
  for (int sensor=1, pin=RELAY_1; sensor<=NUMBER_OF_RELAYS;sensor++, pin++) {
    // Then set relay pins in output mode
    pinMode(pin, OUTPUT);  
    digitalWrite(pin,LOW);
    Serial.print("Pin ");
    Serial.print(pin);
    Serial.println(" to LOW");
  }
}

void presentation()  
{   
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Relay", "1.0");

  for (int sensor=1, pin=RELAY_1; sensor<=NUMBER_OF_RELAYS;sensor++, pin++) {
    // Register all sensors to gw (they will be created as child devices)
    present(sensor, S_LIGHT);
    Serial.print("Presenting sensor:");
    Serial.println(sensor);
  }
}


void loop() 
{
    if (isFirstTime){
      for (int sensor=1, pin=RELAY_1; sensor<=NUMBER_OF_RELAYS;sensor++,pin++) {
          
          currState = loadState(sensor);  
          
          pinMode(pin, OUTPUT);  
          digitalWrite(pin,LOW);
        
          Serial.print("Current Status sensor:");
          Serial.print(sensor);
          Serial.print("\t");
          Serial.print("State:");
          Serial.print(currState);
          Serial.print("\t");
          Serial.print("Pin:");
          Serial.println(pin);

          // Set relay to last known state (using eeprom storage) 
          digitalWrite(pin, currState?RELAY_ON:RELAY_OFF);
          wait(500);
      }
      isFirstTime = false;
    }

}

void receive(const MyMessage &message) {
  // We only expect one type of message from controller. But we better check anyway.
  if (message.type==V_LIGHT) {
     // Change relay state
     digitalWrite(message.sensor-1+RELAY_1, message.getBool()?RELAY_ON:RELAY_OFF);
     // Store state in eeprom
     saveState(message.sensor, message.getBool());
     // Write some debug info
     Serial.print("Incoming change for sensor:");
     Serial.print(message.sensor);
     Serial.print(" On pin:");
     Serial.print(message.sensor-1+RELAY_1);
     Serial.print(", New status: ");
     Serial.println(message.getBool());
   } 
}

