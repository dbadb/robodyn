import requests
import time

url = "http://192.168.4.1/ard"

l = "on"
while True:
    print(l)
    r = requests.post(url, params={"led":l})
    if r.status_code != 200:
        print("post status: %d" % r.status_code)
    l = "off" if (l == "on") else "on"
    time.sleep(1)
