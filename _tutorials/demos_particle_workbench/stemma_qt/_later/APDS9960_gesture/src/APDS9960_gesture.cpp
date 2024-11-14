/* 
 * Project 01_helloworld
 * Author: Jeffrey Lubow
 * Date: 2024-09-26
 */

#include "Particle.h"
#include "SparkFun_APDS9960.h"

//#define APDS9960_INT 2 // Needs to be an interrupt pin\

pin_t APDS9960_pin = D2;

// Define the system setup and log handler
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SparkFun_APDS9960 apds;

bool isr_flag = false;
void interruptRoutine();
void handleGesture();

void setup() {
  interrupts();

  // Set interrupt pin as input
  pinMode(APDS9960_pin, INPUT);
// Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));
  
  // Initialize interrupt service routine
  //attachInterrupt(0, interruptRoutine, FALLING);


  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

Serial.println("---------------------------");
    // Start running the APDS-9960 gesture sensor engine
    if ( apds.enableGestureSensor(true) ) {
      
        Serial.println(F("Gesture sensor is now running"));
    } else {
        Serial.println(F("Something went wrong during gesture sensor init!"));
    }
    Serial.println("---------------------------");

    delay(700); //  Wait for initialization and calibration to finish

}

void loop() {  
   if(isr_flag) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = false;
    attachInterrupt(0, interruptRoutine, FALLING);
  }

  handleGesture();
  delay(1);
}


void interruptRoutine() {
  isr_flag = true;
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {

      Serial.println("gesture" + apds.readGesture());

    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("UP");
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        break;
      case DIR_FAR:
        Serial.println("FAR");
        break;
      default:
        Serial.println("NONE");
    }
  }
}