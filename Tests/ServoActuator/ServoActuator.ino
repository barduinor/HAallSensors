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
 * Contribution by: Derek Macias
 *
 * DESCRIPTION
 * Example showing how to create an atuator for a servo.
 * Connect red to +5V, Black or brown to GND and the last cable to Digital pin 3.
 * The servo consumes much power and should probably have its own powersource.'
 * The arduino might spontanally restart if too much power is used (happend
 * to me when servo tried to pass the extreme positions = full load).
 * http://www.mysensors.org/build/servo
 */

// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Define Node ID
#define MY_NODE_ID 9

#include <SPI.h>
#include <MySensor.h>
#include <Servo.h>
#include <Vcc.h>

#define VCC_MIN 1.9
#define VCC_MAX 5.0

#define SERVO_DIGITAL_OUT_PIN 3
#define SERVO_MIN 10 // Fine tune your servos min. 0-180
#define SERVO_MAX 190  // Fine tune your servos max. 0-180
#define DETACH_DELAY 900 // Tune this to let your movement finish before detaching the servo
#define CHILD_ID 10   // Id of the sensor child

MyMessage msg(CHILD_ID, V_DIMMER);
MyMessage msgUp(CHILD_ID, V_UP);
MyMessage msgDown(CHILD_ID, V_DOWN);
MyMessage msgStop(CHILD_ID, V_STOP);
Servo myservo;  // create servo object to control a servo
// a maximum of eight servo objects can be created Sensor gw(9,10);
unsigned long timeOfLastChange = 0;
bool attachedServo = false;

Vcc vcc;

void setup()
{
  // Request last servo state at startup
  request(CHILD_ID, V_DIMMER);
  send(msg.set(0));
  send(msgUp.set(0));
  send(msgDown.set(0));
  send(msgStop.set(0));
  sendBatteryReport();
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Servo", "1.0");

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID, S_COVER);
}

void loop()
{
  if (attachedServo && millis() - timeOfLastChange > DETACH_DELAY) {
    myservo.detach();
    attachedServo = false;
  }
}

void receive(const MyMessage &message) {
  myservo.attach(SERVO_DIGITAL_OUT_PIN);
  attachedServo = true;
  if (message.type == V_DIMMER) { // This could be M_ACK_VARIABLE or M_SET_VARIABLE
    int val = message.getInt();
    //int x = SERVO_MAX + (((SERVO_MIN-SERVO_MAX))/100 * val);
    int x = map(val, 0, 100, SERVO_MIN, SERVO_MAX);
    myservo.write(x); // sets the servo position 0-180
    // Write some debug info
    Serial.print("Servo changed. new state: \t");
    Serial.print(val);
    Serial.print("\t");
    Serial.println(x);
    send(msg.set(val));
  } else if (message.type == V_UP) {
    Serial.println("Servo UP command");
    myservo.write(SERVO_MAX);
    send(msg.set(100));
  } else if (message.type == V_DOWN) {
    Serial.println("Servo DOWN command");
    myservo.write(SERVO_MIN);
    send(msg.set(0));
  } else if (message.type == V_STOP) {
    Serial.println("Servo STOP command");
    send(msg.set(myservo.read()));
    myservo.detach();
    attachedServo = false;

  }
  timeOfLastChange = millis();
  sendBatteryReport();
}

void sendBatteryReport() {
  float p = vcc.Read_Perc(VCC_MIN, VCC_MAX, true);
  int batteryPcnt = static_cast<int>(p);
  sendBatteryLevel(batteryPcnt);
}



