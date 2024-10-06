/* 
 * Project 06_publishing_info
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
const pin_t button_in = D2;
//define the pins for the LEDs
const pin_t led_out = D7;
const pin_t led_out2 = D3;

int size_hello = sizeof(hello) - 1;
int count = 0;
int count_cycles = 0;
int periodicity = 2000;
bool button_pressed = false;

void button_press(void); // function prototype for the button press function

// setup() runs once, when the device is first turned on
void setup() {
  interrupts();
  pinMode(button_in, INPUT_PULLDOWN);
  pinMode(led_out, OUTPUT);
  digitalWrite(led_out, LOW);
  pinMode(led_out2, OUTPUT);
  digitalWrite(led_out2, LOW);
  attachInterrupt(button_in, button_press, RISING);
}

// function to be called when the button is pressed
// we do not want to do too much in the interrupt handler
void button_press(void) {
  button_pressed = true;
}

void change_period(void) {
  periodicity = random(1000, 2000);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

  digitalWrite(led_out, HIGH);
  digitalWrite(led_out2, HIGH);
  delay(100);
  digitalWrite(led_out, LOW);
  digitalWrite(led_out2, LOW);
  // if the button is pressed, print the message
  if (button_pressed) {
    
    Log.info("Button pressed!-------------------");
    button_pressed = false;
    change_period();
  }

  // if we have reached the end of the message, reset the count & publish the number of cycles
  if (count == size_hello) {
    count = 0;
    count_cycles++;
    Log.info("count_cycles: %d", count_cycles);

    //convert count_cycles to a char array
    char count_cycles_str[10];
    snprintf(count_cycles_str, sizeof(count_cycles_str), "%d", count_cycles);    
    Log.info("Publishing message to the cloud");
    Particle.publish("cycle", count_cycles_str, PRIVATE);
  } 
  
  //log the current character
  Log.info("current character: %c", hello[count]);
  Log.info("about to wait for %d ms", periodicity);
  count++;
  //wait for the amount of time specified by the periodicity variable
  delay(periodicity);
}
