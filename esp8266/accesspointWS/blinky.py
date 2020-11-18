import time
import asyncio
import websockets

# https://websockets.readthedocs.io/en/stable/intro.html

async def doit():
    url = "ws://192.168.4.1:81"
    l = "1"
    async with websockets.connect(url) as websocket:
        res = await websocket.recv()
        print(res)
        await websocket.send("/ard?verbose=1&")
        res = await websocket.recv()
        print("verbose 1: " + res)
        v = 0
        while True:
            v += 1
            # print(l)
            for i in range(2):
                # handshake
                if i == 0:
                    if v == 100:
                        await websocket.send("/ard?verbose=0")
                        res = await websocket.recv()
                        print("verbose 0: " + res);
                    l = "0" if l == "1" else "1"
                    msg = "/ard?led=%s&" % l
                else:
                    msg = "/esp?led=%s&" % l
                # print(msg)
                await websocket.send(msg)
                res = await websocket.recv()
                if res == "break":
                    break
                elif res != "ok":
                    print("unknown ack: %s" % res)
                time.sleep(.1)
             
            # first request ard to send esp an update, this
            # could be automatic, but for now...
            await websocket.send("/ard/ping")
            res = await websocket.recv()
            if res == "ok":
                # retrieve the esp-side ping state
                await websocket.send("/esp/getping")
                res = await websocket.recv()
                print(res)
            time.sleep(.1)

asyncio.get_event_loop().run_until_complete(doit())
