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
 * Motion Sensor example using HC-SR501 
 * http://www.mysensors.org/build/motion
 *
 */

// Enable debug prints
// #define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <SPI.h>
#include <MySensor.h>
#include <DHT.h>  

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define CHILD_ID_PIR 2   // Id of the sensor child

#define PIR_SENSOR_DIGITAL_PIN 3   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)
#define HUMIDITY_SENSOR_DIGITAL_PIN 2

unsigned long SLEEP_TIME = 120000; // Sleep time between reports (in milliseconds)

DHT dht;
float lastTemp;
float lastHum;
boolean metric = true; 

// Initialize motion message

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgPir(CHILD_ID_PIR, V_TRIPPED);

void setup()  
{  
  pinMode(PIR_SENSOR_DIGITAL_PIN, INPUT);      // sets the motion sensor digital pin as input
  pinMode(HUMIDITY_SENSOR_DIGITAL_PIN, INPUT);      // sets the motion sensor digital pin as input
  
  dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 
  metric = getConfig().isMetric;
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Motion & Temperature Sensor", "1.0");

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_HUM, S_MOTION);
  present(CHILD_ID_HUM, S_HUM);
  present(CHILD_ID_TEMP, S_TEMP);
}

void loop()     
{     
  // Read digital motion value
  boolean tripped = digitalRead(PIR_SENSOR_DIGITAL_PIN) == HIGH; 
  #ifdef MY_DEBUG  
    Serial.print('Pir sensor status:');  
    Serial.println(tripped);
  #endif
  send(msgPir.set(tripped?"On":"Off"));  // Send tripped value to gw 
  
  readTempHum();

  // Sleep until interrupt comes in on motion sensor. Send update every two minute.
  sleep(digitalPinToInterrupt(PIR_SENSOR_DIGITAL_PIN), CHANGE, SLEEP_TIME);
}

void readTempHum()
{  
  delay(dht.getMinimumSamplingPeriod());
 
  // Fetch temperatures from DHT sensor
  float temperature = dht.getTemperature();
  if (isnan(temperature)) {
      Serial.println("Failed reading temperature from DHT");
  } else if (temperature != lastTemp) {
    lastTemp = temperature;
    if (!metric) {
      temperature = dht.toFahrenheit(temperature);
    }
    send(msgTemp.set(temperature, 1));
    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature);
    #endif
  }
  
  // Fetch humidity from DHT sensor
  float humidity = dht.getHumidity();
  if (isnan(humidity)) {
      Serial.println("Failed reading humidity from DHT");
  } else if (humidity != lastHum) {
      lastHum = humidity;
      send(msgHum.set(humidity, 1));
      #ifdef MY_DEBUG
      Serial.print("H: ");
      Serial.println(humidity);
      #endif
  }
  
  sleep(SLEEP_TIME); //sleep a bit
}

