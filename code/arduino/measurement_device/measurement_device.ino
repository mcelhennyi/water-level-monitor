#include "heltec.h"

// RX-ed vars
struct RXData
{
  float   waterLevelFt = 10;
  boolean pumpOn = false;
  boolean buzzerOn = false;
  byte    lightColor = 0; // 0 - unknown, 1 - green, 2 - yellow, 3 - red

} rxData;

// TX-ed vars
// TX-ed :: Configuration params
struct ConfigData
{
  float minLevel = 2;
  float maxLevel = 4;
  float mountHeight = 5;
  float tankDepth = 4;
  
} configData;
boolean configUpdated = false;

// LORA Details
#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6


void setup() 
{
    //WIFI Kit series V1 not support Vext control
    Heltec.begin(
      true /*DisplayEnable Enable*/, 
      true /*Heltec.LoRa Disable*/, 
      true /*Serial Enable*/, 
      true /*PABOOST Enable*/, 
      BAND /*long BAND*/
      );
    Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_10);
  
    // First line
    Heltec.display->drawString(0, 0, "Device enabled.");
    Heltec.display->display();
 
}

void loop() 
{

    // TESTER CODE
    rxData.waterLevelFt += 1;
    rxData.pumpOn = !rxData.pumpOn;
    rxData.buzzerOn = !rxData.buzzerOn;
    rxData.lightColor += 1;
    if(rxData.lightColor == 4)
      rxData.lightColor = 0;
    // TESTER CODE
    
    Serial.println("Send RXData");
    LoRa.beginPacket();
    /*
     * LoRa.setTxPower(txPower,RFOUT_pin);
     * txPower -- 0 ~ 20
     * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
     *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
     *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
    */
    LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
    LoRa.write((unsigned char*) &rxData, sizeof(RXData));
    LoRa.endPacket();

    delay(1000); // LoRa-wan is a slow tx - wait 1 second between messages.

    Serial.println("Send config");
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
    delay(1000);
}
