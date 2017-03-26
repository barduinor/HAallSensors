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

#define MY_NODE_ID 10 // 10,11,12,...,19 Garage sensors

#include <SPI.h>
#include <MySensors.h>
#include <NewPing.h>

#define CHILD_DOOR_ID 1  // Door position - Distance
#define CHILD_LIGHT_ID 2 // Light activate message
#define CHILD_SWITCH_ID 3 //  Switch/Button activate message

#define CHILD_INDICATOR_ID 4 //  Remote indicator ld for garage door
#define REMOTE_DESTINATION 20 // Remote sensor is

#define TRIGGER_PIN  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define PING_DELAY_OPERATING 500 // ms wait ping time while door is operating
#define PING_DELAY_IDLE 5000  // ms wait ping time while door is idle

#define DISTANCE_OPEN 20 // max distance to consider closed
#define DISTANCE_CLOSE 250 // min distance to consider open

#define RELAY_PIN 6 //Aruino pin to control relay to trigger door
#define RELAY_ON 1
#define RELAY_OFF 0
#define RELAY_DELAY_DOOR 1000 // ms to actuate relay for door

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

MyMessage msgDoor(CHILD_DOOR_ID, V_PERCENTAGE); // Door position
MyMessage msgSwitch(CHILD_SWITCH_ID, V_STATUS); // Switch Activate 0 Idle 1 activate
MyMessage msgDist(CHILD_LIGHT_ID, V_DISTANCE);  // Control distance in cm from ping sensor

MyMessage msgIndicator(CHILD_INDICATOR_ID, V_STATUS); // message to remote indicator



int lastDoorPosition = 0;
bool metric = true;
int repeatCounter = 0;
bool isIdle = true;
int doorPosition = 100; //door position 0 open 100 closed

void setup()
{
  metric = getControllerConfig().isMetric;
  pinMode(RELAY_PIN, OUTPUT);

}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Garage", "1.0");

  // Register all sensors to gw (they will be created as child devices)

  present(CHILD_DOOR_ID, S_COVER, "Door position");

  present(CHILD_SWITCH_ID, S_BINARY, "Switch");

  present(CHILD_LIGHT_ID, S_DISTANCE, "Distance cm");

  msgIndicator.setDestination(REMOTE_DESTINATION);

}

void loop()
{
  int dist = metric ? sonar.ping_cm() : sonar.ping_in();
  int mapDist = 0;

  if (dist < DISTANCE_OPEN)
    mapDist = DISTANCE_OPEN;
  else if (dist > DISTANCE_CLOSE)
    mapDist = DISTANCE_CLOSE;
  else
    mapDist = dist;

  doorPosition = map(mapDist, DISTANCE_OPEN, DISTANCE_CLOSE, 0, 100);

  if (doorPosition != lastDoorPosition) {
    lastDoorPosition = doorPosition;
    repeatCounter = 0;
    isIdle = false;
    send(msgDist.set(dist));
    send(msgDoor.set(doorPosition));
  } else {
    repeatCounter++;
  }



  if (doorPosition >= 100) {
    send(msgIndicator.set(false)); // closed
    //Serial.println("\t\t sent closed");
  } else {
    send(msgIndicator.set(true)); // open
    //Serial.println("\t\t sent Open");
  }

  if (repeatCounter > 10) {
    isIdle = true;
  }


  //  Serial.print("Ping: ");
  //  Serial.print(dist); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  //  Serial.print(metric ? " cm " : " in ");
  //  Serial.print("Door Position ");
  //  Serial.print(doorPosition);
  //  Serial.print(" Counter: ");
  //  Serial.print(repeatCounter);
  //  Serial.print(" Is Idle:");
  //  Serial.print(isIdle);
  //  Serial.println();

  if (isIdle) {
    wait(PING_DELAY_IDLE, 1, V_STATUS);
  } else {
    wait(PING_DELAY_OPERATING, 1, V_STATUS);
  }
}

void receive(const MyMessage &message) {

  if (message.type == V_STATUS) {
    bool isActivated = message.getBool();

    if (isActivated) {
      send(msgSwitch.set(1));
      doorActivate();
      send(msgSwitch.set(0));
    }
    isIdle = false;
    repeatCounter = 0;
  }
}

void doorActivate() {

  digitalWrite(RELAY_PIN, RELAY_ON);
  delay(RELAY_DELAY_DOOR);
  digitalWrite(RELAY_PIN, RELAY_OFF);
}

/*

  void lightActivate() {
  send(msgSwitch.set(2));
  digitalWrite(RELAY_PIN, RELAY_ON);
  delay(RELAY_DELAY_DOOR);
  digitalWrite(RELAY_PIN, RELAY_OFF);
  repeatCounter = 0;
  }

*/




