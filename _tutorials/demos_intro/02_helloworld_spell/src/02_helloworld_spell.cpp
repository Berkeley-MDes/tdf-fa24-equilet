/* 
 * Project 02_helloworld_spell
 * Author: Jeffrey Lubow
 * Date: 2024-09-26
 */

#include "Particle.h"

// Define the system setup and log handler
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

//make hello world message
//https://docs.arduino.cc/learn/programming/memory-guide/ 
char hello[] = "Hello World!";
//get the size of the message; note the null terminator '\0'
//note also that we account for this by subtracting 1 from the size
int size_hello = sizeof(hello) - 1;
int count = 0;

// setup() runs once, when the device is first turned on
void setup() {
  //note that you may not see this, as it happens very quickly 
  //...typically before you are able to connect to the serial port
  //delay(20000);   
  Log.info("-------------Welcome to the Hello World example!-------------");
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

  // if we have reached the end of the message, reset the count
  if (count == size_hello) {
    count = 0;
  } 
  
  //log the current character
  Log.info("current character: %c", hello[count]);
  count++;

  // incur a delay of 3 seconds
  delay(300);
}
