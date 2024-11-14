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

struct VoronoiSite {
  int x;
  int y;
  uint16_t color; // RGB565 color
};

const int numSites = 4; // Number of Voronoi sites (adjust for performance)
VoronoiSite sites[numSites];
uint32_t timeCounter = 0;

// Function prototypes
void generate();
void generateVoronoiSites();
uint16_t voronoiColor(int x, int y);
uint16_t blendColors(uint16_t c1, uint16_t c2, float alpha);
uint16_t generateRandomColor();


uint16_t generateRandomColor() {
  // Generate a random RGB565 color
  return (random(32) << 11) | (random(64) << 5) | random(32); // RGB565 format
}

void generateVoronoiSites() {
  // Randomly place Voronoi sites
  for (int i = 0; i < numSites; i++) {
    sites[i].x = random(WIDTH);
    sites[i].y = random(HEIGHT);
    sites[i].color = generateRandomColor();
  }
}

uint16_t voronoiColor(int x, int y) {
  // Determine the color of the nearest Voronoi site
  int closestIndex = 0;
  float minDistance = 10000.0;

  for (int i = 0; i < numSites; i++) {
    int dx = x - sites[i].x;
    int dy = y - sites[i].y;
    float distance = sqrt(dx * dx + dy * dy); // Euclidean distance

    if (distance < minDistance) {
      minDistance = distance;
      closestIndex = i;
    }
  }

  return sites[closestIndex].color;
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
  generate();
}

void loop() {
  // Create a time-based offset 
  timeCounter += 1;
  float alpha = sin(timeCounter * 0.05) * 0.5 + 0.5; // Oscillating blend factor

  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      // Get Voronoi-based color
      uint16_t newColor = voronoiColor(x, y);

      // Blend with a background or previously displayed color for effect
      uint16_t blendedColor = blendColors(matrix.Color(0, 0, 0), newColor, alpha);

      // Extract RGB components for rendering (optional)
      uint8_t r = (blendedColor >> 11) & 0x1F;
      uint8_t g = (blendedColor >> 5) & 0x3F;
      uint8_t b = blendedColor & 0x1F;

      // Map RGB565 to 24-bit RGB for DotStar library
      matrix.drawPixel(x, y, matrix.Color(r << 3, g << 2, b << 3)); // Adjust bit-shift as needed
    }
  }
  
  matrix.show(); // Update the display

  if (timeCounter > 100) {
    Log.info("Regenerating Voronoi sites...");
    generate();
    timeCounter = 0;
  }

  delay(50);     // Adjust speed 
}

void generate(){
  randomSeed(analogRead(0)); // Seed random number generator
  generateVoronoiSites(); // Initialize Voronoi sites
}