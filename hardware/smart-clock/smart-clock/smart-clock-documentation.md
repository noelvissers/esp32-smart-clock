# Circuits
Below are the hardware circuits for the clock explained. If you have any questions about the circuits, or anything else, you can make an [issue](https://github.com/noelvissers/esp32-smart-clock/issues) and I'll try to answer it as soon as I can.   

Below you can see the two PCB projects, since we need two PCB's to make this clock. This is becuase the display is at a different angle (see [PCB](#pcb)).   
   
![SCH](/pictures/sch_smartclock.png?raw=true "SCH")   
   
Below you can see the schematic (blocks) for the main board. This is where most components are (outside the display). The buttons, RTC and LDR are all connected to the microcontroller, the ESP32, and the microcontroller is connected to the header that connects to the display PCB. X1 is just there for support. Note that there is no power input anywhere in these schematics. The power comes from the ESP32's USB port. This will be showed later in the mechanical design.   
   
![SCH base board](/pictures/sch_mainboard.png?raw=true "SCH base board")  
## ESP32
This is the heart of the clock; the microcontroller a.k.a. the ESP32. Since it the operating voltage of the microcontroller is 3.3V, the normal 5V USB voltage won't be suited. On the devkit there is a voltage regulator to bring down the 5V to 3.3V, but there is still a pin we can use that is connected to the 5V input. This means we can drive the 5V display from this USB power. For the data lines we need a level shifter (U1001). This means the ESP can send on 3.3V and the MAX7219 will receive this as 5V. 
   
![ESP32](/pictures/sch_esp32.png?raw=true "ESP32")   

## Buttons
For the buttons I used a simple debounce circuit. In testing I found out that this sometimes doesn't filter out all the bouncing, so this is also handled in software. This could be because of bad buttons or wrong resistor/capacitor values, but the solution I found in software with interrupts works very smooth right now. Having a pull-up resistor to 3.3V means the button is active LOW.   
   
![Buttons schematic](/pictures/sch_button.png?raw=true "Buttons schematic")   

## RTC
To keep track of the time, even when the clock is powered off, we need a RTC. The DS3231 RTC used in this project communicates via I2C. This means we need 4 pins connected to the ESP: SDA, SCL and GND/VCC. The DS3231 has a [typical input voltage of 3.3V](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf) which is perfect since the ESP runs on 3.3V. This also means there is no level shifter needed for the data and clock lines. Since the ESP32 has dedicated I2C pins, we can use these to communicate with the RTC. These are pin IO21 (SDA) and IO21 (SCL).   
   
![RTC schematic](/pictures/sch_rtc.png?raw=true "RTC schematic")   

## LDR
This is a simple voltage divider. The resistance of the LDR changes depending on the light level; more light means less resistance. This means the voltage to the analog input of the ESP changes; a higher voltage level with more light, a lower voltage level with less light. The sensitivity can be set with the potentiometer. Since this is an analog output, we need to make sure it is connected to an analog input on the ESP so we can use the ADC from the ESP to read this value.    
   
![LDR schematic](/pictures/sch_ldr.png?raw=true "LDR schematic")   

## Display
The dot matrix displays are powered via the [MAX7219 display driver](https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf) on 5V. Since the ESP developer board is powered by USB (5V), we can use the input voltage of the board to power the MAX7219. The other parts of ESP run on 3.3V, so we need a level shifter for the 3 data lines: D<sub>IN</sub>, CS and CLK. The CS and CLK lines go to both display 1 and 2, while the data line (D<sub>IN</sub>) only goes to display 1. The MAX7219 has a data out (D<sub>OUT</sub>) pin that can be connected to the data in pin of display 1. This way we can daisy chain the displays together.   
The I<sub>SET</sub> resistor sets the maximal current for the display. This means you will get less brightness with a higher resistor value. I found that the typical circuit that uses around 10kOhm is way too bright, especially at night. You might want to experiment with this a bit and see what value you prefer. In [this video](https://www.youtube.com/watch?v=MT4PSEJzVU0) you can see the effect of different resistor values, although a camera always tries to compensate for this light level. This video does however give a good indication of the minimum brightness. I used a 100k potentiometer in series with the recommended 10k resistor, so I can tweak it after assembly, where 10k is too bright and 100k (or 110k in this case) is probably too dark. So somewhere in the middle should be the sweat spot.   
   
![Display schematic](/pictures/sch_display.png?raw=true "Display schematic")   

# Testing
Testing all the hardware (and also software). This is to check if all the circuits work as intented.   
   
![Breadboard testing](/pictures/breadboard_test.png?raw=true "Breadboard testing")   
   
As you can see, the right display has a different brightness. During the testing phase, I found out the display can actually get pretty bright and also blinding. So I expirimented with different resistor values.
