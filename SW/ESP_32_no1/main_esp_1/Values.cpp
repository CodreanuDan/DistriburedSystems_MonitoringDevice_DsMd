/* Values.cpp  */

#include "Values.h"

volatile SystemValues_t g_systemValues = {
    { 0U },                           // button.buttonState = 0 (BUTTON_RELEASED)
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // current (I, U, P, raw_adc, raw_voltage)
    { 0 },                            // light.level = 0
    { false, false, 0, "0.0.0.0" },   // wifi_connected, mqtt_connected, rssi, ip
    { false, 0U, 0.0f, 0.0f},         // motor.running, motor.pwm, rpm, threshold
    { 0.0f, false, 0u, 0},            // mqtt data: temp and overdirve of the motor, pwm, tempThreshold;
};

volatile bool ntpReady = false;
volatile unsigned long wifiConnectTime = 0;