/* Values.cpp  */

#include "Values.h"

volatile SystemValues_t g_systemValues = {
    { 0U },                                // button.buttonState = 0 (BUTTON_RELEASED)
    { 0.0f, 0U, 0.0f, 0.0f, 0.0f, 0.0f},   // bme680 T H P R ITU IAQ
    { false, false, 0, "0.0.0.0" },        // wifi_connected, mqtt_connected, rssi, ip
    { 0U, 0U},                             // led status, led pwm 
    { 0U, 0U, false},                      // mqtt toggle  status, set pwm, overdirve
};

volatile bool ntpReady = false;
volatile unsigned long wifiConnectTime = 0;