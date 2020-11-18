# AccesspointWS

## Overview

AccesspointWS is the communication/Remote-Control component
for our mobile robot.  It currently targets https://robotdyn.com/uno-wifi-r3-atmega328p-esp8266-32mb-flash-usb-ttl-ch340g-micro-usb.html,
but could be retargeted to other "wifi-equipped" IOT models.

AccesspointWS sets up a wifi access point named RoboNet and 
offers a websocket service on ws://192.168.4.1:81.

WebSocketServer accepts connections then routes incoming 
url-like messages to control settings on the esp8266 and/or 
the arduino code.  To dispatch messages to arduino (where, eg, motors
are controlled), the esp8266 program employs Serial.println. Note that
this only works when the DIP switches on the robodyn are configured 
`11000000` (or `11110000`, or `11001100`). 

In this implementation, websocket messages take the form of a url 
with url-encoded parameters.  The Arduino-side handling is implemented in the 
robodyn project and responds to urls like this: 

  `/ard?led=on&M1=.25&M2=.25&`

or this:

  `/esp?led=on&`

The interpretation of the messages is up to the target program.

To obtain status from the robot, we can either return status
as part of the url protocol or implement a UDP broadcast on the
esp8266.  Since most status is expected to reside on the Arduino
side, the esp8266 must be provided status from the Arduino code.

## Remote Control

* python websockets (see blinky.py):
    * `python -m pip install asyncio websockets`
    * `python blinky.py` (toggles two LEDs 10 times per sec).
* web browser with javascript
    * currently the esp code (accesspointWS) doesn't offer
      a http server. The companion project, `accesspoint`, 
      demonstrates that http is too slow for control and
      may therefore not be worth the extra complexity.
    * one could easily develop a desktop server, analogous
      (or equivalent to?) the Spartronics Dashboard server
      that runs on the driver station and connects to
      the websocket.

## During Development

Remember: the robodyn is a board with two different programmable chips
and two different machine architectures. We must select/configure toolset 
for each architecture and make sure that the dev settings are correct for
the target chip.  This directory contains programs for the esp8266 and
a companion directory, robodyn, contains a program for the arduino-uno.

### install libraries

A non-standard platform library is required for websockets. We are 
currently using this websocket implementation: https://github.com/Links2004/arduinoWebSockets,
with a tutorial here: https://tttapa.github.io/ESP8266/Chap14%20-%20WebSocket.html.
The websocket library should be compiled with debugging strings disabled
via a commandline define (or by editing the header file).

  `-DNODEBUG_WEBSOCKETS`.

### upload programs

To upload code to esp8266, set DIP switch to:

* `00001110` to upload this sketch (red 8266 LED illuminates)

Run upload.py (`jsmk test`), press reset button before initiation.

To run code on esp8266, set DIP switches to:

* `11110000` to run with comms enabled and listen on ard-side println.
    This is the preferred operating mode.
* `11001100` to run with comms enabled and listen on esp-side println

Now you should be able to connect to the wifi accesspoint RoboCtl and
view the debugging console with arduino IDE Serial Monitor.
