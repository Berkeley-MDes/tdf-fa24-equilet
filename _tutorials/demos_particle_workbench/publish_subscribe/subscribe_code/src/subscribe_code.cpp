/*
 * Project subscribe_code
 * Description: This project subscribes to an event and prints the data to the serial monitor
 */

// Include Particle Device OS APIs
#include "Particle.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

//function prototype
void value_handler(const char *event, const char *data);
//vars
double floatval = 0.0;

// setup() runs once, when the device is first turned on.
void setup() {
  //https://docs.particle.io/reference/device-os/api/cloud-functions/particle-subscribe/
  Particle.subscribe("random_value", value_handler);
}

//note that we are not doing anything in the loop
//this is because we are using the subscribe function to listen for events, 
//and the handler function to process the data
void loop() {
}

void value_handler(const char *event, const char *data){
  float myvalue = (float)(atof(data));
  //view in serial monitor
  Log.info("received an event -----------------------------");
  Log.info("the raw data (string): %s\n", data);
  Log.info("the converted value: %f\n", myvalue); 
}