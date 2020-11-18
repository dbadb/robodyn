#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

const char *ssid = "RoboNet"; // name of wifi
const char *password = "";    // wifi password, blank for an open network
const char *hostname = "robo";
const int LED_PIN = 14;

static void webSocketEvent(uint8_t num, WStype_t type, 
                            uint8_t *payload, size_t len);
static void log(String const &msg);
static void log(char const *msg);
static void sendARD(char const *m);

static int sNextPing = 0;
static String sPingBufs[2];

WebSocketsServer webSocket(81);

void setup() 
{
    pinMode(LED_PIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output

    Serial.begin(115200);
    delay(100);
    Serial.println('\n');

    WiFi.softAP(ssid, password);             // Start the access point
    log(String("Access Point \"") + ssid + "\" started.");
    log(String("IP address: ") + WiFi.softAPIP().toString());

    /* init websocket server */
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    log("websocket server started on port 81");
}

void loop()
{
    webSocket.loop();
    if(Serial.available())
    {
        String msg = Serial.readStringUntil('\n'); 
        if(msg.startsWith("/ard2esp/"))
        {
            // this is where ping 
            sPingBufs[sNextPing] = msg.c_str() + 9;
            sNextPing = (sNextPing + 1) % 2;
        }
    }
}

// https://github.com/Links2004/arduinoWebSockets
static void 
webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t len)
{
    switch (type) 
    {
    case WStype_DISCONNECTED:
        log(String("Disconnected ") + String(num));
        break;
    case WStype_CONNECTED: 
        {
            char buf[100];
            IPAddress ip = webSocket.remoteIP(num);
            snprintf(buf, 100,
                "[%u] Connected from %d.%d.%d.%d url: %s", 
                num, ip[0], ip[1], ip[2], ip[3], payload);
            log(buf);
            webSocket.sendTXT(num, "Connected");
        }
        break;
    case WStype_TEXT: 
        // log(String("WSText ") + String(num));
        if(!strncmp((char *) payload, "/ard", 4))
        {            
            sendARD((char *) payload);
            payload[0] = '\0';
            // webSocket.sendTXT(num, (char *) payload);
            webSocket.sendTXT(num, "ok");
        } 
        else
        if(!strncmp((char *) payload, "/esp/getping", 12))
        {
            webSocket.sendTXT(num, sPingBufs[(sNextPing+1)%2]);
        }
        else
        if(!strncmp((char *) payload, "/esp?led=", 9))
        {
            int l = atoi((char *) payload+9);
            digitalWrite(LED_PIN, l ? HIGH : LOW);
            webSocket.sendTXT(num, "ok");
        }
        else
            sendARD("/error/unrecognized payload");
        break;
    case WStype_ERROR:
        log(String("WSError ") + (char *) payload);
        break;
    case WStype_BIN: // currently unsupported
        log(String("WSText ") + String(num));
        break;
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    case WStype_PING:
    case WStype_PONG:
        /* unimplemented */
        break;
    }
}

static void sendARD(char const *m)
{
    // Serial.print("emsg ");
    Serial.println(m);
}

static void log(String const &logmsg)
{
    // nb: this will be consumed by arduino code
    Serial.print("/esplog:");
    Serial.println(logmsg);
}

static void log(char const *msg)
{
    Serial.print("/esplog:");
    Serial.println(msg);
}