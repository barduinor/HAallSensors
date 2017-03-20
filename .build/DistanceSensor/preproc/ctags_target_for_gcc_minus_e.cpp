# 1 "/Users/rvb/Documents/Arduino/Barduino/HAallSensors/DistanceSensor/DistanceSensor.ino"
# 1 "/Users/rvb/Documents/Arduino/Barduino/HAallSensors/DistanceSensor/DistanceSensor.ino"
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
 * Version 1.0 - Henrik EKblad
 * 
 * DESCRIPTION
 * This sketch provides an example how to implement a distance sensor using HC-SR04 
 * http://www.mysensors.org/build/distance
 */

// Enable debug prints


// Enable and select radio type attached

//#define MY_RADIO_RFM69

# 37 "/Users/rvb/Documents/Arduino/Barduino/HAallSensors/DistanceSensor/DistanceSensor.ino" 2
# 38 "/Users/rvb/Documents/Arduino/Barduino/HAallSensors/DistanceSensor/DistanceSensor.ino" 2
# 39 "/Users/rvb/Documents/Arduino/Barduino/HAallSensors/DistanceSensor/DistanceSensor.ino" 2





unsigned long SLEEP_TIME = 5000; // Sleep time between reads (in milliseconds)

NewPing sonar(4 /* Arduino pin tied to trigger pin on the ultrasonic sensor.*/, 7 /* Arduino pin tied to echo pin on the ultrasonic sensor.*/, 300 /* Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.*/); // NewPing setup of pins and maximum distance.
MyMessage msg(1, V_DISTANCE);
int lastDist;
bool metric = true;

void setup()
{
  metric = getControllerConfig().isMetric;
}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Distance Sensor", "1.0");

  // Register all sensors to gw (they will be created as child devices)
  present(1, S_DISTANCE);
}

void loop()
{
  int dist = metric?sonar.ping_cm():sonar.ping_in();
  Serial.print("Ping: ");
  Serial.print(dist); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println(metric?" cm":" in");

  if (dist != lastDist) {
      send(msg.set(dist));
      lastDist = dist;
  }

  sleep(SLEEP_TIME);
}
