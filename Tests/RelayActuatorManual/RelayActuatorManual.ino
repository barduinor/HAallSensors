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
//#define MY_REPEATER_FEATURE

#include <SPI.h>
#include <MySensor.h>

//#define RELAY_1  3  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
//#define NUMBER_OF_RELAYS 4 // Total number of attached relays
#define RELAY_ON 1  // GPIO value to write to turn on attached relay
#define RELAY_OFF 0 // GPIO value to write to turn off attached relay

MyMessage msg1(1, V_LIGHT);
MyMessage msg2(2, V_LIGHT);
MyMessage msg3(3, V_LIGHT);
MyMessage msg4(4, V_LIGHT);


void setup()  
{ 
  
//  send(msg1.set(0));
//  send(msg2.set(0));
//  send(msg3.set(0));
//  send(msg4.set(0));
  
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
}

void presentation()  
{   
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Relay", "1.0");
    // Register all sensors to gw (they will be created as child devices)
    present(1, S_LIGHT);
    present(2, S_LIGHT);
    present(3, S_LIGHT);
    present(4, S_LIGHT);
  
}


void loop() 
{
    Serial.println("Starting in 3 seconds");
    wait(3000);
    
    for( int i=3;i<=6;i++){
      digitalWrite(i,LOW);
      Serial.print("Pin:");
      Serial.print(i);
      Serial.println(" to LOW");
      wait(500);
    }
    
    for( int i=3;i<=6;i++){
      digitalWrite(i,HIGH);
      Serial.print("Pin:");
      Serial.print(i);
      Serial.println(" to HIGH");
      wait(500);
    
    }

}

//void receive(const MyMessage &message) {
//  // We only expect one type of message from controller. But we better check anyway.
//  if (message.type==V_LIGHT) {
//     // Change relay state
//     digitalWrite(message.sensor-1+RELAY_1, message.getBool()?RELAY_ON:RELAY_OFF);
//     // Store state in eeprom
//     saveState(message.sensor, message.getBool());
//     // Write some debug info
//     Serial.print("Incoming change for sensor:");
//     Serial.print(message.sensor);
//     Serial.print(", New status: ");
//     Serial.println(message.getBool());
//   } 
//}

