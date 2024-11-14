/*
 * Project publish_code
 * Description: A simple project to publish a random value to the Particle Cloud
 */

// Include Particle Device OS APIs
#include "Particle.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

//a global value to publish either via Particle.publish() or Particle.variable()
int random_value = 0;

void setup() {
  //this is an option for "exposing" a cloud variable
  //https://docs.particle.io/reference/device-os/api/cloud-functions/particle-variable/
  //Particle.variable("random_value", random_value);
}

void loop() {
    
    //note: if you are using Particle.variable(), this value can be updated as frequently as you like without a
    //data operation being counted.  It is only when you _access_ the variable online
    //that you are charged.  So another timer elsewhere on the web could access this variable
    //at a given polling interval, for example.
    random_value = random(0, 255);

    //Particle.publish() is another way to publish the value.  Note that each publish call will 
    //incur a data operation and that you only have so many of those in 
    //your free particle account.
    //https://docs.particle.io/reference/device-os/api/cloud-functions/particle-publish/
    Particle.publish("random_value", String(random_value), PRIVATE);

    //The subscriber code can be found in the same directory as this project's parent folder
    //it is called "subscribe_code"

    //Log.info("the published value: %d\n", random_value);
    delay(10000);
    //delay(500);
}