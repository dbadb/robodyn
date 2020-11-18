# robodyn

Controls arduino-uno side pins for input and output. Listens on serial
connection to wifi-enabled esp8266 for command/control. See esp8266/accesspointWS
for more on the wifi side.  

* receive a url-encoded reqwuest over serial to establish/modify our 
  runtime behavior.
* send updates of current state (sensors, motor-levels, etc) periodically
* turn motors off if we haven't received a heartbeat within the motor-safety
  interval.

## download programs

Set DIP switch to:

* `00110000` to upload sketch
* `11001100` to run with comms enabled and listen on esp-side println
* `11110000` to run with comms enabled and listen on ard-side println

Run avrdude (via jsmk test), press reset button right after initiation.

https://robotdyn.com/uno-wifi-r3-atmega328p-esp8266-32mb-flash-usb-ttl-ch340g-micro-usb.html