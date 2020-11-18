#include "heltec.h"

// RX-ed vars
struct RXData
{
  float   waterLevelFt = 10;
  boolean pumpOn = false;
  boolean buzzerOn = false;
  byte    lightColor = 0; // 0 - unknown, 1 - green, 2 - yellow, 3 - red

} rxData;


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

}

void loop() 
{

    rxData.waterLevelFt += 1;
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

    Serial.println("Send packet");

    delay(1000);
}
