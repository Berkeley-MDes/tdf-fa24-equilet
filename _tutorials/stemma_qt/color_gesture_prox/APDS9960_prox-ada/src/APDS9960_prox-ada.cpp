/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

/***************************************************************************
  This sketch puts the sensor in proximity mode and enables the interrupt
  to fire when proximity goes over a set value
 ***************************************************************************/

// Include Particle Device OS library
#include "Particle.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);
#include "Adafruit_APDS9960_Particle.h"

//the pin that the interrupt is attached to
pin_t interrupt_pin = D2;

//create the APDS9960 object
Adafruit_APDS9960 apds;

void setup() {
  Serial.begin(115200);
  pinMode(interrupt_pin, INPUT_PULLUP);

  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");

  //enable proximity mode
  apds.enableProximity(true);
  //set the interrupt threshold to fire when proximity reading goes above 175
  apds.setProximityInterruptThreshold(0, 175);
  //enable the proximity interrupt
  apds.enableProximityInterrupt();
}

void loop() {

  //print the proximity reading when the interrupt pin goes low
  if(!digitalRead(interrupt_pin)){
    Serial.println(apds.readProximity());

    //clear the interrupt
    apds.clearInterrupt();
  }
}
