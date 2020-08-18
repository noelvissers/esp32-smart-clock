
# Smart Clock

[![Build Status](https://travis-ci.com/noelvissers/esp32-smart-clock.svg?token=iFxUVHtLTBXHs7qouyTv&branch=master)](https://travis-ci.com/noelvissers/esp32-smart-clock)

The 'Smart Clock' is a clock that displays time, date and temperature. It can auto change display brightness and almost everything is configurable (like 12 hour or 24 hour clock, temperature in °C or °F, date in days-months or months-days format and auto or manual brightness control).  
Hardware files are made in Altium and software is written in C++.

![Smart Clock](/thumbnail.png?raw=true "Smart Clock")

'Smart Clock' is a project I started years ago with an idea for my arduino nano after seeing [this](https://www.instructables.com/id/Making-a-Wooden-LED-Clock/) project. I got the basics to work but wanted to expand on this idea.
A lot more was possible with WiFi integration on the ESP32, so that's what made me pick up this project again. I'm most interested in hardware (that's also what I studied) but during this project I learned a lot about C++ in general, so it was a good learning experience. I know the code can get a lot cleaner if I remove some global vars and structure some things a bit different, but I found a lot of that out while already being pretty far into the project. Maybe something to change in the future, but not for now.

## Table of content
- [Features](#features)
- [Project directories](#project-directories)
- [Manual](#manual)
- [Software](#software)
  * [Language](#language)
  * [IDE](#ide)
  * [Libraries](#libraries)
  * [Documentation](#documentation)
- [Hardware](#hardware)
  * [Components](#components)
  * [Circuits](#circuits)
    + [RTC](#rtc)
    + [LDR](#ldr)
    + [Display](#display)
    + [Buttons](#buttons)
    + [PCB assembly](#pcb-assembly)
- [Case](#case)
- [Wish list](#wish-list)

## Features
I called this project 'Smart Clock' but what does this exactly mean in this project? The clock in 'smart' in a way that it does a lot of things automatically, without the need of any user input. This results to the following features: 
- Displaying time and weather information   
Of course the most important feature of a clock: Displaying the time. Besides displaying time, the Smart Clock can display date, temperature, humidity and time with date in binary (when enabled and always in 24H format).    
![Diffrent modes gif]()   
- Automatic network configuration:   
The clock can automatically connect to a saved WiFi network. When there is no network configured, it sets up its own network to connect to via your PC, tablet or phone. A new WiFi connection can be configured in the portal that shows up after connecting. The settings of the clock can also be configured in this portal as shown below:   
![Configuration portal]()   
- As shown in the portal, the clock has a lot of different settings:
   - Auto brightness: Automatically change the brightness of the matrix display depending on the light level in the room. This can also be manually controlled with the buttons on top of the clock (this will disable auto brightness and can be enabled again by holding the '+' button). Sensitivity can be adjusted in software, and with the potentitometer on the PCB.
   - Auto cycling: Auto cycle between the different screens like time, date and weather information. This can also be manually done by pressing the middle button on the clock. Doing this will disable auto cycling. To enable this again, hold the '-' button.
   - Clock format: Choose between 12 hour or 24 hour clock format.
   - Date format: Choose between Day-Month format or Month-Day format.
   - Temperature unit: Choose between °F or °C. 
   - City/Country: Get accurate weather information from your specified location. Check the available locations and valid format [here](https://openweathermap.org/).
- The clock automatically changes to DST when this is applicable for your location (automatically detected). 
- The RTC is synced with the web. This means if for some reason the RTc gets out of sync, the right time is set based on your [IP/location](http://worldtimeapi.org/api/ip).   

## Project directories
### `/hardware`
This directory includes all the hardware files used in this project. The .pdf files of the Smart Clock schematic and PCB design are located in the main folder. The original Altium Designer files and gerber files are located in the sub folders.
### `/include`
The include directory contains all the header files needed for this project. 
### `/src`
The src directory contains all the `.cpp` files needed to run this project, including the `main.cpp` file.

## Manual
- Powering the Smart Clock   
The clock is USB powered via USB type C. Connect an USB cable to a power source (like a phone charger or USB port) and connect the USB type C connector to the back of the clock like shown below.   
![USB C connector]()   
- Setting up the Smart Clock   
	1. To connect the ESP32 to a network, power the clock and wait for the status icon to appear:   
![Init status]()   
	2. Meaning of status icons:   
![Status meaning]()   (what is what (wifi/time/weather) and what does the icon mean ⚪⦿●)   
	3. When the network status is 'stuck' on this initializing, it means it couldn't connect to a saved network and it needs to be configured. To do this, open the WiFi settings on your phone (or other device) and connect with 'Smart Clock'. After connecting with the ESP, the configuration portal will appear.
	4. Click on X to configure WiFi and settings:   
![Portal login screen]() ![Portal config screen]()   
	5. Select the network you want to connect to and fill in the password.
	6. Check the settings to your preference:   
![Portal setting screen]()   
	7. Fill in your location and API key:   
  - **City**: The name of the city you want to get the weather data from (ex. Amsterdam). 
  - **Country code**: The country the city is in (ex. Netherlands).
	  Valid location data can be found on the [OpenWeatherMap site](https://openweathermap.org/).
  - **API key**: Your OpenWeatherMap API key (see [here](https://openweathermap.org/appid) how to get an API key).   
![Portal weather settings]()   
  - After setting up your network, the clock should be configured and show the current time.   
   
- Buttons   
![Buttons]()   
`- | Min` Brightness level down (this disables auto brightness if enabled). Hold to enable auto cycling.   
`○ | Select` Cycle between time, date, temperature and humidity (this disables auto cycling if enabled). Hold (for 5 seconds) to reset clock (wipes settings and saved networks).   
`+ | Plus`  Brightness level up (this disables auto brightness if enabled). Hold (for 3 seconds) to enable auto brightness.   

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
- Explanation of code   
See comments in source files for the explanation of the code.

## Hardware
The whole project is built on the ESP32. The board I used is the [ESP32-DevKitC V4](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html). This microcontroller is easy to use (uses the Arduino framework) and has built-in bluetooth and wifi connectivity. This controller also has two cores which is very usefull in some situations.
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
- [<value> Potmeter for LDR adjustments](https://aliexpress.com/item/4000109378348.html?spm=a2g0s.9042311.0.0.27424c4dp3I9x0)
- 4x [5mm M3 standoff](https://aliexpress.com/item/32982370726.html?spm=a2g0s.9042311.0.0.27424c4dp3I9x0)
- 4x [4mm M3 screw](https://aliexpress.com/item/4000193782706.html?spm=a2g0o.detail.1000014.3.49a055c56TTvvg&gps-id=pcDetailBottomMoreOtherSeller&scm=1007.13338.128125.0&scm_id=1007.13338.128125.0&scm-url=1007.13338.128125.0&pvid=237d7521-e37e-4988-b3a8-f84ffeda019e)
- \+ Other various components like resistors and capacitors (see schematic).

The electonics for this project are pretty simple. I choose to use TH components instead of SMD components (and in some cases breakout boards) since these are easier to solder and replace by hand. 

### Circuits
Below are the hardware circuits for the clock explained. 
#### RTC
//Sch.
#### LDR
//Sch.
#### Display
//Sch.
#### Buttons
//Sch.

#### PCB assembly
//Print on board where components need to be. R1 R2 C1 U1 etc...   
//Order of assembly   
//Mechanical design

## Case
The case.   
//Pictures   
Mechanical sketch to quickly see what would go where and how:   
<img src="https://i.imgur.com/OlD9MNi.png" alt="MCH sktech" width="500">

Mechanical drawing to figure out actual sizes of the case:   
![MCH drawing]()

//Wood sawing etc...

## Wish list
These are features I don't expect to implement, but that could be nice for a future update.
- [x] Have Wifi configurable and not hard-coded.  
- [ ] Have all the components (SMD) on 1 PCB so the whole thing can be a lot smaller. This also means not using an ESP32 dev. kit (or any USB interface at all).  
- [ ] Have an app or better online portal to configure all the settings, whenever you want (not only on startup).  
- [ ] Auto detect location to configure weather or use location from app.
- [ ] Add audio support to add alarms/mp3 playback via app.   
   
Feel free to submit any issues and/or pull requests if you have new idea's or run into issues. Of course you are also free to implement idea's I mentioned.
