# Smart Clock
SmarClock is a clock that displays time, date, temperature and weather forecasts. It can auto change display brightness and almost everything is configurable (like 12 hour or 24 hour clock, temperature in °C or °F, date in days-months or months-days format and auto or manual brightness controll). The only input that is needed is a USB-C cable for power.  
Hardware files are made in Altium and software is written in C++.

< Insert picture >

## Story
Smart clock is a project I started years ago with an idea for my arduino nano after seeing [this](https://www.instructables.com/id/Making-a-Wooden-LED-Clock/) project. I got the basics to work but wanted to expand on this idea.
A lot more was possible with WiFi intergration on the ESP32, so thats what made me pick up this project again. It is not perfect, but I am happy with how it improved from the last version. If there could be a big improvement, it would be an app to configure everything, since things like weather location need to be hardcoded at the moment.

## Project directories
### `/hardware`
This directory includes all the hardware files used in this project. The .pdf files of the SmartClock schematic and PCB design are located in the main folder. The original Altium Designer files and gerber files are located in the subfolders.
### `/include`
The include directory contains all the header files needed for this project. 
### `/src`
The src directory contains all the .cpp files and the main (SmartClock.cpp) file.

## Software
### Language
C++
### IDE
- [Visual Studio Code]()
- [PlatformIO for VSCode](https://platformio.org/install/ide?install=vscode)
- [Tutorial for using PlatformIO](https://docs.platformio.org/en/latest/ide/vscode.html#quick-start)
### Libraries
- LedControl ([github](https://github.com/wayoda/LedControl?utm_source=platformio&utm_medium=piohome))
- RTC ([github](https://github.com/Makuna/Rtc?utm_source=platformio&utm_medium=piohome))
- (?) WiFiManager (development branch) ([github](https://github.com/tzapu/WiFiManager/tree/development))

## Hardware
For this project, the ESP32 is used. Pretty much any ESP32 module can be used, as long as it has two cores. 
You can probably rewrite the software to run on one core, but I didnt want to delay the whole program to get data from internet, especially when you're displaying seconds.

### Component list:
- [ESP32-WROOM-32D](https://nl.aliexpress.com/item/4000103411061.html?spm=a2g0o.productlist.0.0.65194be4xhn0yb&algo_pvid=acb1591c-bfd1-4233-9fea-c6a11bc02fd2&algo_expid=acb1591c-bfd1-4233-9fea-c6a11bc02fd2-1&btsid=0b0a187915827913900936140e29eb&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
- [Micro USB cable extender](https://nl.aliexpress.com/item/32859863965.html?spm=a2g0s.9042311.0.0.27424c4du3jPcB)
- [USB C (F) to micro USB (M)](https://nl.aliexpress.com/item/33004521025.html?spm=a2g0s.9042311.0.0.27424c4du3jPcB)
- 2x [Matrix display 8x8 + MAX7219](https://nl.aliexpress.com/item/4000263982956.html?spm=a2g0o.productlist.0.0.45acfb9fdBEEGM&algo_pvid=de74b198-9022-41cd-8737-1d07475b5ed4&algo_expid=de74b198-9022-41cd-8737-1d07475b5ed4-0&btsid=0b0a050b15827869426782783e036c&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
- 2x [Narrow 24 pin DIP socket](https://nl.aliexpress.com/item/32866573215.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- 4x [1x8 Matrix header](https://nl.aliexpress.com/item/32847506950.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- [DS3231 RTC](https://nl.aliexpress.com/item/32822420722.html?spm=a2g0o.productlist.0.0.2cbd47d4cJstGj&algo_pvid=58e460dd-718e-4981-b7f9-9ece20311d7b&algo_expid=58e460dd-718e-4981-b7f9-9ece20311d7b-0&btsid=0b0a050b15827871250014067e036c&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
- 5x [100nF/0.1µF TH capacitors](https://nl.aliexpress.com/item/4000124984189.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS) 
- 2x [10µF TH Electrolytic capacitor](https://nl.aliexpress.com/item/32866006892.html?spm=a2g0s.9042311.0.0.27424c4dp3I9x0)
- [Resistors (value differs)](https://nl.aliexpress.com/item/32323198194.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- [3V3 - 5V level shifter](https://nl.aliexpress.com/item/1972789887.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- [WiFi status LED](https://nl.aliexpress.com/item/32524842632.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- 3x [Push buttons](https://nl.aliexpress.com/item/32811504662.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- [2x10 2.54mm 90 degree Male header](https://nl.aliexpress.com/item/32683792015.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- [2x10 2.54mm  Female header](https://nl.aliexpress.com/item/32864030298.html?spm=a2g0s.9042311.0.0.27424c4dnmoTWS)
- [5528 LDR](https://nl.aliexpress.com/item/32760631393.html?spm=a2g0o.productlist.0.0.6157a46cjwLxp7&algo_pvid=cd24cd9a-44d2-4029-ac5e-a4303474664d&algo_expid=cd24cd9a-44d2-4029-ac5e-a4303474664d-1&btsid=0b0a22a415827875061507113e83f2&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
- [<value> Potmeter for LDR adjustments](https://nl.aliexpress.com/item/4000109378348.html?spm=a2g0s.9042311.0.0.27424c4dp3I9x0)
- 4x [5mm M3 standoff](https://nl.aliexpress.com/item/32982370726.html?spm=a2g0s.9042311.0.0.27424c4dp3I9x0)
- 4x [4mm M3 screw](https://nl.aliexpress.com/item/4000193782706.html?spm=a2g0o.detail.1000014.3.49a055c56TTvvg&gps-id=pcDetailBottomMoreOtherSeller&scm=1007.13338.128125.0&scm_id=1007.13338.128125.0&scm-url=1007.13338.128125.0&pvid=237d7521-e37e-4988-b3a8-f84ffeda019e)
### Tools and materials
These are the things I used for the casing. You can also 3d print one or come up with something different ofcourse.
- Wood (a thin piece (few mm) for the front cover and around 1cm thick for the case)
- Saw/sandpaper/screwdriver/drill/Glue

## Build
### 1. Electronics
The electonics for this project are pretty simple. I choose to use TH components instead of SMD components (and in some cases breakout boards) since these are easier to solder and replace by hand. In a future 'perfect' version This is something I would change; have all the components on 1 PCB so the whole thing can be a lot smaller. This also means not using an ESP32 dev kit.
Assambling everything could be a bit tricky since it uses 2 PCBs connected to each other via a single connector. I try to explain everything in the chapters below.

#### 1.1 Soldering the PCB
##### Buttons (3x)
Buttons  
![Buttons](https://i.imgur.com/USFgHV3.png)
##### LDR
LDR, Potmeter, Resistor.  
(test LDR resistance, change pulldown to 5k (depending on ~50% preferable pot).  
![LDR](https://i.imgur.com/9jR07Vt.png)
##### Display
Resistors (Iset, SS pulldown), caps (100nF, 10µF), level shifter, MAX7219 socket, headers for display, status LED, Header to main board.  
Put MAX7219 in socket, disp on header.  
(change 1uF to 10uF, add pulldown SS).  
![Display](https://i.imgur.com/0byBxXv.png)
![Display2](https://cdn.instructables.com/FBG/6G4X/HJ1XP58W/FBG6G4XHJ1XP58W.LARGE.jpg)  
Place the components. Value is on PCB.
##### RTC
RTC
##### ESP32
Header ESP, header to disp.

#### 1.2 Connecting the two PCBs
Connect the display to the main board.  
(Bottom should go under front)
![Assembly](https://i.imgur.com/iHDc2Or.png)
![Edge](https://i.imgur.com/vOnZQnX.png)

#### 1.3 Testing
Upload the code and test if everything works.

### 2. Case
#### 2.1 Base
These are the base components for the housing.
![Housing drawing](https://i.imgur.com/Doj9Lle.png)
#### 2.2 Buttons

#### 2.3 USB port

#### 2.4 Standoffs 
Drill a 3mm hole in the top where you can screw in the M3 standoff. Put in the standoffs (with some glue).

#### 2.5 Front cover
Drill wholes where the LEDs will go. Also drill a hole for the indication LED.

#### 2.6 Putting everything togerther

### 3. Result

## TODO
### DateAndTime.cpp
Get online data  
Sync data with RTC 
### LDRSensor.cpp
Scale the value to 0..15  
### Weather.cpp
Get temperature  
Get forecast
### ESPWifi.cpp
Implement non-hardcoded wifi connection
### Hardware
10k pulldown CS  
### Testing
LDR range / resistor values (3.3v!)  
Display brightness setting  
Order ESP32D :)))   

```c++
int analogPin = 0;
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 1000;
float R2 = 0;
float buffer = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  raw = analogRead(analogPin);
  if(raw) 
  {
    buffer = raw * Vin;
    Vout = (buffer)/1024.0;
    buffer = (Vin/Vout) -1;
    R2 = R1 * buffer;
    Serial.print("R2: ");
    Serial.println(R2);
    delay(100);
  }
}
```
