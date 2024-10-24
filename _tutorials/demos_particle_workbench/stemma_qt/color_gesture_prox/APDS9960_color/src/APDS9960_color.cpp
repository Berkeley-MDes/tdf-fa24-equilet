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

    Serial.begin(9600);   

    if(!apds.init()){
        Serial.println("failed to initialize APDS  device!");
    } else {
        Serial.println("APDS device initialized!");
    }
    
    Serial.println("---------------------------");
   
    if ( apds.enableLightSensor(false) ) {
        Serial.println(F("Light sensor is now running"));
    } else {
        Serial.println(F("Something went wrong during light sensor init!"));
    }
   
    Serial.println("---------------------------");

    delay(700); //  Wait for initialization and calibration to finish
}

void loop() {

    // read the ambient color values
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
}