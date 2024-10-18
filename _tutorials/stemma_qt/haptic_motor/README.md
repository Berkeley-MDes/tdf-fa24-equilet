## Stemma-QT series: DRV2605L Haptic Motor Driver

![stemma-qt haptic motor](2605_persp.JPG)

---

### Goal of Project 

*Connect an DRV2605L unit to the `Photon 2` via a [Stemma QT interfacing board](https://www.adafruit.com/product/2305).  Cycle through the on-board options of haptic feedback types for evaluation, and utilize a button to disable/enable feedback.*

This tutorial will assist you in the acquisition of data from the [Adafruit DRV2605L](https://www.adafruit.com/product/3886) driver board.  The demo code prints the current vibration pattern to the serial port, as well as initiating the vibration pattern for the motor driver.  This processing results in variable voltage being applied to the motor over time.  The code demonstrates what sort of variables to initialize, which function calls to use, etc.  The library that this project uses is called `Adafruit_DRV2605_Library_Sahagun`.  You can include it manually by going into the command palette of VS Code and typing `Particle: Install Library`, then entering in `Adafruit_DRV2605_Library_Sahagun` when prompted.

#### This project demonstrates the following:

1. write firmware code with the Particle.io "Device OS" that takes input, processes it, and outputs a vibration pattern via a motor
1. connect an DRV2605L board to a `Photon 2` via the [Stemma QT interfacing board](https://www.adafruit.com/product/4515)
1. use imported library functions within DeviceOS
1. wire a button to our microcontroller
1. utilize an interrupt function to enable a portion of our code to run

---

### Documentation: 

The circuit is powered over 3.3V from the microcontroller, which can be supplied via a usb power adapter, a lipo battery, or your computer's usb port.

haptic motor top| button circuit fritzing
--- | ---
![haptic motor top](2605_top.JPG) | ![haptic button](haptic_motor.png) 

### Steps to recreate circuit:

Preparation:

- Solder the Stemma QT interfacing board, as described [here](https://www.youtube.com/watch?v=DHG7GmUL8wM&ab_channel=loopstick)

Photon connection:

- Plug the `Photon 2` into the Stemma-QT interfacing board
- Make sure that the pins match with respect to orientation

Stemma-QT connection:

- Plug the 4-pin JST cable into one of the 4-pin male ports
- Make sure that the black wire is on the left

Motor driver connection:

- Plug the motor driver's 4-pin JST connector into the Stemma QT interfacing board.  
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
// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_DRV2605.h"
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);
// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);
// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

Adafruit_DRV2605 drv;
uint8_t effect = 1;

void button_handler();
int effect_on = true;

const pin_t button_pin = D2;

void setup() {
  
  interrupts();
  pinMode(button_pin, INPUT_PULLDOWN);
  attachInterrupt(button_pin, button_handler, RISING);

  Serial.begin(9600);
  Serial.println("Adafruit DRV2605 Basic test");
  
  if (! drv.begin()) {
    Serial.println("Could not find DRV2605");
    while (1) delay(10);
  }
 
  drv.selectLibrary(1);  
  drv.setMode(DRV2605_MODE_INTTRIG); 
}

void button_handler() {
  effect_on = !effect_on;
}

void loop() {

  if(effect_on) {
    Serial.println("Effect on");

	//full library of effects in project download
    if (effect == 1) {
      Serial.println("11.2 Waveform Library Effects List");
      Serial.println(F("1: Strong Click, 100%"));
    }
    if (effect == 2) {
      Serial.println(F("2: Strong Click, 60%"));
    }
    if (effect == 3) {
      Serial.println(F("3: Strong Click, 30%"));
    }
    if (effect == 4) {
      Serial.println(F("4: Sharp Click, 100%"));
    }
    if (effect == 5) {
      Serial.println(F("5: Sharp Click, 60%"));
    }
    if (effect == 6) {
      Serial.println(F("6: Sharp Click, 30%"));
    }
    if (effect == 7) {
      Serial.println(F("7: Soft Bump, 100%"));
    }
    if (effect == 8) {
      Serial.println(F("8: Soft Bump, 60%"));
    }
    if (effect == 9) {
      Serial.println(F("9: Soft Bump, 30%"));
    }
    if (effect == 10) {
      Serial.println(F("10: Double Click, 100%"));
    }
    
    // set the effect to play
    drv.setWaveform(0, effect);  // play effect 
    drv.setWaveform(1, 0);       // end waveform

    // play the effect!
    drv.go();
    effect++;
    if (effect > 117) effect = 1;

  } else {

    Serial.println("Effect off");

  }
  delay(500);
 
}
```

---

### To use/test

1. Open the project folder as described [here](https://github.com/Berkeley-MDes/24f-desinv-202/wiki/Particle-Workbench#open-an-existing-project-in-vs-code), and follow the subsequent steps to compile & flash
1. Once the project is on the Photon 2, proceed
1. Move the accelerometer around in the air
1. View values sent to the serial port by opening up a monitor in the Particle Workbench and selecting your port, then clicking on `Start Monitoring`
1. Try pressing the button to see values get mapped and printed in a more readable format to the serial port
