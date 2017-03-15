// Enable debug prints
#define MY_DEBUG
#define MY_BAUD_RATE 115200
// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <SPI.h>
#include <MySensor.h>
#include <NewPing.h>

#define SONAR_A_CHILD_ID 1
#define SONAR_B_CHILD_ID 2
#define MOTION_A_CHILD_ID 3


#define SONAR_A_TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define SONAR_A_ECHO_PIN     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MOTION_A_TRIGGER_PIN  2

#define SONAR_B_TRIGGER_PIN  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define SONAR_B_ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
unsigned long SLEEP_TIME = 200; // Sleep time between reads (in milliseconds)

unsigned long MOTION_SLEEP_TIME = 120000; // Sleep time between reports (in milliseconds)
#define INTERRUPT MOTION_A_CHILD_ID-2 // Usually the interrupt = pin -2 (on uno/nano anyway)

NewPing sonar_A(SONAR_A_TRIGGER_PIN, SONAR_A_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar_B(SONAR_B_TRIGGER_PIN, SONAR_B_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


MyMessage SONAR_A_msg(SONAR_A_CHILD_ID, V_DISTANCE);
int SONAR_A_lastDist;

MyMessage SONAR_B_msg(SONAR_B_CHILD_ID, V_DISTANCE);
int SONAR_B_lastDist;

MyMessage MOTION_A_msg(MOTION_A_CHILD_ID, V_TRIPPED);

boolean metric = true;

void setup()
{
  metric = getConfig().isMetric;
  pinMode(MOTION_A_TRIGGER_PIN, INPUT);      // sets the motion sensor digital pin as input
}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Distance+Motion Sensor", "1.0");

  // Register all sensors to gw (they will be created as child devices)
  present(SONAR_A_CHILD_ID, S_DISTANCE);
  present(SONAR_B_CHILD_ID, S_DISTANCE);
  present(MOTION_A_TRIGGER_PIN, S_MOTION);
}

void loop()
{

  boolean tripped = digitalRead(MOTION_A_TRIGGER_PIN) == HIGH;


  Serial.println(tripped);
  send(MOTION_A_msg.set(tripped ? "1" : "0")); // Send tripped value to gw
  if (tripped)  {

    int SONAR_A_dist = metric ? sonar_A.ping_cm() : sonar_A.ping_in();
    Serial.print("Ping Sonar A: ");
    Serial.print(SONAR_A_dist); // Convert ping time to distance in cm and print result (0 = outside set distance range)
    Serial.println(metric ? " cm" : " in");

    if (SONAR_A_dist != SONAR_A_lastDist) {
      send(SONAR_A_msg.set(SONAR_A_dist));
      SONAR_A_lastDist = SONAR_A_dist;
    }

    wait(1000);

    int SONAR_B_dist = metric ? sonar_B.ping_cm() : sonar_B.ping_in();
    Serial.print("Ping Sonar B: ");
    Serial.print(SONAR_B_dist); // Convert ping time to distance in cm and print result (0 = outside set distance range)
    Serial.println(metric ? " cm" : " in");

    if (SONAR_B_dist != SONAR_B_lastDist) {
      send(SONAR_B_msg.set(SONAR_B_dist));
      SONAR_B_lastDist = SONAR_B_dist;
    }

    sleep(SLEEP_TIME);

  }
  //else{
  // Sleep until interrupt comes in on motion sensor. Send update every two minute.
  sleep(INTERRUPT, CHANGE, MOTION_SLEEP_TIME);
  //}
}
