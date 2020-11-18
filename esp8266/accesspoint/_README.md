# Accesspoint

## Intro
Sets up a wifi access point named RoboNet and theoretically advertises
a http service on roboctl:80.  For some reason the mdns isn't working
so we connect to roboctl on 192.168.4.1:80.

Webserver routes incoming POST urlencoded params to arduino-side 
(see robodyn project) under the base url `/ard?led=on&M1=.25&M2=.25`.
Checkboxes in html forms don't send unchecked boxes.

## Control

* use web browser to press buttons
* python requests
    * `python -m pip install requests`
* use curl to post controls
  * `curl -X POST "http://192.168.4.1/ard?led=1M1=.25&"`
  * `curl -d '{"led": "on"}' -H 'Content-Type: application/json' http://192.168.4.1/ard`
    * but accesspoint or robodyn would need to parse the json.
* use javascript XMLHttpRequest from another webpage
* use node axios/XMLHttpRequest command-line
* use java XMLHttpRequest

## Development

### download programs

Set DIP switch to:

* `00001110` to upload sketch (red 8266 LED illuminates)
* `00001100` to connect
* `11001100` to run with comms enabled and listen on esp-side println
* `11110000` to run with comms enabled and listen on ard-side println

Run upload.py (`jsmk test`), press reset button before initiation.

To upload webdata, touch data, then `jsmk install`. (upload.py at offset of 0x1000000)

https://robotdyn.com/uno-wifi-r3-atmega328p-esp8266-32mb-flash-usb-ttl-ch340g-micro-usb.html