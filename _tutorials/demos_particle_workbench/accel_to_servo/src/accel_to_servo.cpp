// Program to test sending accelerometer data off a Photon in real time
// Uses an Analog Devices ADXL362 SPI accelerometer (the one in the Electron Sensor Kit)

#include "Particle.h"
#include "ADXL362DMA.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// Connect the ADXL362 breakout:
// VIN: 3V3
// GND: GND
// SCL: A3 (SCK)
// SDA: A5 (MOSI)
// SDO: A4 (MISO)
// CS: A2 (SS)
// INT1: no connection
// INT1: no connection
ADXL362DMA accel(SPI, A2);
Servo serv;

const int servoPin = D1;
int pos = 0;
// Global variables
unsigned long lastReport = 0;
const unsigned long lastReportPeriod = 100;

unsigned long temperatureReport = 0;
const unsigned long temperatureReportPeriod = 30000;

void setup() {
    waitFor(Serial.isConnected, 10000);

	accel.softReset();
	while(accel.readStatus() == 0) {
		Log.info("no status yet, waiting for accelerometer");
		delay(1000);
	}

	accel.setMeasureMode(true);
  serv.attach(servoPin);
}

void loop() {
  if (millis() - lastReport >= lastReportPeriod) {
      
    lastReport = millis();
    
    int16_t x, y, z;
    accel.readXYZ(x, y, z);

    Log.info("x: %d, y: %d, z: %d", x, y, z);
    pos = map(x, -1000, 1000, 0, 180);
  }

  if (millis() - temperatureReport >= temperatureReportPeriod) {
      temperatureReport = millis();
      Log.info("temperature %.1f C, %.1f F", accel.readTemperatureC(), accel.readTemperatureF());
      Log.info("last report %lu ms ago", millis() - lastReport);
      Log.info("current time %lu", millis()); 
  }

  //output: set the servo position
  serv.write(pos);

  Log.info("position: %d\n", pos);

  delay(1500);      // wait for 1.5 seconds   
  serv.write(90);   // set servo to mid-point
  delay(1500);      // wait for 1.5 seconds
    
}