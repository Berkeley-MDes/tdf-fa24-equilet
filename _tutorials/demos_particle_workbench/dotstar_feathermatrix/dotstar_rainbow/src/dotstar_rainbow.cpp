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
#define BRIGHTNESS 1  // put this up to 20 for a retinal zap (max 255)

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

#define SPI_INTERFACE SPI1

Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(12, 6, SPI_INTERFACE, DS_MATRIX_BOTTOM + DS_MATRIX_LEFT + DS_MATRIX_ROWS + DS_MATRIX_PROGRESSIVE, DOTSTAR_BGR);


const uint16_t primaryColors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};

const uint16_t adaColors[] = {
  matrix.Color(255, 0, 0),   //A red
  matrix.Color(255, 125, 0), //D orange
  matrix.Color(200, 255, 0), //A yellowish
  matrix.Color(0, 255, 0),   //F green
  matrix.Color(0, 255, 225), //R blue
  matrix.Color(150, 0, 255), //U purple
  matrix.Color(255, 0, 220), //I pink
  matrix.Color(255, 65, 0),  //T reddish
  matrix.Color(255, 220, 0)  //! orange/yellow
};




void setup() {
  pinMode(D7, OUTPUT); // sets D7 LOW and keeps BLUE LED off

  matrix.begin();
  matrix.setBrightness(BRIGHTNESS);
  matrix.show();
 
 
}



void loop() {

  for (int i = 0; i < 512; i++) {
    //matrix.rainbow(0xFF0000 + (i * 1000), 1 + i/25, 255, 255, 1);
      matrix.rainbow(0xFF0000 + (i * 1000), 1, 255, 255, 1);
      matrix.show();
      delay(20);
  }

}
