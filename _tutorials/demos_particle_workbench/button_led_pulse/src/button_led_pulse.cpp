/*
 * Project button_led_pulse 2024
 * Description: use a button to change the pulse rate of an LED
 * Author: Jeffrey Lubow
 * Date: 10/13/24
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

//function prototypes
void buttonPressed();

// Define the pins
const int ledPin = D5; // the pin that the LED is attached to
const int buttonPin = D6; // this pin can be used as an interrupt pin
int pulserate = 900; // variable to store the read value
bool buttonval = false;

// setup() runs once, when the device is first turned on.
void setup() {
  interrupts(); //enable interrupts
  pinMode(ledPin, OUTPUT); // initialize the LED pin as an output:
  pinMode(buttonPin, INPUT_PULLDOWN); // initialize the pushbutton pin as an input:
  attachInterrupt(buttonPin, buttonPressed, RISING); //attach an interrupt to the button pin, based on the rising edge
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  digitalWrite(ledPin, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(pulserate);  // delay for the pulse value (ms)
  digitalWrite(ledPin, LOW); // turn the LED off by making the voltage LOW
  delay(pulserate); // delay for the pulse value (ms)
}

//this function is called whenever the button is pressed
//this happens separately from the main loop
void buttonPressed() {
    pulserate = pulserate - 25;
    
    if (pulserate < 50) {
      pulserate = 900;
    }
  
}