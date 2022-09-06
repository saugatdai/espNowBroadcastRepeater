/*
 * "THE BEER-WARE LICENSE" (Revision 42):
 * regenbogencode@gmail.com wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return
 */
#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif
#include "ESPNowW.h"

#define SECRET_DATA "PRABHU"

#define LIGHTPIN 2

uint8_t broadcastMac[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

boolean light = true;

struct TokenData
{
    char category[32];
    int tokenNumber;
    int counterNumber;
    char secretString[15];
} tokenData;

void onRecv(unsigned char *mac_addr, unsigned char *data, unsigned char data_len)
{
    memcpy(&tokenData, data, sizeof(tokenData));

    if (strcmp(tokenData.secretString, SECRET_DATA) == 0)
    {
        light = !light;
        digitalWrite(LIGHTPIN, light);
        ESPNow.send_message(broadcastMac, (uint8_t *)&tokenData, sizeof(tokenData));
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("ESPNow receiver Demo");
    pinMode(LIGHTPIN, OUTPUT);
#ifdef ESP8266
    WiFi.mode(WIFI_STA); // MUST NOT BE WIFI_MODE_NULL
#elif ESP32
    WiFi.mode(WIFI_MODE_STA);
#endif
    WiFi.disconnect();
    ESPNow.init();
    ESPNow.reg_recv_cb(onRecv);
}

void loop() {}