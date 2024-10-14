/* 
 * Project BME688_proj
 * Author: Jeffrey Lubow
 * Date: 2024-10-10
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);
// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

Adafruit_BME680 bme; // I2C
//Adafruit_BME680 bme(BME_CS); // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

double temperatureInC = 0;
double relativeHumidity = 0;
double pressureHpa = 0;
double gasResistanceKOhms = 0;
double approxAltitudeInM = 0;

void setup() {
  if (!bme.begin()) {
    Particle.publish("Log", "Could not find a valid BME680 sensor, check wiring!");
  } else {
    Particle.publish("Log", "bme.begin() success =)");
    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
  }
}

void loop() {
  if (! bme.performReading()) {
    Log.info("Failed to perform reading :(");
  } else {
    temperatureInC = bme.temperature;
    relativeHumidity = bme.humidity;
    pressureHpa = bme.pressure / 100.0;
    gasResistanceKOhms = bme.gas_resistance / 1000.0;
    approxAltitudeInM = bme.readAltitude(SEALEVELPRESSURE_HPA);
    Log.info("Temperature: %.2f C, Humidity: %.2f %%, Pressure: %.2f hPa, Gas Resistance: %.2f kOhms, Altitude: %.2f m",
             temperatureInC, relativeHumidity, pressureHpa, gasResistanceKOhms, approxAltitudeInM);
  }
  delay(2000);
}
