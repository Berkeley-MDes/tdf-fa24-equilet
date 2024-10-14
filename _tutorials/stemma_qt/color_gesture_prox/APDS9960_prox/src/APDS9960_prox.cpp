/* 
 * Project 01_helloworld
 * Author: Jeffrey Lubow
 * Date: 2024-09-26
 */

#include "Particle.h"
#include "SparkFun_APDS9960.h"

// Define the system setup and log handler
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SparkFun_APDS9960 apds;

void setup() {

  //interrupts();

  Serial.begin(9600);   

  if(!apds.init()){
    Serial.println("failed to initialize APDS  device!");
  }
  else{
    Serial.println("APDS device initialized!");
  }
Serial.println("---------------------------");
    // Start running the APDS-9960 gesture sensor engine
    if ( apds.enableGestureSensor(true) ) {
      
        Serial.println(F("Gesture sensor is now running"));
    } else {
        Serial.println(F("Something went wrong during gesture sensor init!"));
    }
    if ( apds.enableLightSensor(false) ) {
        Serial.println(F("Light sensor is now running"));
    } else {
        Serial.println(F("Something went wrong during light sensor init!"));
    }
    if ( apds.enableProximitySensor(false) ) {
        Serial.println(F("Proximity sensor is now running"));
    } else {
        Serial.println(F("Something went wrong during proximity sensor init!"));
    }
 // Adjust the Proximity sensor gain
    if ( !apds.setProximityGain(PGAIN_2X) ) {
        Serial.println("Something went wrong trying to set PGAIN");
    }
    Serial.println("---------------------------");

    delay(700); //  Wait for initialization and calibration to finish

}

void loop() {

  
  // read the proximity value
  uint8_t prox = 0;
  if(!apds.readProximity(prox)){
   Serial.println("Error reading proximity value");
  }else{
   Serial.printf("proximity: %d\n", (int)prox);
  }

  delay(100);  //we can read the proximity value faster than the other sensors
  
  
  /*
  // uncomment this code to enable ambient light reading
  // read the color values
  uint16_t r, g, b, c;

  // Read the light levels (ambient, red, green, blue)
  if (!apds.readAmbientLight(c) || !apds.readRedLight(r) || !apds.readGreenLight(g) || !apds.readBlueLight(b)) {
    Serial.println("Error reading light values");
  } else {
    Serial.print("Ambient: ");
    Serial.print(c);
    Serial.print(" Red: ");
    Serial.print(r);
    Serial.print(" Green: ");
    Serial.print(g);
    Serial.print(" Blue: ");
    Serial.println(b);
  }
  
  // Wait 1 second before next reading
  delay(1000);
  */  
    
  /*
  // uncomment this code to enable the gesture detection; note that it 
  // is only processed at interrupt so you'll need to alter the code to access it.
  // see the example included in the library for more information or reach out to one of 
  // the members of the TDF teaching team!

  if ( apds.isGestureAvailable() ) {
        // Get the next gesture from the sensor
        if ( apds.readGesture() ) {
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
    */

}