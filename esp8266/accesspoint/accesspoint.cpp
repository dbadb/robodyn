#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>

static void handleNotFound();
static bool handlePOST(String url); // returns true if handled
static bool handleFileRead(String url); // dito

static void log(String const &msg);
static void log(char const *msg);
static void send(String const &msg);

const char *ssid = "RoboNet"; // name of wifi
const char *password = "";    // wifi password, blank for an open network
const char *hostname = "robo";
const int LED_PIN = 14;

ESP8266WebServer httpServer(80);

void setup() 
{
    Serial.begin(115200);
    delay(10);
    Serial.println('\n');
    pinMode(LED_PIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output

    WiFi.softAP(ssid, password);             // Start the access point
    log(String("Access Point \"") + ssid + "\" started.");
    log(String("IP address: ") + WiFi.softAPIP().toString());

    /* initialize httpServer */
    httpServer.onNotFound(handleNotFound);
    httpServer.begin();
    log("http server started on port 80");

    if(MDNS.begin(hostname))
    {
        log(String("mDNS responder started as ") + hostname);
        MDNS.addService("http", "tcp", 80);
    }
    else
        log("MDNS error");

    LittleFS.begin();
}

void loop()
{
    MDNS.update();
    httpServer.handleClient();
    if(Serial.available())
    {
        // message from arduino or typed-in console (depends on DIP settings)
    }
}

static void handleNotFound()
{
    String uri = httpServer.uri();
    if(handlePOST(uri) || 
       handleFileRead(uri))
    {
        return;
    }
    else
    {
        log(String("unknown uri: ") + uri);
        httpServer.send(404, "text/plain", "404: Not found"); 
    }
}

// Since we want to minimize http overhead we'd like to
// support setting multiple parameters in a single post.
// Parameters can either arrive via the url: application/x-www-form-urlencoded
// or the post body. We'll gather them into a parameterlist and pass them
// the the ard.
static bool handlePOST(String url)
{
    bool found = false;
    if(url.equals("/ard"))
    {
        String murl = url;
        murl += "?";
        if(false && httpServer.hasArg(FPSTR("plain")))
        {
            murl += httpServer.arg(FPSTR("plain"));
            murl += "&";
        }
        else
        {
            for(int i=0;i<httpServer.args();i++)
            {
                if(httpServer.argName(i) == FPSTR("plain"))
                    continue;
                murl += httpServer.argName(i);
                murl += "=";
                murl += httpServer.arg(i);
                murl += "&";
            }
        }
        found = true;
        send(murl);
    }
    else
    if(url.equals("/esp"))
    {
        bool ledOn=false;
        String murl = url;
        murl += "?";
        for(int i=0;i<httpServer.args();i++)
        {
            String const &argName = httpServer.argName(i);
            if(argName == FPSTR("plain"))
                continue;

            String const &argVal = httpServer.arg(i);
            if(argName == FPSTR("led"))
                ledOn = true;

            murl += argName;
            murl += "=";
            murl += argVal;
            murl += "&";
        }
        digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
        found = true;
        log(murl);
    }
    if(found)
    {
        httpServer.sendHeader("Location", "/"); // redirect
        httpServer.send(303);                         
    }
    return found;
}

static String getContentType(String filename)
{
    if(filename.endsWith(".html")) return "text/html";
    if(filename.endsWith(".css")) return "text/css";
    if(filename.endsWith(".js")) return "application/javascript";
    if(filename.endsWith(".json")) return "application/json";
    if(filename.endsWith(".ico")) return "image/x-icon";
    if(filename.endsWith(".png")) return "image/png";
    if(filename.endsWith(".gif")) return "image/gif";
    if(filename.endsWith(".jpg")) return "image/jpeg";
    return "text/plain";
}

static bool handleFileRead(String path)
{
    if(path.endsWith("/"))
        path += "index.html";
    String contentType = getContentType(path);
    if(LittleFS.exists(path))
    {
        File f = LittleFS.open(path, "r");
        httpServer.streamFile(f, contentType); // returns size_t nbytes
        f.close();
        return true;
    }
    else
        return false;
}

static void send(String const &m)
{
    // Serial.print("emsg ");
    Serial.println(m);
}

static void log(String const &logmsg)
{
    Serial.print("elog ");
    Serial.println(logmsg);
}

static void log(char const *msg)
{
    log(String(msg));
}