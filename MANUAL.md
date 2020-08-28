# Manual

## Powering the Smart Clock   
The clock is USB powered via USB type C. Connect an USB cable to a power source (like a phone charger or USB port) and connect the USB type C connector to the back of the clock like shown below.   
![USB C connector]()   

## Setting up the Smart Clock   
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
   
## Buttons   
![Buttons]()   
`- | Min` Brightness level down (this disables auto brightness if enabled). Hold to enable auto cycling.   
`○ | Select` Cycle between time, date, temperature and humidity (this disables auto cycling if enabled). Hold (for 5 seconds) to reset clock (wipes settings and saved networks).   
`+ | Plus`  Brightness level up (this disables auto brightness if enabled). Hold (for 3 seconds) to enable auto brightness.   