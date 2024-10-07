## Stemma-QT series:  accel_gyro

![stemma-qt accel_gyro](accelgyro_persp.JPG)

---

### Goal of Project 

*Connect an MPU6050 unit to the `Photon 2` via a [Stemma QT interfacing board](https://www.adafruit.com/product/4515).  Print the values of an accelerometer and gyroscope to the serial port.*

This tutorial will assist you in the acquisition of data from the [Adafruit MPU6050](https://www.adafruit.com/product/3886) sensor board.  The demo code prints data to the serial port via the `Log.info()` method, and demonstrates what sort of variables to initialize, which function calls to use, etc.  You'll note that we don't `delay()` in the `loop()`, but that we rely on the microcontroller to pull data in as fast as it can.  The library that this project uses is called **MPU6050**.  You can include it manually by going into the command palette of VS Code and typing `Particle: Install Library`, then entering in `MPU6050` when prompted.

#### This project demonstrates the following:

1. write firmware code with the Particle.io "Device OS" that takes input, processes it, and passes the processed data to the serial port
1. connect an MPU6050 board to a `Photon 2` via the [Stemma QT interfacing board](https://www.adafruit.com/product/4515)
1. use custom functions within DeviceOS
1. pass in addresses of variables to be set by a function in Wiring/C/C++
1. wire button to our microcontroller
1. utilize an interrupt function to enable a portion of our code to run
1. control the onboard LED (D7)

---

### Documentation: 

The circuit is powered over 3.3V from the microcontroller, which can be supplied via a usb power adapter, a lipo battery, or your computer's usb port.

accel/gyro top image:
---
![accel_gyro top](accelgyro_top.JPG) 
### Steps to recreate circuit:

Preparation:

- Solder the Stemma QT interfacing board, as described [here](https://www.youtube.com/watch?v=DHG7GmUL8wM&ab_channel=loopstick)

Photon connection:

- Plug the `Photon 2` into the Stemma-QT interfacing board
- Make sure that the pins match with respect to orientation

Stemma-QT connection:

- Plug the 4-pin JST cable into one of the 4-pin male ports
- Make sure that the black wire is on the left

Sensor connection:

- Plug the sensor's 4-pin JST connector into the Stemma QT interfacing board.  
- Make sure that the black cable is near the "on" label

Button connections:

- push a 2-leg button into two separate rails of a breadboard
- wire the "top" leg (refer to the image) to the 3.3V pin of the microcontroller
- wire the "bottom" leg to pin D2

---

### Steps to replicate software environment

**Note:** *We assume that you have installed Particle Workbench inside of Visual Studio Code, and are familiar with the process of selecting your device.*  If not, see [this tutorial](https://github.com/Berkeley-MDes/tdf-fa24-equilet/blob/main/_tutorials/installation_compilation/p2_pw_tutorial/README.md).

1. open the folder this README resides within in Visual Studio Code
1. prepare your Particle Workbench environment for flashing as described [here](https://github.com/Berkeley-MDes/24f-desinv-202/wiki/Particle-Workbench#setting-a-project-up).
1. verify/compile
1. flash the code to your photon2 

#### Code (from project):

```
// Include the necessary libraries
#include "Particle.h"
#include "MPU6050.h"
#define OUTPUT_READABLE_ACCELGYRO

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

const pin_t ledPin = D7;
const pin_t buttonPin = D2;

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

    buttonState = false;
  }
  
  // toggle the LED every second
  toggleLed();
  delay(100);  //note that this delay is optional and mainly here to make the serial output easier to read
}
```

---

### To use/test

1. Open the project folder as described [here](https://github.com/Berkeley-MDes/24f-desinv-202/wiki/Particle-Workbench#open-an-existing-project-in-vs-code), and follow the subsequent steps to compile & flash
1. Once the project is on the Photon 2, proceed
1. Move the accelerometer around in the air
1. View values sent to the serial port by opening up a monitor in the Particle Workbench and selecting your port, then clicking on `Start Monitoring`
1. Try pressing the button to see values get mapped and printed in a more readable format to the serial port
