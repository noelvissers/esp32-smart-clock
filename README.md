
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
- [Manual](#manual)
- [Software](#software)
  * [Language](#language)
  * [IDE and framework](#ide-and-framework)
  * [Libraries](#libraries)
  * [Documentation](#documentation)
- [Hardware](#hardware)
  * [Components](#components)
  * [Circuits](#circuits)
    + [RTC](#rtc)
    + [LDR](#ldr)
    + [Display](#display)
    + [Buttons](#buttons)
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
![Differnt modes gif]()   
- Automatic network configuration:   
The clock can automatically connect to a saved wifi network. When there is no network configured, it sets up its own network to connect to via your PC, tablet or phone. A new wifi connection can be configured in the portal that shows up after connecting. The settings of the clock can also be configured in this portal as shown below:   
![Configuration portal]()   
- As shown in the portal, the clock has a lot of different settings:
   - **Auto brightness**: Automatically change the brightness of the matrix display depending on the light level in the room. This can also be manually controlled with the buttons on top of the clock (this will disable auto brightness and can be enabled again by holding the '+' button). Sensitivity can be adjusted in software, and with the potentiometer on the PCB.
   - **Auto cycling**: Auto cycle between the different screens like time, date and weather information. This can also be manually done by pressing the middle button on the clock. Doing this will disable auto cycling. To enable this again, hold the '-' button.
   - **Clock format**: Choose between 12-hour or 24-hour clock format.
   - **Date format**: Choose between Day-Month format or Month-Day format.
   - **Temperature unit**: Choose between °F or °C. 
   - **City/Country**: Get accurate weather information from your specified location. Check the available locations and valid format [here](https://openweathermap.org/).
- The clock automatically changes to DST when this is applicable for your location (automatically detected). 
- The RTC is synced with the web. This means if for some reason the RTC gets out of sync, the right time is set based on your [IP/location](http://worldtimeapi.org/api/ip).   

## Manual
- Powering the Smart Clock   
The clock is USB powered via USB type C. Connect an USB cable to a power source (like a phone charger or USB port) and connect the USB type C connector to the back of the clock like shown below.   
![USB C connector]()   
- Setting up the Smart Clock   
	1. To connect the ESP32 to a network, power the clock and wait for the status icon to appear:   
![Init status]()   
	2. Meaning of status icons:   
![Status meaning]()   (what is what (wifi/time/weather) and what does the icon mean ⚪⦿●)   
	3. When the network status is 'stuck' on this initializing, it means it couldn't connect to a saved network and it needs to be configured. To do this, open the wifi settings on your phone (or other device) and connect with 'Smart Clock'. After connecting with the ESP, the configuration portal will appear.
	4. Click on X to configure wifi and settings:   
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

### Circuits
Below are the hardware circuits for the clock explained. If you have any questions about the circuits, or anything else, you can make an [issue](https://github.com/noelvissers/esp32-smart-clock/issues) and I'll try to answer it as soon as I can.   

#### RTC
To keep track of the time, even when the clock is powered off, we need a RTC. The DS3231 RTC used in this project communicates via I2C. This means we need 4 pins connected to the ESP: SDA, SCL and GND/VCC. The DS3231 has a [typical input voltage of 3.3V](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf) which is perfect since the ESP runs on 3.3V. This also means there is no level shifter needed for the data and clock lines. Since the ESP32 has dedicated I2C pins, we can use these to communicate with the RTC. These are pin IO21 (SDA) and IO21 (SCL).   
//Sch.

#### LDR
This is a simple voltage divider. The resistance of the LDR changes depending on the light level; more light means less resistance. This means the voltage to the analog input of the ESP changes; a higher voltage level with more light, a lower voltage level with less light. The sensitivity can be set with the potentiometer. Since this is an analog output, we need to make sure it is connected to an analog input on the ESP so we can use the ADC from the ESP to read this value.    
//Sch.

#### Display
The dot matrix displays are powered via the [MAX7219 display driver](https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf) on 5V. Since the ESP developer board is powered by USB (5V), we can use the input voltage of the board to power the MAX7219. The other parts of ESP run on 3.3V, so we need a level shifter for the 3 data lines: D<sub>IN</sub>, CS and CLK. The CS and CLK lines go to both display 1 and 2, while the data line (D<sub>IN</sub>) only goes to display 1. The MAX7219 has a data out (D<sub>OUT</sub>) pin that can be connected to the data in pin of display 1. This way we can daisy chain the displays together.   
The I<sub>SET</sub> resistor sets the maximal current for the display. This means you will get less brightness with a higher resistor value. I found that the typical circuit that uses around 10kOhm is way too bright, especially at night. You might want to experiment with this a bit and see what value you prefer. In [this video](https://www.youtube.com/watch?v=MT4PSEJzVU0) you can see the effect of different resistor values, although a camera always tries to compensate for this light level. This video does however give a good indication of the minimum brightness. I used a 100k potentiometer in series with the recommended 10k resistor, so I can tweak it after assembly, where 10k is too bright and 100k (or 110k in this case) is probably too dark. So somewhere in the middle should be the sweat spot.   
//Sch.

#### Buttons
For the buttons I used a simple debounce circuit. In testing I found out that this doesn't filter out all the bouncing, so this is also handled in software. This could be because of bad buttons or wrong resistor/capacitor values, but the solution I found in software with interrupts works very smooth right now. Having a pull-up resistor to 3.3V means the button is active LOW.   
//Sch.

### PCB
//Design (connectors etc)...
//Print on board where components need to be. R1 R2 C1 U1 etc...   
//Order of assembly   

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
- [x] Have wifi configurable and not hard-coded.  
- [ ] Have all the components (SMD) on 1 PCB so the whole thing can be a lot smaller. This also means not using an ESP32 dev. kit (or any USB interface at all).  
- [ ] Have an app or better online portal to configure all the settings, whenever you want (not only on startup).  
- [ ] Auto detect location to configure weather or use location from app.
- [ ] Add audio support to add alarms/mp3 playback via app.   
- [ ] Clean up the code a lot   
   
Feel free to submit any issues and/or pull requests if you have new idea's or run into issues. Of course, you are also free to implement idea's I mentioned.
