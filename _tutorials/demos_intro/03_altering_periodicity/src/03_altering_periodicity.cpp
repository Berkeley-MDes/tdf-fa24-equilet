/* 
 * Project 03_altering_periodicity
 * Author: Jeffrey Lubow
 * Date: 2024-09-26
 */

#include "Particle.h"

// Define the system setup and log handler
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

//make hello world message
char hello[] = "Hello World!";
//define the pin for the button
pin_t button_in = D2;

int size_hello = sizeof(hello) - 1;
int count = 0;
int periodicity = 3000;
bool button_pressed = false;

// function prototypes
void button_press(void); // function prototype for the button press function
void change_period(void); 

// setup() runs once, when the device is first turned on
void setup() {
  // enable interrupts
  interrupts();
  // Initialize the button pin as an input
  pinMode(button_in, INPUT_PULLDOWN);
  // Attach an interrupt to the button pin
  attachInterrupt(button_in, button_press, RISING);
}

// function to be called when the button is pressed
// we do not want to do too much in the interrupt handler
void button_press(void) {
  button_pressed = true;
}

void change_period(void) {
  periodicity = random(300, 1000);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

  // if the button is pressed, print the message
  if (button_pressed) {
    
    Log.info("Button pressed!-------------------");
    button_pressed = false;
    change_period();
  }
  // if we have reached the end of the message, reset the count
  if (count == size_hello) {
    count = 0;
  } 
  
  //log the current character
  Log.info("current character: %c", hello[count]);
  Log.info("about to wait for %d ms", periodicity);
  count++;
  //wait for the amount of time specified by the periodicity variable
  delay(periodicity);
}