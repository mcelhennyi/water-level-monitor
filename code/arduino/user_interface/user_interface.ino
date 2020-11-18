#ifdef ARDUINO_ARCH_ESP32
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "Credentials.h"

#include "heltec.h"
#include "images.h"

// prototypes
boolean connectWifi();
String getHomePage();
String getSetupPage();

// Globals
boolean wifiConnected = false;
AsyncWebServer server(80);
int runRate = 2; // times per second

// RX-ed vars
struct RXData
{
  float   waterLevelFt = -99;
  boolean pumpOn = false;
  boolean buzzerOn = false;
  byte    lightColor = 0; // 0 - unknown, 1 - green, 2 - yellow, 3 - red

} rxData;

// TX-ed vars
// TX-ed :: Configuration params
struct ConfigData
{
  float minLevel = 2;
  float maxLevel = 3.5;
  float mountHeight = 5;
  float tankDepth = 4;
  
} configData;
boolean configUpdated = false;

// TX Details
#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6

// Display Vars
const int LINE_HEIGHT = 10;

// Heltec
void heltecLoop();


void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}


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

    // LORA SETUP
    //WIFI Kit series V1 not support Vext control
    Heltec.begin(
      true /*DisplayEnable Enable*/, 
      true /*Heltec.Heltec.Heltec.LoRa Disable*/, 
      true /*Serial Enable*/, 
      true /*PABOOST Enable*/, 
      BAND /*long BAND*/
    );
   
    Heltec.display->init();
    Heltec.display->flipScreenVertically();  
    Heltec.display->setFont(ArialMT_Plain_10);
    logo();
    delay(1500);
    Heltec.display->clear();
    
    Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
    Heltec.display->display();
    delay(1000);
    // LORA SETUP

    // Deep sleep setup
    // esp_sleep_enable_timer_wakeup(1000000 / 2);
    // esp_deep_sleep_start(); // Runs the deep sleep routine (a system reboot happens each time)
    
} // End setup

void loop() 
{    
    // Loop LoRa and  OLED
    heltecLoop();

    // Sleep
    delay(1000 / runRate);

} // End loop

void heltecLoop()
{
    // Setup Display
    Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_10);

    // First line
    Heltec.display->drawString(0, 0, "Water Level: " + String(rxData.waterLevelFt) + String(" ft"));

    // Second line
    Heltec.display->drawString(0, LINE_HEIGHT, "Pump: " + String(rxData.pumpOn ? "On": "Off"));
    Heltec.display->drawString(70, LINE_HEIGHT, "Buzzer: " + String(rxData.pumpOn ? "On": "Off") );

    // Third line
    Heltec.display->drawString(0, LINE_HEIGHT * 2, 
      "         Color: " + String(rxData.lightColor == 3 ? "Red": rxData.lightColor == 2 ? "Yellow": rxData.lightColor == 1 ? "Green": "Unknown"));

    // Fourth line
    Heltec.display->drawString(0, LINE_HEIGHT * 3, "---------- Configuration ---------");
    
    // Fifth line
    Heltec.display->drawString(0, LINE_HEIGHT * 4, "Hgt: " + String(configData.mountHeight));
    Heltec.display->drawString(70, LINE_HEIGHT * 4, "Dpth: " + String(configData.tankDepth) );

    // Sixth line
    Heltec.display->drawString(0, LINE_HEIGHT * 5, "Min lvl: " + String(configData.minLevel));
    Heltec.display->drawString(70, LINE_HEIGHT * 5, "Max lvl: " + String(configData.maxLevel) );

    // Display
    Heltec.display->display();
  
    // send packet
    if(configUpdated)
    {
        // Send once
        configUpdated = false;
        
        Serial.println("Configuration updated, sending changes.");
        LoRa.beginPacket();
        /*
         * LoRa.setTxPower(txPower,RFOUT_pin);
         * txPower -- 0 ~ 20
         * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
         *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
         *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
        */
        LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
        LoRa.write((unsigned char*) &configData, sizeof(ConfigData));
        LoRa.endPacket();
    }
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
        <label class=\'water_level\' > Water Level: " + String(rxData.waterLevelFt) + " </label> \
        <br> \
        <label class=\'pump_status\' > Pump Status: " + (rxData.pumpOn ? "On": "Off") + " </label> \
        ") 
      // END BODY
  ); // END makeHtmlPage
}

String addFormLine(String key, String label, float defaultValue)
{
  return String(" \
    <label for=\"") + key + String("\">") + label + String(":</label><br> \
    <input type=\"number\" id=\"") + key + String("\" name=\"") + key + String("\" step=\"0.1\" value=\"") + String(defaultValue) + String("\"><br><br> \
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
          addFormLine("min_level", "Min Level", configData.minLevel) + \
          addFormLine("max_level", "Max Level", configData.maxLevel) + \
  
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
      configData.minLevel = request->getParam("min_level")->value().toFloat();
  }
  else
  {
      Serial.println("min_level does not exist!");
  }

  if(request->hasParam("max_level"))
  {
      Serial.println("Max: " + String(request->getParam("max_level")->value().toFloat()));
      configData.maxLevel = request->getParam("max_level")->value().toFloat();
  }
  else
  {
      Serial.println("max_level does not exist!");
  }

//  int params = request->params();
//  for(int i=0;i<params;i++){
//    AsyncWebParameter* p = request->getParam(i);
//    if(p->isFile()){ //p->isPost() is also true
//      Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
//    } else if(p->isPost()){
//      Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
//    } else {
//      Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
//    }
//  }

  // Trigger update
  configUpdated = true;
  
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
