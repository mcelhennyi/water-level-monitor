
<p align="center">
  <img src="https://github.com/mcelhennyi/water-level-monitor/blob/main/logo.png" />
</p>

# Water Level  Monitor
This project specifies the hardware and software necessary to monitor the water level within a tank of water.

## The Problem
My parents live outside of the city and have a well that they use to collect water for use. This well fills a ~1000 gallon tank. The logic for the well pump is to pump until the tank is full. Currently this consists of a float connected to a switch. Once the float rises high-enough the switch disconnects the power to the well. Conversely onces the float falls far enough it triggers the pump to turn on. Additionally there is a water level indicator float that has a pipe connected to it with red markings to show the amount of water in the tank for manual intervention. This system will replecate these two features.

### Pump auto-on/off
This part of the measurement device logic will essentially turn the relay on or off depending on the level of the water. The user will be able to set a min threshold and a max threshold for the tank to turn on and off.

### Water Level Indication
This part of the logic the device will both send a signal to the user interface device with the current water level AND will light up a three tiered light for empty, half and full. When the tank water is raising, and the device is not in control (i.e. someone is filling the tank externally and the pump isnt the source of the added water) the device will set off a buzzer if the water level continues to rise after it signals with a green light that it is full of water. 

### User Interface
This part is also split into two parts: The on device display AND the webpage hosted on the device.  The display will show two simple things: The level of the water AND the status of the pump (on/off). In addition to the on device display the device will join the houses wifi, and will host a server that is available to connect to from a users  phone so they can see (from anywhere within range of the home wifi) the level of the water and the status of the pump.

## Hardware
The hardware used for this project consist of two category:   

1) The measurement device.  
2) The User interface device.  
  
In addition to the below listed hardware, there will be a misc amount of 3D printed components and screw/nuts/bolts for mounting.

### Measurement device
- [1 x Water Proof Enclosure](https://www.amazon.com/gp/product/B07TS6RY85/ref=ppx_yo_dt_b_asin_title_o02_s03?ie=UTF8&psc=1)  
- [5 x 5V 120V Opto Isolated Relays](https://www.amazon.com/gp/product/B07ZM6GBLS/ref=ppx_yo_dt_b_asin_title_o02_s05?ie=UTF8&psc=1)  
- [1 x ESP32, LoRa/WiFi/Bluetooth, OLED](https://www.amazon.com/gp/product/B07HJ49VN8/ref=ppx_yo_dt_b_asin_title_o02_s06?ie=UTF8&psc=1)  
- [1 x HC-SR04 Ultrasonic sensor](https://www.amazon.com/gp/product/B01JG09DCK/ref=ppx_yo_dt_b_asin_title_o02_s02?ie=UTF8&psc=1)  
- [1 x AC to DC convertor](https://www.amazon.com/gp/product/B07SSWTJT3/ref=ppx_yo_dt_b_asin_title_o02_s01?ie=UTF8&psc=1)  
- [1 x 3 position tower light with buzzer](https://www.amazon.com/gp/product/B0871ZTZ6S/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)  

### User Interface Device
In addition to the below hardware, a custom 3D printed enclosure will be created as well.  
- [1 x ESP32 + LoRa/WiFi/Bluetooth + OLED](https://www.amazon.com/gp/product/B07HJ49VN8/ref=ppx_yo_dt_b_asin_title_o02_s06?ie=UTF8&psc=1)  
- [1 x micro usb power supply](https://www.amazon.com/gp/product/B071KWFZ9Z/ref=ppx_yo_dt_b_asin_title_o02_s06?ie=UTF8&psc=1)  

## Similar Projects
These projects were either referenced, and/or used in the creation of this repo.

- [Water Level Meter](https://github.com/grillbaer/esp32-lora-water-level-meter)
