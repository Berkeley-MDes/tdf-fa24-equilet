## `Photon 2 / P2` Getting Started tutorial

#### Opening the box

1. Open your **Photon 2** box and take the device out.  

<p align="center">
<img src="photon2_unbox_01.jpeg" width="200" />
<img src="photon2_unbox_02.jpeg" width="200" />
<img src="photon2_unbox_03.jpeg" width="200" />
</p>

1. If you are connecting the photon2 to the breadboard, loosen the anti-static foam from the pins, and press the device into the breadboard to straddle the two sets of rails.

<p align="center">
<img src="photon2_unbox_04.jpeg" width="200" />
</p>

1. Connect the device to your computer via the micro-USB port.

<p align="center">
<img src="photon2_unbox_05.jpeg" width="200" />
<img src="photon2_unbox_06.jpeg" width="200" />
</p>

#### Acquiring your MAC Address & Adding your P2 to UC Campus IoT

We are going to take a series of steps to get you set up with your new _Photon 2_ device.  This includes getting your device on the Berkeley IoT network, setting up a new Particle account that you can use for your work, and configuring your Particle dashboard.  Let's get into it! 

- We need to get your *Photon 2* registered on the UC Berkeley IoT network, including the acquisition of your MAC address and the addition of your device to the IoT network.  A tutorial that covers that process can be found [here](https://github.com/Berkeley-MDes/tdf-fa24-equilet/tree/main/_tutorials/installation_compilation/p2_iot_tutorial).  Please complete all instructions, and come back here when you have safely copied the password for your device!

#### Particle.io Account setup

1. [set up a new Particle.io account](https://particle.io/signup) with your information, and copy your login credentials somewhere safe.

<p align="center">
<img src="particle_signup.png" width="70%" />
</p>

1. [register and set up your *Photon 2* device](https://setup.particle.io/) with the Particle.io platform, associated with your new account (note that the [Chrome browser](https://www.google.com/chrome/) is recommended and sometimes required when using Particle).  Make sure to write down your device name - we'll use it later.

<p align="center">
<img src="particle_setup.png" width="90%" />
</p>

1. Verify that your device is active by navigating to the [Particle Console Device List](https://console.particle.io/devices), and then selecting the device.  You should see your device listed here as being online, and an illuminated circle that is "breathing cyan".

<p align="center">
<img src="particle_consoleverify.png" width="90%" />
</p>


***Note:*** if you just want to restore your device to a known good state without wiping persistent data like Wi-Fi credentials, you can use [this link](https://docs.particle.io/tools/device-restore/device-restore-usb/)

---

#### Installing VS Code 

<div align="center"><img src="visualstudio_code.png"></img></div>

Let's install Microsoft's Visual Studio Code (VS Code) integrated development environment (IDE).  We chose this environment because it...

- is cross platform
- integrates all manner of codebases and programming paradigms into a single environment
- is an industry standard at the moment
- allows you to utilize [Github Copilot](https://github.com/features/copilot), an AI agent for generating code based on textual prompts
- is the recommended method with which to build firmware using Particle.io, using the [Particle Workbench](https://www.particle.io/workbench/) extension

1. Navigate to [the Visual Studio Code download page](https://code.visualstudio.com/Download) and download the software pertaining to your operating system.

1. Open the software application.  You'll notice that there is a sidebar on the left that displays options for selection.  This is the _Extensions_ pane, meant for extending the Visual Studio code IDE.  It looks like this: <div align="left"><img src="extensions_pane.png"></img></div>

1. Click on this icon to expose the Extensions pane.  You'll note that your cursor pops into a field where you can search.

---

#### Installing the Particle Workbench Extension

1. Search for "Particle Workbench".  A number of extensions pop up in the results.  The one we want to install is called "Workbench".  This [Extensions] view might display other Particle extensions, some of which are related to the workbench.  You can ignore them.

1. Click on the **Workbench** extension, and you'll see it pop up in a new tab to the right of the sidebar.  <div align="left"><img src="workbench_installer.png"></img></div>

1. Click on **Install** to install the extension.  This may take some time, depending on your internet connection.  You may also be prompted to install Command Line tools associated with features that the Particle Workbench needs.  Go ahead and say "yes" to these subsequent installations.  They are required for the Workbench to function.

1. Verify that the Particle command line tools have been installed:
    - Click on the Particle Workbench **Extension** 
    - Scroll down until the **CODE** section is visible (it will be on the left)
    - Click on **Launch CLI** (you may get a prompt here to install CLI tools - go for it!)
    - ...wait some time...
    - Type `which particle`
    - You should see a path print, indicating the installation location of the tools.
      - If your command line tools still do not seem to be installed, try the command listed [here](https://docs.particle.io/reference/developer-tools/cli/) in the terminal window.

#### Supplementary: Installing Microsoft's Serial Monitor Extension

The Particle Workbench should install a utility for monitoring the serial port.  In some rare cases, you may find that there is no serial monitor installed.

- To test, try typing `⇧⌃P (Windows, Linux) | ⇧⌘P (macOS)`, and entering "serial".  
- If a serial monitor is present, you'll see it pop up in the results of the command palette - it will read `>Particle: Serial Monitor`
- If not, we'll need to install one.  Microsoft distributes an extension for just that.  See below:

1. Click on the **Extensions** panel again.  It looks like this: <div align="left"><img src="extensions_pane.png"></img></div>

1. Search for "serial monitor".  There will be a Microsoft extension that shows up.  This is the one you want.  It looks like this: 

<div align="center"><img src="serial_monitor_ext.png" width="300"></img></div>

1. Click on **Install** to install the extension. 

<div align="center" ><img src="serial_monitor_installer.png" width="300"></img></div>

---

#### Sign in to Particle Workbench

1. Now that you've installed the Particle Workbench, you will see its icon in the left sidebar.  It looks like this: <div align="left"><img src="pw_extension_icon.png"></img></div>

1. Click on the extension to reveal the main Workbench page. <div align="left"><img src="workbench_main.png"></img></div>

1. Locate the **LOGIN** button, and click it.

1. Choose "Login with Username"

1. Enter your username (frequently your email), then hit `enter`.

1. Enter your password, then hit `enter`.

***Note:*** You can verify that you are logged in by opening a terminal (`⇧⌃P (Windows, Linux) | ⇧⌘P (macOS)`, then type `>Terminal: Relaunch Active Terminal`) and typing `particle whoami`.

---

#### Creating your first Project

Click on `Create a Project`.  This will generate all of the necessary files on disk for you to build a project in VS Code and start using the Workbench system.  You'll note that once you do this, the boiler plate `setup()` and `loop()` functions appear, ready to be filled out.

_Things to make sure of:_

1. At the bottom of the Workbench screen, choose device os 5.8.0 <div align="left" ><img  src="device_os.png"></img></div>

1. In that same area (This tab initially reads `Argon` as of September 2024), choose your device type (make sure it reads `Photon 2 / P2`) <div align="left" ><img src="device_type.png"></img></div>

1. In the area where it says `<select device>`, enter in device name (this is the name of the device that you set up earlier) <div align="left" ><img src="device_name.png"></img></div>

1. _If it is not already_, connect the photon2 to the computer
    - plug the micro-usb cable into the photon's usb port
    - plug the other end of the cable to your computer

1. You will notice that an LED illuminates.  This is a multi-colored LED that relays status when the device is powered.  It  will tell you whether your photon2 has a particular issue, if it is connected to a wifi network, and many other modes.  You can view all the modes [here](https://docs.particle.io/troubleshooting/led/).

***Note:*** The usb cable you use is going to depend on your computer that you connect to the photon2 with.  If you have a Type A port, you'll need a type a to micro cable.  If you have a Type C port, you'll need a type C to micro cable, etc.

#### Compiling and Flashing your first project

1. To make sure that we can compile (and flash) our code, we need to click on the light grey checkbox.  It is located in the top-right menubar of the particle workbench, but is _only_ visible if you are selected inside of a Particle Workbench `.cpp` file. Here is an overview of creating a new project and flashing for the first time:

  - Assuming you have a project loaded, you can click on the explorer icon.  It looks like this: <div align="left" ><img src="explorer.png"></img></div>

  - Once in the explorer, you'll see a number of items including 
      - `.github`
      - `.vscode`
      - `src`
      - `.gitignore`
      - `project.properties`
      - `README.md` 

  ...The one we're interested in is `src`, an abbreviation for _source code_.

1. Click on `src` to expose a file of interest:  `<yourfilename>.cpp`.  

1. Click on the `.cpp` file to display its contents.  
  - You are now in the "coding" area of the Particle Workbench project.
    - This is the main file we'll concern ourselves with when writing firmware code to flash the photon2 with.  
    - Note that there is a special menubar that appears at the top-right of the screen.  
      - This contains the buttons you'll need to compile and flash your code.  
      
<div align="center" ><img src="pw_build_menu.png"></img></div>

1. Click on the small grey checkbox located in this menubar. 

<div align="center" ><img src="pw_build_menu1.jpg"></img></div>

1. If the compilation was successful you will see the following in the Terminal: `*** COMPILED SUCCESSFULLY ***`.  
  - Success!

***Note on compiling:*** compilation can take some time.  Please be patient with the process, which is dependant on a number of factors including whether it is the first time you've compiled this code, if you need to download elements, if you are running Particle Workbench on a slower computer, etc. 

***Note on errors:*** Your compilation may have errors.  If it does, please report them to one of your professors and/or TA so that we can assist you in the process of debugging.  

***Note on conflicts:*** You may have conflicts with other extensions of VS Code, or possibly compiler clutter.  It may be best for you to check out [this standalone tutorial](https://github.com/Berkeley-MDes/24f-desinv-202/wiki/Particle-Workbench) on creating and flashing your first Particle Workbench project.

#### Flashing your code to the Photon 2 

Now that we have compiled our code, we are ready to flash it to our microcontroller.  This is done by clicking a different button in the menubar, which looks like a lightning bolt. 

<div align="center" ><img src="pw_build_menu2.jpg"></img></div>

1. Click the lightning bolt

1. Wait for some time 

1. If the flash is successful, you will see the microcontroller's status LED blinking through various colors and associated patterns.  If the flash was successful, you'll see the following in the Terminal: `*** FLASHED SUCCESSFULLY ***`.  

More success!  You're done!

#### A note on the Command Palette

<img src="command_palette.png" />

We've already used the command palette a couple of times throughout this tutorial.  It allows you to execute commands associated with VS Code and the Particle Workbench from anywhere in the VS Code workspace.  Just to be clear, let's review a couple of notes:

- You focus the command palette with the key combo:  `⇧⌃P (Windows, Linux) | ⇧⌘P (macOS)`.  This key command focuses the cursor to the palette. 
- Type `>Particle: XXXX`, where `XXXX` represents the command in Particle Workbench you would like to execute.  

We don't need this to get started, but keep it in mind as you start to familiarize yourself with the envionment.  It can make the process of finding things quicker.

---
