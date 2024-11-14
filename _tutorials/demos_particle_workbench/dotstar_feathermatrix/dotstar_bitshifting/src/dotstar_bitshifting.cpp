/*
 * Project DotStar Featherwing Matrix demo
 * Description: demonstrates the use of the Adafruit DotStar Featherwing Matrix
 * Author: Jeffrey Lubow 
 * Date: 2024-10-15
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_DotStarMatrix.h"
#include "Adafruit_GFX.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

#define DATAPIN D6
#define CLOCKPIN D8
#define SPI_INTERFACE SPI1
#define WIDTH 12
#define HEIGHT 6

SerialLogHandler logHandler(LOG_LEVEL_INFO);
Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(WIDTH, HEIGHT, SPI_INTERFACE, DS_MATRIX_BOTTOM + DS_MATRIX_LEFT + DS_MATRIX_ROWS + DS_MATRIX_PROGRESSIVE, DOTSTAR_BGR);

uint16_t noiseField[WIDTH][HEIGHT]; // Noise field to store RGB565 values
uint32_t timeCounter = 0;

//function prototypes
uint16_t generateNoise();
uint16_t blendColors(uint16_t c1, uint16_t c2, float alpha);

uint16_t generateNoise() {
  // Example function to generate simple noise
  return (random(32) << 11) | (random(64) << 5) | random(32); // RGB565 format
}

uint16_t blendColors(uint16_t c1, uint16_t c2, float alpha) {
  // Blend between two RGB565 colors with a weight factor alpha (0-1)
  uint16_t r1 = (c1 >> 11) & 0x1F;
  uint16_t g1 = (c1 >> 5) & 0x3F;
  uint16_t b1 = c1 & 0x1F;
  
  uint16_t r2 = (c2 >> 11) & 0x1F;
  uint16_t g2 = (c2 >> 5) & 0x3F;
  uint16_t b2 = c2 & 0x1F;
  
  uint16_t rBlend = r1 + alpha * (r2 - r1);
  uint16_t gBlend = g1 + alpha * (g2 - g1);
  uint16_t bBlend = b1 + alpha * (b2 - b1);
  
  return (rBlend << 11) | (gBlend << 5) | bBlend;
}

void setup() {
  matrix.begin(); // Start DotStar matrix
  matrix.setBrightness(40); // Adjust brightness as needed
  randomSeed(analogRead(0)); // Seed random noise generation

  // Initialize noise field
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      noiseField[x][y] = generateNoise();
    }
  }
}

void loop() {
  // Create a time-based offset for swarming effect
  timeCounter += 1;
  float alpha = sin(timeCounter * 0.05) * 0.5 + 0.5; // Oscillating blend factor

  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      // Generate new noise and blend with existing color
      uint16_t newColor = generateNoise();
      noiseField[x][y] = blendColors(noiseField[x][y], newColor, alpha);

      // Extract RGB components for rendering
      uint16_t color = noiseField[x][y];
      uint8_t r = (color >> 11) & 0x1F;
      uint8_t g = (color >> 5) & 0x3F;
      uint8_t b = color & 0x1F;

      // Map RGB565 to 24-bit RGB for DotStar library
      matrix.drawPixel(x, y, matrix.Color(r << 3, g << 2, b << 3)); // Adjust bit-shift as needed
    }
  }
  matrix.show(); // Update the display
  delay(50);     // Adjust speed of fading/swarming
}
