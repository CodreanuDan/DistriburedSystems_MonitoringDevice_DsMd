/* Values.h */

#ifndef VALUES_H
#define VALUES_H

#include <stdint.h>
#include <stdbool.h>

#include "BtnMng.h"

/* ================= BME 680 SENSOR ================= */
typedef struct
{
    float temp;       
    uint8_t hum;
    float pressure;        
    float RkOhm;
    float itu;
    float iaq;
    float alt;
} Bme680Sensor_t;


/* ================= WIFI INFO ================= */
typedef struct
{
    bool wifi_connected;
    bool mqtt_connected;
    int8_t rssi;
    char ip[16];
} WifiInfo_t;

/* ================= BUTTON ================= */
typedef struct
{
    uint8_t buttonState;
} ButtonInfo_t;

/* ================= LED ================= */
typedef struct
{
    bool toggled;
    uint8_t pwm;
} LedInfo_t;

/* ================= MQTT RX VALUES ================= */
typedef struct
{
    uint8_t light;
    uint8_t pwm;
    bool overdrive;
} MqttData_t;

/* ================= SYSTEM VALUES ================= */
typedef struct
{
    ButtonInfo_t button;
    Bme680Sensor_t bme;
    WifiInfo_t wifi;
    LedInfo_t led;
    MqttData_t mqtt;
} SystemValues_t;

/* ========= GLOBAL INSTANCE (extern) ========= */
extern volatile SystemValues_t g_systemValues;

extern volatile bool ntpReady;  // flag: NTP sincronizat?
extern volatile unsigned long wifiConnectTime;  // timp când s-a conectat WiFi

#endif /* VALUES_H */
