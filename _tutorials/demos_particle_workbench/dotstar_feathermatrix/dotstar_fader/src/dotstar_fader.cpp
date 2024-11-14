/*
 * Project DotStar Featherwing Matrix demo
 * Description: demonstrates the use of the Adafruit DotStar Featherwing Matrix
 * Author: Jeffrey Lubow 
 * Date: 2024-10-15
 */

#include "Particle.h"
#include "Adafruit_DotStar.h"
#include "Adafruit_DotStarMatrix.h"
#include "Adafruit_GFX.h"
#include "TomThumb.h"

#define SHIFTDELAY 2
#define BRIGHTNESS 20 

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

#define SPI_INTERFACE SPI1

Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(12, 6, SPI_INTERFACE, DS_MATRIX_BOTTOM + DS_MATRIX_LEFT + DS_MATRIX_ROWS + DS_MATRIX_PROGRESSIVE, DOTSTAR_BGR);

#define DATAPIN D6
#define CLOCKPIN D8

//void calcWave();
//void renderWave();
void fade_led(int x, int y);

int red = 6; 
int green = 5;
int blue = 3;
int dot_x = 0;
int dot_y = 0;

void setup(){

  matrix.begin();
  matrix.show();
  matrix.setBrightness(BRIGHTNESS);
}

void loop(){

  for(int x = 0; x < matrix.width(); x++){
    for(int y = 0; y < matrix.height(); y++){
      dot_x = random(0, matrix.width());
      dot_y = random(0, matrix.height());
      fade_led(dot_x, dot_y);      
    }
  }
  
}

void fade_led(int x, int y){
  for( int i = 0; i <= 255; i++){
    red = i;
    green = 255 - i;
    blue = 0;

    matrix.drawPixel(x, y, matrix.Color(red, green, blue));
    matrix.show();
    delay(SHIFTDELAY);
  }
  for( int i = 0; i <= 255; i++){
    red = 255 - i;
    green = 0;
    blue = i;
    matrix.drawPixel(x, y, matrix.Color(red, green, blue));
    matrix.show();
    delay(SHIFTDELAY);
  }
  for( int i = 0; i <= 255; i++){
    red = 0;
    green = i;
    blue = 255 - i;
   
    matrix.drawPixel(x, y, matrix.Color(red, green, blue));
    matrix.show();
    delay(SHIFTDELAY);
  }

  for( int i = 0; i <= 255; i++){
    red = 0;
    green = 255 - i;
    blue = 0;
    matrix.drawPixel(x, y, matrix.Color(red, green, blue));
    matrix.show();
    delay(SHIFTDELAY);
  }

} 

/*
void setup() {
  matrix.begin();
  matrix.show();
  dx = (TWO_PI / period) * (matrix.width() / 2);
  matrix.setBrightness(50);
}

void loop() {
  matrix.clear();
  calcWave();
  renderWave();
  delay(5);
}

void calcWave() {
  theta += 0.02;
  float x = theta;
  for (int i = 0; i < matrix.width(); i++) {
    yvalues[i] = sin(x) * amplitude;
    x += dx;
  }
}

void renderWave() {
    for (int col = 0; col < matrix.width(); col++) {
      int index = matrix.width() * col + theta;
      int brightness = abs(yvalues[col]) * 255 / amplitude;
      int color = matrix.Color(col * 255 / matrix.width(), 255, brightness);
      matrix.setPixelColor(index, color);
  }
  matrix.show();
}
*/