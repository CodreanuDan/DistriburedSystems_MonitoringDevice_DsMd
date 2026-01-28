/* Values.h */

#ifndef VALUES_H
#define VALUES_H

#include <stdint.h>
#include <stdbool.h>

#include "BtnMng.h"

/* ================= CURRENT SENSOR ================= */
typedef struct
{
    float current_A;       
    float voltage_V;
    float power_W;        
    float raw_adc;
    float raw_voltage;
} CurrentSensor_t;

/* ================= LIGHT SENSOR ================= */
typedef struct
{
    uint8_t level;
} LightSensor_t;

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

/* ================= MOTOR ================= */
typedef struct
{
    bool running;
    uint8_t pwm;
    float rpm;
    float threshold;
} MotorInfo_t;

/* ================= MQTT RX DATA  ================= */
typedef struct
{
    float temp;
    bool overdirve;
    uint8_t pwm;
    float threshold;
} MqttData_t;

/* ================= SYSTEM VALUES ================= */
typedef struct
{
    ButtonInfo_t button;
    CurrentSensor_t current;
    LightSensor_t  light;
    WifiInfo_t  wifi;
    MotorInfo_t motor;
    MqttData_t mqtt;
} SystemValues_t;

/* ========= GLOBAL INSTANCE (extern) ========= */
extern volatile SystemValues_t g_systemValues;

extern volatile bool ntpReady;  // flag: NTP sync ?
extern volatile unsigned long wifiConnectTime;  

#endif /* VALUES_H */
