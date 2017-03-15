#include <SPI.h>
#include <MySensor.h>  
#include <DHT.h>  

#define CHILD_ID_HUM 1
#define CHILD_ID_TEMP 2
#define HUMIDITY_SENSOR_DIGITAL_PIN 4
unsigned long SLEEP_TIME = 900000; // Sleep time between reads (in milliseconds)

#define CE_PIN 9
#define CS_PIN 10

MySensor gw(CE_PIN,CS_PIN);


DHT dht;
float lastTemp;
float lastHum;
boolean metric = true; 
boolean dhtInF = false;
long lastMillis;

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);


void setup()  
{ 
  pinMode(HUMIDITY_SENSOR_DIGITAL_PIN,INPUT);
  dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 
  //dhtInF = (dht.getModel()==2);

  
  Serial.println("Begin");
  gw.begin(NULL,AUTO,true);
  gw.wait(5000);
  
  // Send the Sketch Version Information to the Gateway
  Serial.println("Send Sketch Info");
  gw.sendSketchInfo("Temp & Humidity", "1.0");
  gw.wait(5000);

  // Register all sensors to gw (they will be created as child devices)
  Serial.println("Present Node 0");
  gw.present(CHILD_ID_HUM, S_HUM);
  gw.wait(5000);
  
  Serial.println("Present Node 1");
  gw.present(CHILD_ID_TEMP, S_TEMP);
  gw.wait(5000);
  
  Serial.print("Get Config: ");
  metric = gw.getConfig().isMetric;
  Serial.println(metric);
  gw.wait(5000);
  
  
  
  Serial.println("Send Battery Level");
  gw.sendBatteryLevel(80);
  gw.wait(5000);
  
  Serial.println("Request Time");
  gw.requestTime(receiveTime);
  gw.wait(5000);
  
  lastMillis = -SLEEP_TIME;

}




void loop()      
{  
  delay(dht.getMinimumSamplingPeriod());
  
  if((lastMillis+SLEEP_TIME) < millis()) {
  
  
      float temperature = dht.getTemperature();
      Serial.print("Raw temp: ");
      Serial.println(temperature);
      
//      if (dhtInF){
//        temperature = dht.toCelsius(temperature);
//      }

      if (isnan(temperature)) {
          Serial.println("Failed reading temperature from DHT");
      } else if ((temperature != lastTemp) or (1==1)) {
        lastTemp = temperature;
        if (!metric) {
          temperature = dht.toFahrenheit(temperature);
        } 
        
        gw.send(msgTemp.set(temperature, 1));
        gw.wait(500);
        Serial.print("T: ");
        Serial.println(temperature);
      }
      
      float humidity = dht.getHumidity();
      if (isnan(humidity)) {
          Serial.println("Failed reading humidity from DHT");
      } else if ((humidity != lastHum) or (1==1)){
          lastHum = humidity;
          gw.send(msgHum.set(humidity, 1));
          gw.wait(500);
          Serial.print("H: ");
          Serial.println(humidity);
      }
      gw.sendBatteryLevel(80);
      lastMillis = millis();
      gw.wait(500);
  }
  gw.process();
  //gw.sleep(SLEEP_TIME); //sleep a bit
}

// This is called when a new time value was received
void receiveTime(unsigned long controllerTime) {
  // Ok, set incoming time 
  Serial.print("Time value received: ");
  Serial.println(controllerTime);
  //RTC.set(controllerTime); // this sets the RTC to the time from controller - which we do want periodically
  //timeReceived = true;
}

