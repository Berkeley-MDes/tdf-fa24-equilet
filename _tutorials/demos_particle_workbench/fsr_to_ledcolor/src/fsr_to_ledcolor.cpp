/*
 * Project fsr_to_ledcolor
 * Description: Change the color of an RGB LED based on the pressure applied to a FSR
 * Author: Jeffrey Lubow
 * Date: 2024-10-13
 */

// Include Particle Device OS APIs
#include "Particle.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

void setTarget(int red, int green, int blue);
void setColor(int R, int G, int B);

const int REDPIN = D13;
const int GREENPIN = D14;
const int BLUEPIN = D15;
const int FSRPIN = A1;
const int deltime = 2;

int rValue = 0;
int gValue = 0;
int bValue = 0;
 
void setup() {
  //digital output
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  //analog input
  pinMode(FSRPIN, INPUT); 
}
 
void loop() {
  int fsr = analogRead(FSRPIN);
  
  switch(fsr){
    case 10 ... 682:
      setTarget(255, 0, 255); // Magenta
      break;
    case 683 ... 1365:
      setTarget(0, 255, 0);   // Green
      break;
    case 1366 ... 2048:
      setTarget(0, 0, 255);   // Blue
      break;
    case 2049 ... 2731:
      setTarget(255, 255, 0); // Yellow
      break;
    case 2732 ... 3414:
      setTarget(255, 0, 0);   // Red
      break;
    case 3415 ... 4095:
      setTarget(0, 255, 255); // Aqua
      break;

    default:
      setTarget(0, 0, 0); // dimmed
      break;
  }
}
 
void setTarget(int red, int green, int blue) {
  while ( rValue != red || gValue != green || bValue != blue ) {
    if ( rValue < red ) {rValue += 1;}
    if ( rValue > red ) {rValue -= 1;}
 
    if ( gValue < green ) {gValue += 1;}
    if ( gValue > green ) {gValue -= 1;}
 
    if ( bValue < blue ) {bValue += 1;}
    if ( bValue > blue ) {bValue -= 1;}
 
    setColor(rValue, gValue, bValue);
    delay(deltime);
  }
}
 
void setColor(int R, int G, int B) {
  analogWrite(REDPIN, R);
  analogWrite(GREENPIN, G);
  analogWrite(BLUEPIN, B);
}