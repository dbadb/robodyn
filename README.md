# robodyn
dabbling with robodyn

https://robotdyn.com/uno-wifi-r3-atmega

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
