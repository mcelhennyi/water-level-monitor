# Water Level  Monitor
This project specifies the hardware and software necessary to monitor the water level within a tank of water.

## The Problem
My parents live outside of the city and have a well that they use to collect water for use. This well fills a ~1000 gallon tank. The logic for the well pump is to pump until the tank is full. Currently this consists of a float connected to a switch. Once the float rises high-enough the switch disconnects the power to the well. Conversely onces the float falls far enough it triggers the pump to turn on. Additionally there is a water level indicator float that has a pipe connected to it with red markings to show the amount of water in the tank for manual intervention. This system will replecate these two features.

### Pump auto-on/off


## Hardware
The hardware used for this project consist of two category: 1) The measurement device and 2) The User interface device.  
  
In addition to the below listed hardware, there will be a misc amount of 3D printed components and screw/nuts/bolts for mounting.

### Measurement device
[1 x Enclosure](https://www.amazon.com/gp/product/B07TS6RY85/ref=ppx_yo_dt_b_asin_title_o02_s03?ie=UTF8&psc=1)  
[5 x 5V 120V Opto Isolated Relays](https://www.amazon.com/gp/product/B07ZM6GBLS/ref=ppx_yo_dt_b_asin_title_o02_s05?ie=UTF8&psc=1)  
[1 x ESP32, LoRa/WiFi/Bluetooth, OLED](https://www.amazon.com/gp/product/B07HJ49VN8/ref=ppx_yo_dt_b_asin_title_o02_s06?ie=UTF8&psc=1)  
[1 x HC-SR04 Ultrasonic sensor](https://www.amazon.com/gp/product/B01JG09DCK/ref=ppx_yo_dt_b_asin_title_o02_s02?ie=UTF8&psc=1)  
[1 x AC to DC convertor](https://www.amazon.com/gp/product/B07SSWTJT3/ref=ppx_yo_dt_b_asin_title_o02_s01?ie=UTF8&psc=1)  
[1 x 3 position tower light with buzzer](https://www.amazon.com/gp/product/B0871ZTZ6S/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)  

### Water Level Indication
[1 x ESP32 + LoRa/WiFi/Bluetooth + OLED] (https://www.amazon.com/gp/product/B07HJ49VN8/ref=ppx_yo_dt_b_asin_title_o02_s06?ie=UTF8&psc=1)  
[1 x micro usb power supply](https://www.amazon.com/gp/product/B071KWFZ9Z/ref=ppx_yo_dt_b_asin_title_o02_s06?ie=UTF8&psc=1)  
