#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
//#include "Credentials.h"

// prototypes
//boolean connectWifi();
String getHomePage();
//String getSetupPage();

// Globals
boolean wifiConnected = false;
AsyncWebServer server(80);
float waterLevelFt = -9999;
boolean pumpOn = false;


void setup() 
{
  Serial.begin(115200);
//  
//  // Initialise wifi connection
//  wifiConnected = connectWifi();
//
//  // Connect to the Wifi
//  if(wifiConnected)
//  {
//
//    // Handle the GET for the webpage
//    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//      request->send(200, "text/html", getHomePage());
//    });
//
//    server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request){
//      request->send(200, "text/html", getSetupPage());
//    });
//
//    // Hange the GET for the data update
//    server.on("/test1", HTTP_GET, [](AsyncWebServerRequest *request){
//      request->send(200, "text/plain", "This is a second subpage you may have.");
//    });
//
//    // Handle 404 requests
//    server.onNotFound([](AsyncWebServerRequest *request){
//        //whatever you want to do with 404s
//        request->send(404, "text/plain", "Not found");
//    });
//  } 
//  // WIFI Failed to connect
//  else
//  {
//    while (1)
//    {
//      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
//      delay(2500);
//    }
//  }
    delay(5000);
    Serial.println(getHomePage());
}

void loop() 
{

}

//// connect to wifi – returns true if successful or false if not
//boolean connectWifi()
//{
//  boolean state = true;
//  int i = 0;
//  
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
//  Serial.println("");
//  Serial.println("Connecting to WiFi");
//
//  // Wait for connection
//  Serial.print("Connecting...");
//  while (WiFi.status() != WL_CONNECTED)0
//  {
//    delay(500);
//    Serial.print(".");
//    if (i > 20)
//    {
//      state = false; break;
//    }
//    i++;
//  }
//
//  Serial.println("");
//  if (state){
//    Serial.print("Connected to ");
//    Serial.println(ssid);
//    Serial.print("IP address: ");
//    Serial.println(WiFi.localIP());
//  }
//  else 
//  {
//    Serial.println("Connection failed.");
//  }
//  delay(100);
//  return state;
//}

// ----------------------------------------------------------------- //
// --------------------------- Web Pages --------------------------- //
// ----------------------------------------------------------------- //
String makeHtmlPage(String head, String style, String body)
{
  return String(" \
        <!DOCTYPE html> \
        <html> \
        <head> \
        ") + head + String(" \
        <style> \
        ") + style + String(" \
        </head> \
        </style> \
        <body> \
        ") + body + String(" \
        </body> \
        </html> \
    ");    
}


String getHomePage()
{
  // Return the Home page
  return makeHtmlPage(
    
      // HEAD TAG
      String(" \
      <META HTTP-EQUIV=\"refresh\" CONTENT=\"3\"> \
      "), // END HEAD
  
      // STYLE TAG
      String(" \
      label.water_level { \
        color: black; \n\
        background-color: rgb(255,255,255); \
      } \
      label.pump_status { \
        color: black; \n\
        background-color: rgb(255,255,255); \
      } \
      topcorner{ \
       position:absolute; \
       top:0; \
       right:0; \
      } \
      "), // END STYLE
      
      // BODY TAG
      String(" \
      <div class=\"topcorner\"><a href=\"/setup\">Setup Page</a></div> \
      <h1>Well Tank Monitor Portal</h1> \
      <label class=\'water_level\' > Water Level: " + String(waterLevelFt) + " </label> \
      <br> \
      <label class=\'pump_status\' > Pump Status: " + (pumpOn ? "On": "Off") + " </label> \
      ") // END BODY
    
  ); // END makeHtmlPage
}

String getSetupPage()
{
  return makeHtmlPage(
    
      // HEAD TAG
      String("\
                // NO HEAD
      "), // END HEAD
  
      // STYLE TAG
      String(" \
      topcorner{ \
       position:absolute; \
       top:0; \
       right:0; \
      } \
      "), // END STYLE
      
      // BODY TAG
      String(" \
      <div class=\"topcorner\"><a href=\"/\">Home Page</a></div> \
      <h1>Well Tank Monitor Portal</h1> \
      <label class=\'water_level\' > Water Level: " + String(waterLevelFt) + " </label> \
      <br> \
      <label class=\'pump_status\' > Pump Status: " + (pumpOn ? "On": "Off") + " </label> \
      ") // END BODY
    
    );
}
