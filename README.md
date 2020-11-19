# robodyn
dabbling with robodyn

https://robotdyn.com/uno-wifi-r3-atmega

https://docs.google.com/presentation/d/1pR-GWibRpY5YUyJdgLA0I1OqRzdq0XbG7Jeu1yraSqM/edit?usp=sharing

This part is comprised of two programable units: esp8266 for wifi and arduino-uno for robot control.

On the esp8266 side, we have two examples: accessPoint and accessPointWS. The former uses
http + POST as the control mechanism.  The latter uses websockets.  Websockets offer significant
advantage in performance.  Both versions include python client examples to help characterize the 
per-message overhead and convey the "programming model".

On the Arduino side, a trivial example, `robodyn` is presented to show how to send and receive and
parse control messages.

I've built these using a custom build environment, `jsmk`.  It should be possible to build
and deploy these examples with the generic ESP toolset and the standard Arduino board.
More configuration notes can be found in the associated projects.

Getting started:

* configure your build environment for both ESP8266 and Arduino-UNO
* make sure you have the correct websockets library for ESP8266, build it with debugging disabled
* deploy the robodyn and accessPointWS project with the appropriate DIP settings
* start the robodyn
* connect via the wifi net
* look at the serial output associated with your COM port (Arduino IDE should work)
* cd esp8266/accesspointWS
* python blinky.py (make lights blink and pings the arduino for machine state)

Going beyond:

* connect your arduino pins to real sensors and motors
* update the comms/websocket protocol to support your devices
* develop a front end that connects to the websocket
* create autonomous and user-controlled programs

