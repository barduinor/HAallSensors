# 1 "/Users/rvb/Documents/Arduino/Barduino/HAallSensors/GarageDoorOperations/GarageDoorOperations.ino"
# 1 "/Users/rvb/Documents/Arduino/Barduino/HAallSensors/GarageDoorOperations/GarageDoorOperations.ino"
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

//#define MY_RADIO_RFM69





# 38 "/Users/rvb/Documents/Arduino/Barduino/HAallSensors/GarageDoorOperations/GarageDoorOperations.ino" 2
# 39 "/Users/rvb/Documents/Arduino/Barduino/HAallSensors/GarageDoorOperations/GarageDoorOperations.ino" 2
# 40 "/Users/rvb/Documents/Arduino/Barduino/HAallSensors/GarageDoorOperations/GarageDoorOperations.ino" 2
# 61 "/Users/rvb/Documents/Arduino/Barduino/HAallSensors/GarageDoorOperations/GarageDoorOperations.ino"
NewPing sonar(4 /* Arduino pin tied to trigger pin on the ultrasonic sensor.*/, 7 /* Arduino pin tied to echo pin on the ultrasonic sensor.*/, 300 /* Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.*/); // NewPing setup of pins and maximum distance.

MyMessage msgDoor(1 /* Door position - Distance*/, V_PERCENTAGE); // Door position
//  MyMessage msgLight(CHILD_LIGHT_ID, V_LIGHT); // Light Activate
MyMessage msgSwitch(3 /*  Switch/Button activate message*/, V_DIRECTION); // Switch Activate 0 reset, 1 door, 2 light

int lastDist;
bool metric = true;
int repeatCounter = 0;
bool isIdle = true;
int doorPosition = 100; //door position 0 open 100 closed

void setup()
{
  metric = getControllerConfig().isMetric;
}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Garage", "1.0");

  // Register all sensors to gw (they will be created as child devices)
  present(1 /* Door position - Distance*/, S_DISTANCE, "Door position");
  //  present(CHILD_LIGHT_ID, S_BINARY, "Light Message");
  present(3 /*  Switch/Button activate message*/, S_DOOR, "Switch message");

}

void loop()
{

  int dist = metric ? sonar.ping_cm() : sonar.ping_in();
  if (dist < 20 /* max distance to consider closed*/)
    dist = 20 /* max distance to consider closed*/;
  else if (dist > 190 /* min distance to consider open*/)
    dist = 190 /* min distance to consider open*/;

  doorPosition = map(dist, 20 /* max distance to consider closed*/, 190 /* min distance to consider open*/, 0, 100);

  if (doorPosition != lastDist) {

    lastDist = doorPosition;


    send(msgDoor.set(doorPosition));

    repeatCounter = 0;

  } else {
    repeatCounter++;
  }
  sendBatteryLevel(100);

  if (repeatCounter > 10) {
    isIdle = true;
    send(msgSwitch.set(0));
  }

  Serial.print("Ping: ");
  Serial.print(dist); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.print(metric ? " cm " : " in ");
  Serial.print("Door Position ");
  Serial.print(doorPosition);
  Serial.print(" Counter: ");
  Serial.print(repeatCounter);
  Serial.print(" Is Idle:");
  Serial.print(isIdle);
  Serial.println();

  if (isIdle) {
    wait(10*1000 /* ms wait ping time while door is idle*/, 1, V_DIRECTION);
  } else {
    wait(500 /* ms wait ping time while door is operating*/, 1, V_DIRECTION);
  }
}

void receive(const MyMessage &message) {

  if (message.isAck()) {
    Serial.println("This is an ack from gateway");
  }

  if (message.type == V_DIRECTION) {
    int mode = message.getInt();
    Serial.print("Got Message:");
    Serial.print(mode);
    Serial.println();
    if (mode == 2) {
      lightActivate();
    }
    else if (mode == 1) {
      doorActivate();
    }

  }
}

void doorActivate() {
      send(msgSwitch.set(1));
  digitalWrite(3 /*Aruino pin to control relay to trigger door*/, 1);
  delay(1000 /* ms to actuate relay for door*/);
  digitalWrite(3 /*Aruino pin to control relay to trigger door*/, 0);
  isIdle = !isIdle;
  repeatCounter = 0;
}

void lightActivate() {
      send(msgSwitch.set(2));
  digitalWrite(3 /*Aruino pin to control relay to trigger door*/, 1);
  delay(250 /* ms to actuate light for door*/);
  digitalWrite(3 /*Aruino pin to control relay to trigger door*/, 0);
  repeatCounter = 0;
}
