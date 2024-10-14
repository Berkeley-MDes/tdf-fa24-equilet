/* 
 * Project accel_gyro MPU6050_proj
 * Author: Jeffrey Lubow
 * Date: 2024-10-02
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include the necessary libraries
#include "Particle.h"
#include "MPU6050.h"
#define OUTPUT_READABLE_ACCELGYRO

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

const pin_t ledPin = D7;
const pin_t buttonPin = D3;

// MPU variables:
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;

bool ledState = false;
bool buttonState = false;
int counter = 0;

void toggleLed();
void buttonHandler();

// this function just flips the state of the LED
// it makes it easier to see that the device is working
void toggleLed() {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
}

void buttonHandler() {
    buttonState = true;
}

// setup() runs once, when the device is first turned on
void setup() {
  
  interrupts();
  Serial.begin(38400);

  // initialize the LED pin as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLDOWN);

  attachInterrupt(buttonPin, buttonHandler, RISING);

  // initialize device
  Serial.println("Initializing I2C devices...");
  Wire.begin(); // join I2C bus (I2Cdev library doesn't do this automatically)
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  pinMode(ledPin, OUTPUT);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  // accelgyro.getAcceleration(&ax, &ay, &az);

  // display tab-separated accel/gyro x/y/z values
  // keep in mind that this is raw data and not processed
  // to get real-world values, you will need to apply
  // calibration and conversion factors
  // see MPU6050_DMP6 example for a more advanced usage
  // see also https://learn.adafruit.com/adafruit-sensorlab-gyroscope-calibration
  // https://learn.adafruit.com/mpu6050-6-dof-accelerometer-and-gyro/arduino   
  Serial.print("raw a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);

  // if the button was pressed, acquire & print the cooked values
  if(buttonState) {
    Serial.println("Button pressed");

    //map the values to a more legible range
    int vx = map(ax, -17000, 17000, -100, 100);
    int vy = map(ay, -17000, 17000, -100, 100);
    int vz = map(az, -17000, 17000, -100, 100);
    // display tab-separated (and mapped) accel x/y/z values
    Serial.print("cooked a:\t");
    Serial.print(vx); Serial.print("\t");
    Serial.print(vy); Serial.print("\t");
    Serial.println(vz);

    int counter = 0;
    Log.info("count", counter);

    //write a message to the serial port with the counter 
    Serial.printlnf("Counter: %d", counter);

    buttonState = false;
  }
  
  // toggle the LED every second
  toggleLed();
  delay(100);  //note that this delay is optional and mainly here to make the serial output easier to read
}
