# Smart Clock

[![Build Status](https://travis-ci.com/noelvissers/esp32-smart-clock.svg?token=iFxUVHtLTBXHs7qouyTv&branch=master)](https://travis-ci.com/noelvissers/esp32-smart-clock)

The 'Smart Clock' is a clock that displays time, date and temperature. It can auto change display brightness and almost everything is configurable (like 12 hour or 24 hour clock, temperature in °C or °F, date in days-months or months-days format and auto or manual brightness control).  
Hardware files are made in Altium and software is written in C++.

![Smart Clock](/thumbnail.png?raw=true "Smart Clock")

'Smart Clock' is a project I started years ago with an idea for my arduino nano after seeing [this](https://www.instructables.com/id/Making-a-Wooden-LED-Clock/) project. I got the basics to work but wanted to expand on this idea.
A lot more was possible with WiFi intergration on the ESP32, so thats what made me pick up this project again. 

## Project directories
### `/hardware`
This directory includes all the hardware files used in this project. The .pdf files of the SmartClock schematic and PCB design are located in the main folder. The original Altium Designer files and gerber files are located in the subfolders.
### `/include`
The include directory contains all the header files needed for this project. 
### `/src`
The src directory contains all the `.cpp` needed to run this project, including the `main.cpp` file.

## Software
### Language
This project is written in C++ on the Arduino framework using PlatformIO.
### IDE
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO for VSCode](https://platformio.org/install/ide?install=vscode)
- [Tutorial for using PlatformIO with ESP32](https://docs.platformio.org/en/latest/tutorials/espressif32/arduino_debugging_unit_testing.html)
### Libraries
- Libraries are automatically downloaded when building the project. They are defined in `platformio.ini`.
### Documentation 
- //how to connect via portal etc
- //valid inputs for portal
- //how to setup weather acc / key
- //overall functionality (what do buttons do etc)
- See comments in source files for the explanation of the code.

## Hardware
The whole project is built on the ESP32. The board I used is the [ESP32-DevKitC V4](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html). This microcontroller is easy to use (uses the Arduino framework) and has built-in bluetooth and wifi connectivity. This controller also has two cores which is very usefull in some situations.
### Components
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
- [<value> Potmeter for LDR adjustments](https://aliexpress.com/item/4000109378348.html?spm=a2g0s.9042311.0.0.27424c4dp3I9x0)
- 4x [5mm M3 standoff](https://aliexpress.com/item/32982370726.html?spm=a2g0s.9042311.0.0.27424c4dp3I9x0)
- 4x [4mm M3 screw](https://aliexpress.com/item/4000193782706.html?spm=a2g0o.detail.1000014.3.49a055c56TTvvg&gps-id=pcDetailBottomMoreOtherSeller&scm=1007.13338.128125.0&scm_id=1007.13338.128125.0&scm-url=1007.13338.128125.0&pvid=237d7521-e37e-4988-b3a8-f84ffeda019e)
- \+ Other various components like resistors and capacitors (see schematic).

The electonics for this project are pretty simple. I choose to use TH components instead of SMD components (and in some cases breakout boards) since these are easier to solder and replace by hand. 

## Circuits
Below are the hardware circuits for the clock explained. 
### Ciruit_1
TODO...

### PCB assembly
TODO... (print on board where components need to be. R1 R2 C1 U1 etc...)

## Case
And ofcourse the most important part: the case.
TODO...

### Wishlist
These are features I don't expect to implement ever, but could be nice if I (or someone else) wants to expand on this project.
- [x] Have Wifi configurable and not hardcoded.  
- [ ] Have all the components (SMD) on 1 PCB so the whole thing can be a lot smaller. This also means not using an ESP32 dev kit.  
- [ ] Have an app or better online portal to configure all the settings, whenever you want (not only on startup).  
- [ ] Auto detect location to configure weather.  
