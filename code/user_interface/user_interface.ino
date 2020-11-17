#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "Credentials.h"

// prototypes
boolean connectWifi();
String getHomePage();
String getSetupPage();

// Globals
boolean wifiConnected = false;
AsyncWebServer server(80);
float waterLevelFt = -9999;
boolean pumpOn = false;

// Configuration params
float minLevel = -9999;
float maxLevel = 9999;


void setup() 
{
    Serial.begin(115200);
    
    // Initialise wifi connection
    wifiConnected = connectWifi();
  
    // Connect to the Wifi
    if(wifiConnected)
    {
  
      // Handle the GET for the webpage
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", getHomePage());
      });

      // Handle the setup
      server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", getSetupPage());
      });
  
      // Handle the GET for the data update
      server.on("/configure", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", handleConfigureGet(request));
      });
  
      // Handle 404 requests
      server.onNotFound([](AsyncWebServerRequest *request){
          //whatever you want to do with 404s
          request->send(404, "text/plain", "Not found");
      });

      // Run the server
      server.begin();
    } 
    // WIFI Failed to connect
    else
    {
      while (1)
      {
        Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
        delay(2500);
      }
    }
    
    delay(5000);
    //Serial.println(getSetupPage());
}

void loop() 
{

}

// connect to wifi – returns true if successful or false if not
boolean connectWifi()
{
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (i > 20)
    {
      state = false; break;
    }
    i++;
  }

  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else 
  {
    Serial.println("Connection failed.");
  }
  delay(100);
  return state;
}

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
        "), 
      // END HEAD
  
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
        "),
      // END STYLE
      
      // BODY TAG
        String(" \
        <div class=\"topcorner\"><a href=\"/setup\">Setup Page</a></div> \
        <h1>Well Tank Monitor Portal</h1> \
        <label class=\'water_level\' > Water Level: " + String(waterLevelFt) + " </label> \
        <br> \
        <label class=\'pump_status\' > Pump Status: " + (pumpOn ? "On": "Off") + " </label> \
        ") 
      // END BODY
  ); // END makeHtmlPage
}

String addFormLine(String key, String label, float defaultValue)
{
  return String(" \
    <label for=\"") + key + String("\">") + label + String(":</label><br> \
    <input type=\"number\" id=\"") + key + String("\" name=\"") + key + String("\" value=\"") + String(defaultValue) + String("\"><br><br> \
  ");
}

String getSetupPage()
{
  return makeHtmlPage(
    
      // HEAD TAG
        String(""), 
      // END HEAD
  
      // STYLE TAG
        String(" \
        topcorner{ \
         position:absolute; \
         top:0; \
         right:0; \
        } \
        "), 
      // END STYLE
      
      // BODY TAG
        String(" \
          <div class=\"topcorner\"><a href=\"/\">Home Page</a></div> \
          <form action=\"/configure\" method=\"get\"> \
          ") +
  
          // Min / Max depths
          addFormLine("min_level", "Min Level", minLevel) + \
          addFormLine("max_level", "Max Level", maxLevel) + \
  
          // End Form 
          String(" \
          <input id=\"submitme\" type=\"submit\" value=\"Submit\"> \
          </form> \
        ") 
      // END BODY
  ); // END makeHtmlPage
}

String handleConfigureGet(AsyncWebServerRequest *request)
{
  // Handle the configuration params
  // TODO: MUTEX this?
  
  if(request->hasParam("min_level"))
  {
      Serial.println("Min: " + String(request->getParam("min_level")->value().toFloat()));
      minLevel = request->getParam("min_level")->value().toFloat();
  }
  else
  {
      Serial.println("min_level does not exist!");
  }

  if(request->hasParam("max_level"))
  {
      Serial.println("Max: " + String(request->getParam("max_level")->value().toFloat()));
      maxLevel = request->getParam("max_level")->value().toFloat();
  }
  else
  {
      Serial.println("max_level does not exist!");
  }

  int params = request->params();
  for(int i=0;i<params;i++){
    AsyncWebParameter* p = request->getParam(i);
    if(p->isFile()){ //p->isPost() is also true
      Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
    } else if(p->isPost()){
      Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    } else {
      Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
  }

  Serial.flush();

  
  // Return the success page
  return makeHtmlPage(
      
        // HEAD TAG
          String(""), 
        // END HEAD
    
        // STYLE TAG
          String(" \
          topcorner{ \
           position:absolute; \
           top:0; \
           right:0; \
          } \
          "), 
        // END STYLE
        
        // BODY TAG
          String(" \
            <div class=\"topcorner\"><a href=\"/\">Home Page</a></div> \
            <form action=\"/configure\" method=\"get\"> \
            <label> Success </label> \
          ") 
        // END BODY
  ); // END makeHtmlPage
}
