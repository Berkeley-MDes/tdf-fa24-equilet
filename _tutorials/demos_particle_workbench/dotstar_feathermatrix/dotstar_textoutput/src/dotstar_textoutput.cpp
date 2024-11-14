/*
 * Project DotStar Featherwing Matrix demo
 * Description: demonstrates the use of the Adafruit DotStar Featherwing Matrix
 * Author: Jeffrey Lubow 
 * Date: 2024-10-15
 */

#include "Particle.h"
#include <Adafruit_GFX.h>
#include <Adafruit_DotStarMatrix.h>
#include <Adafruit_DotStar.h>
#include <TomThumb.h>

#define SHIFTDELAY 100
#define BRIGHTNESS 5  // put this up to 20 for a retinal zap

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

#define SPI_INTERFACE SPI1

Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(12, 6, SPI_INTERFACE, DS_MATRIX_BOTTOM + DS_MATRIX_LEFT + DS_MATRIX_ROWS + DS_MATRIX_PROGRESSIVE, DOTSTAR_BGR);
#define DATAPIN D6
#define CLOCKPIN D8

const uint16_t primaryColors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};

const uint16_t msgColors[] = {
  matrix.Color(255, 0, 0),   //red
  matrix.Color(255, 125, 0), //orange
  matrix.Color(200, 255, 0), //yellowish
  matrix.Color(0, 255, 0),   //green
  matrix.Color(0, 255, 225), //blue
  matrix.Color(150, 0, 255), //purple
  matrix.Color(255, 0, 220), //pink
  matrix.Color(255, 65, 0),  //reddish
  matrix.Color(255, 220, 0)  //orange/yellow
};

char themsg[] = "HELLOoooooO0OOOo0H TDF!!!";

int msgColorLen = sizeof(msgColors) / sizeof(msgColors[0]);
int msgLen = sizeof(themsg) / sizeof(themsg[0]) - 1;

void setup() {

  matrix.begin();
  matrix.setFont(&TomThumb);
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);

  for (byte i = 0; i < 3; i++) {
    matrix.fillScreen(primaryColors[i]);
    matrix.show();
    delay(100);
  }
}

int x = matrix.width();

void loop() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 5);

  for (byte i = 0; i < msgLen; i++) {

    // set the color
    matrix.setTextColor(msgColors[i % msgColorLen]);
    // write the letter
    matrix.print(themsg[i]);
  }

  if (--x < -(4 * msgLen)) {
    x = matrix.width();
  }

  matrix.show();
  delay(SHIFTDELAY);
}
