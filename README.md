# IoTDoorOpener
Open a door using an Arduino connected to the internet via an ESP8266 and a webpage you can control the door opening and closing. The DC motor on the door is controlled over digital pins by setting one output to low and the other to low to move in the desired direction. Test the ESP8266 module, with no code flashed on it to determine the baud rate of the Wi-Fi module and adjust it accordingly for the software serial code. 

### Part List
|         Part        | Quantity | Link                                          |
|:-------------------:|:--------:|-----------------------------------------------|
|       Arduino       |     1    | https://store.arduino.cc/usa/arduino-uno-rev3 |
|       DC Motor      |     1    | https://www.sparkfun.com/products/11696       |
|       ESP8266       |     1    | https://www.sparkfun.com/products/13678       |
| Potentiometer (10k) | 1        | https://www.sparkfun.com/products/9288        |
| Resistor (220 ohm)  | 2        | https://www.sparkfun.com/products/10969       |
| LED                 | 1        | https://www.sparkfun.com/products/12062       |
| LCD Display         | 1        | https://www.sparkfun.com/products/255         |

### Hardware Connections
![Fritzing_Schmatic](https://github.com/pkashyap95/IoTDoorOpener/blob/master/Fritzing_Schmatic.JPG)
