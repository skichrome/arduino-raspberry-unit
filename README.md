# arduino-raspberry-unit
Arduino and Node-Red on Raspberry

This progam works with a node-red server on a banana-pi (Raspberry pi clone). The arduino is connected to the board with usb cable.

## Purpose
  - Sending 433Mhz codes, according to an id received on serial port.
  - Sending air quality data to the banana-pi
  - Detect if the light in the room must be on or off.

## Hardware used
  - Arduino nano
  - Banana Pi with Debian stretch lite
  - MQ-135 sensor
  - Pyroelectric sensor

## Libraries used
  - [MQ-135](https://github.com/GeorgK/MQ135)
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
  - [RCSwitch](https://github.com/sui77/rc-switch)
