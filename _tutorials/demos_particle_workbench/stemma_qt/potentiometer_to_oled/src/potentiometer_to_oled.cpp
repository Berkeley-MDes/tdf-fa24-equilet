/*
 * Project potentiometer_to_oled
 * Description: read values from a potentiometer and display them on an OLED screen
 * Author: Jeffrey Lubow
 * Date: 2024-10-13
 */

#include "Particle.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "splash.h" //this is our custom header containing the splash screen bitmap

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3D // OLED display address (for the 128x64)
#define POT_PIN A0 // Potentiometer is connected to A0

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);
// Instantiate SSD1306 driver display object via I2C interface; note that no reset is used
Adafruit_SSD1306 disp(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void draw_splash(void); //our splash screen function
float draw_potval(float val); //our potentiometer value display function
void draw_bitmap(const unsigned char* bitmap, int x, int y, int w, int h, int color);
int potval = 0;

void setup() {
  
  delay(8); 
  // if initialization fails print failure to Serial, and enter an infinite loop
  bool test_access = disp.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  if(!test_access){
    Log.info("SSD1306 allocation failed");
    return;
  }else{
    Log.info("SSD1306 allocation success");
    draw_splash();
    delay(2000);
  }
}

void loop() {
  int potval = map(analogRead(POT_PIN), 4095, 0, 0, 1000);
  float potval_float = (float)potval / 1000.0;
  Log.info("potval: %f", potval_float);
  draw_potval(potval_float);

  delay(100); // Pause for a moment
}

void draw_splash(void) {
  disp.clearDisplay();
  disp.drawBitmap(0, 0, epd_pirate_small, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  disp.display();
}

float draw_potval(float val) {

  float radians = val * 2 * PI;
  float circle_radius1 = 64.0 * val;
  float circle_radius2 = circle_radius1 * 0.4;
  float circle_radius3 = circle_radius1 * 0.125;;

  int16_t x = 64;
  int16_t y = 32;
  int16_t r_outer = int16_t(circle_radius1);
  int16_t r_inner1 = int16_t(circle_radius2);
  int16_t r_inner2 = int16_t(circle_radius3);

  disp.clearDisplay();

  disp.drawCircle(x, y, r_outer, WHITE);
  disp.drawCircle(x, y, r_inner1, WHITE);
  disp.drawCircle(x, y, r_inner2, WHITE);


  //calculate a line from the center of the cirlce to the edge
  //make the line rotate with the potentiometer
  int16_t x2 = x + cos(radians) * r_outer;
  int16_t y2 = y + sin(radians) * r_outer;
  disp.drawLine(x, y, x2, y2, WHITE);
  disp.fillRect(0, 0, SCREEN_WIDTH, 18, BLACK); 

  disp.setTextSize(1);
  disp.setTextColor(WHITE);
  disp.setCursor(0,0);

  disp.println("knob value: " + String(val, 3));
  disp.println("angle deg: " + String(radians * (180.0 / PI), 3));
  disp.display();
  return val;
}

void draw_bitmap(const unsigned char* bitmap, int x, int y, int w, int h, int color) {
  int byte_width = (w + 7) / 8;
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++ ) {
      if (pgm_read_byte(bitmap + j * byte_width + i / 8) & (128 >> (i & 7))) {
        disp.drawPixel(x + i, y + j, color);
      }
    }
  }
}

