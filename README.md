
# Smart Clock

[![Build Status](https://travis-ci.com/noelvissers/esp32-smart-clock.svg?token=iFxUVHtLTBXHs7qouyTv&branch=master)](https://travis-ci.com/noelvissers/esp32-smart-clock)
![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/noelvissers/esp32-smart-clock?include_prereleases)
![GitHub All Releases](https://img.shields.io/github/downloads/noelvissers/esp32-smart-clock/total)

The 'Smart Clock' is a clock that displays time, date and weather information. It can auto change display brightness and almost everything is configurable (like 12-hour or 24-hour clock, temperature in °C or °F, date in days-months or months-days format and automatic or manual brightness control).  
Hardware files are made in Altium and software is written in C++.

![Smart Clock](/thumbnail.png?raw=true "Smart Clock")

'Smart Clock' is a project I started years ago with an idea for my Arduino Nano after seeing [this](https://www.instructables.com/id/Making-a-Wooden-LED-Clock/) project. I got the basics to work but wanted to expand and improve on this idea.
A lot more was possible with wifi integration on the ESP32, so that's what made me pick up this project again. I'm most interested in hardware (that's also what I studied) but during this project I learned a lot about C++ in general, so it was a good learning experience. I know the code can get a lot cleaner if I remove some global vars and structure some things a bit different, but I found a lot of that out while already being pretty far into the project. Maybe something to change in the future, but not for now.

## Table of content
- [Project directories](#project-directories)
- [Features](#features)
- [Software](#software)
  * [Language](#language)
  * [IDE and framework](#ide-and-framework)
  * [Libraries](#libraries)
  * [Documentation](#documentation)
- [Hardware](#hardware)
  * [Components](#components)
  * [PCB](#pcb)
- [Case](#case)
- [Wish list](#wish-list)

## Project directories
### `/hardware`
This directory includes all the hardware files used in this project. The .pdf files of the Smart Clock schematic and PCB design are located in the main folder. The original Altium Designer files and gerber files are located in the sub folders.
### `/include`
The include directory contains all the header files needed for this project. 
### `/src`
The src directory contains all the `.cpp` files needed to run this project, including the `main.cpp` file.

## Features
I called this project 'Smart Clock' but what does this exactly mean for this project? The clock is 'smart' in a way that it does a lot of things automatically, without the need of any user input. This results to the following features: 
- Displaying time and weather information   
Of course, the most important feature of a clock: Displaying the time. Besides displaying time, the Smart Clock can display date, temperature, humidity and time with date in binary (when enabled and always in 24H format).   
   
   ![Different modes gif]()   

- Automatic network configuration:   
The clock can automatically connect to a saved wifi network. When there is no network configured, it sets up its own network to connect to via your PC, tablet or phone. A new wifi connection can be configured in the portal that shows up after connecting. The settings of the clock can also be configured in this portal as shown below:   
   
   ![Configuration portal](/pictures/portal.png?raw=true "Configuration portal")   

- As shown in the portal, the clock has a lot of different settings:
   - **Auto brightness**: Automatically change the brightness of the matrix display depending on the light level in the room. This can also be manually controlled with the buttons on top of the clock (this will disable auto brightness and can be enabled again by holding the '+' button). Sensitivity can be adjusted in software, and with the potentiometer on the PCB.
   - **Auto cycling**: Auto cycle between the different screens like time, date and weather information. This can also be manually done by pressing the middle button on the clock. Doing this will disable auto cycling. To enable this again, hold the '-' button.
   - **Clock format**: Choose between 12-hour or 24-hour clock format.
   - **Date format**: Choose between Day-Month format or Month-Day format.
   - **Temperature unit**: Choose between °F or °C. 
   - **City/Country**: Get accurate weather information from your specified location. Check the available locations and valid format [here](https://openweathermap.org/).
- The RTC is synced with the web. This means if for some reason the RTC gets out of sync, the right time is set based on your [IP/location](http://worldtimeapi.org/api/ip).   

## Software
### Language
This project is written in C++ on the Arduino framework using PlatformIO. 
### IDE and framework
- [Visual Studio Code](https://code.visualstudio.com/) (Free)
- [PlatformIO for VSCode](https://platformio.org/install/ide?install=vscode) (Extension for VSCode. This basically is a fancy Arduino IDE and I recommend it for everyone programming an Arduino or using the Arduino platform since it's way more user and debug friendly)
- [Tutorial for using PlatformIO with ESP32](https://docs.platformio.org/en/latest/tutorials/espressif32/arduino_debugging_unit_testing.html)
### Libraries   
Libraries are automatically downloaded when building the project. They are defined in `platformio.ini`.
- [LedControl](https://github.com/gordoste/LedControl)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [RTClib](https://github.com/adafruit/RTClib)
- [WiFiManager](https://github.com/tzapu/WiFiManager#development) (development branch)
### Documentation
See comments in source files for the explanation of the code. If you have any questions about the code, or anything else, you can make an [issue](https://github.com/noelvissers/esp32-smart-clock/issues) and I'll try to answer it as soon as I can.

## Hardware
The whole project is built on the ESP32. The board I used is the [ESP32-DevKitC V4](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html). This microcontroller is easy to use (uses the Arduino framework) and has built-in bluetooth and wifi connectivity. This controller also has two cores which is very useful in some situations.
### Components   
NEEDS UPDATING!!!   
- [ESP32-WROOM-32D](https://aliexpress.com/item/4000103411061.html?spm=a2g0o.productlist.0.0.65194be4xhn0yb&algo_pvid=acb1591c-bfd1-4233-9fea-c6a11bc02fd2&algo_expid=acb1591c-bfd1-4233-9fea-c6a11bc02fd2-1&btsid=0b0a187915827913900936140e29eb&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
- [Micro USB cable extender](https://aliexpress.com/item/32859863965.html?spm=a2g0s.9042311.0.0.27424c4du3jPcB)
- [USB C (F) to micro USB (M)](https://aliexpress.com/item/33004521025.html?spm=a2g0s.9042311.0.0.27424c4du3jPcB)
- 2x [Matrix display 8x8 + MAX7219](https://aliexpress.com/item/4000263982956.html?spm=a2g0o.productlist.0.0.45acfb9fdBEEGM&algo_pvid=de74b198-9022-41cd-8737-1d07475b5ed4&algo_expid=de74b198-9022-41cd-8737-1d07475b5ed4-0&btsid=0b0a050b15827869426782783e036c&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
- 2x [Narrow 24 pin DIP socket](https://aliexpress.com/item/32866573215.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- 4x [1x8 Matrix header](https://aliexpress.com/item/32847506950.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- [DS3231 RTC](https://aliexpress.com/item/32822420722.html?spm=a2g0o.productlist.0.0.2cbd47d4cJstGj&algo_pvid=58e460dd-718e-4981-b7f9-9ece20311d7b&algo_expid=58e460dd-718e-4981-b7f9-9ece20311d7b-0&btsid=0b0a050b15827871250014067e036c&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
- [3V3 - 5V level shifter](https://aliexpress.com/item/1972789887.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- [2x10 2.54mm 90 degree Male header](https://aliexpress.com/item/32683792015.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- [2x10 2.54mm  Female header](https://aliexpress.com/item/32864030298.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- [5528 LDR](https://aliexpress.com/item/32760631393.html?spm=a2g0o.productlist.0.0.6157a46cjwLxp7&algo_pvid=cd24cd9a-44d2-4029-ac5e-a4303474664d&algo_expid=cd24cd9a-44d2-4029-ac5e-a4303474664d-1&btsid=0b0a22a415827875061507113e83f2&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
- [<value> Potentiometer for LDR adjustments](https://aliexpress.com/item/4000109378348.html?spm=a2g0s.9042311.0.0.27424c4dp3I9x0)
- 4x [5mm M3 standoff](https://aliexpress.com/item/32982370726.html?spm=a2g0s.9042311.0.0.27424c4dp3I9x0)
- 4x [4mm M3 screw](https://aliexpress.com/item/4000193782706.html?spm=a2g0o.detail.1000014.3.49a055c56TTvvg&gps-id=pcDetailBottomMoreOtherSeller&scm=1007.13338.128125.0&scm_id=1007.13338.128125.0&scm-url=1007.13338.128125.0&pvid=237d7521-e37e-4988-b3a8-f84ffeda019e)
- \+ Other various components like resistors and capacitors (see schematic).

The electronics for this project are fairly simple. I choose to use TH components instead of SMD components (and in some cases breakout boards) since these are easier to solder and replace by hand. Since this is meant as a prototype, a later version would include SMD components, no breakout or developer boards and probably JTAG programming. But this is out of my scope for this project.   
   
For the schematic and other documentation, see the [/hardware](/hardware/smart-clock/smart-clock) folder.

### PCB
Below you can see (from left to right) the top, front and side view of the PCB.   
   
![PCB](/pictures/pcb_layout.png?raw=true "PCB")   
   
The gerber files to order the PCB are located in the designated hardware folder.
   
## Case
To see what would go where (on the PCB as well as the mechanical design) I created a [mechanical sketch](/pictures/mch_sketch.png) (yes, in paint).   
   
After this, I could make the PCB and a real mechanical drawing to figure out actual sizes of the case:   
   
![MCH design](/pictures/smart-clock.gif?raw=true "MCH design")   
   
After I was happy with how everything turned out, I rendered the image to get a clear view of what the end result would be like:   
   
![3D render](/pictures/3d_sketch.png?raw=true "3D render")   
   
Note that there is a 8x18 grid of holes in the case, while the display is only 8x16 pixels. This (in my opinion) looks better if a digit is displayed on the edge of the display.
   
After having all the files, the PCB, all hardware components, and the software done, it was time to finish this project (v1). 

### End result
![Result](/pictures/result.png?raw=true "Result")   

## Wish list
These are features I don't expect to implement in this version, but that could be nice for a future update.
- [x] Have wifi configurable and not hard-coded.  
- [ ] Have all the components (SMD) on 1 PCB so the whole thing can be a lot smaller. This also means not using an ESP32 dev. kit (or any USB interface at all).  
- [ ] Have an app or better online portal to configure all the settings, whenever you want (not only on startup).  
- [ ] Auto detect location to configure weather or use location from app.
- [ ] Add audio support to add alarms/mp3 playback via app.   
- [ ] Add RGB display and/or more displays, to be able to implement more features (like LaMetric Time).
- [ ] Clean up the code a lot.   
   
Feel free to submit any issues and/or pull requests if you have new idea's or run into issues. Of course, you are also free to implement idea's I mentioned.
