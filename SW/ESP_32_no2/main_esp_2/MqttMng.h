/* MqttMng.h */
#ifndef MQTT_MNG_H
#define MQTT_MNG_H

#include <PubSubClient.h>
#include <WiFi.h>
#include "Values.h"

// Configurează IP-ul PC-ului tău aici
#define MQTT_SERVER "192.168.0.10" 
#define MQTT_PORT   1883
#define MQTT_PUBLISH_INTERVAL 1000UL

void MqttMng_Init();
void MqttMng_MainFunction();
void MqttMng_Callback(char* topic, byte* payload, unsigned int length);

#endif // MQTT_MNG_H