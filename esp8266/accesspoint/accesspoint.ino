#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

const char *ssid = "RoboCtl"; // name of wifi
const char *password = "";    // wifi password, blank for an open network
const int LED_PIN = 14;

ESP8266WebServer httpServer(80);

static void handleNotFound();
static void handleLED();


void setup() 
{
    Serial.begin(115200);
    delay(10);
    Serial.println('\n');
    pinMode(LED_PIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output

    WiFi.softAP(ssid, password);             // Start the access point
    Serial.print("Access Point \"");
    Serial.print(ssid);
    Serial.println("\" started");

    Serial.print("IP address:\t");
    Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer

    /* initialize httpServer */
    httpServer.on("/led", handleLED);
    httpServer.onNotFound(handleNotFound);
    httpServer.begin();
    Serial.println("http server started on port 80");

    LittleFS.begin();
}

void loop()
{
    httpServer.handleClient();
}

static void handleLED() 
{
    digitalWrite(LED_PIN,!digitalRead(LED_PIN));
    // redirect to /
    httpServer.sendHeader("Location", "/");
    httpServer.send(303);                         
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
        Serial.println(String("Sent file: ") + path);
        return true;
    }
    else
    {
        Serial.println(String("Missing file: ") + path);
        return false;
    }
}

static void handleNotFound()
{
    if(!handleFileRead(httpServer.uri()))
        httpServer.send(404, "text/plain", "404: Not found"); 
}
