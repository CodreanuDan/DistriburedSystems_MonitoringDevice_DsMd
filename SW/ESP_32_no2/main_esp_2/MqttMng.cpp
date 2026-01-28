/* MqttMng.cpp */

#include "MqttMng.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);
unsigned long lastMqttPublishTime = 0;
unsigned long lastReconnectAttempt = 0;

void MqttMng_Init() 
{
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setSocketTimeout(3);
    mqttClient.setCallback(MqttMng_Callback);
}

void MqttMng_MainFunction() 
{
    if (!g_systemValues.wifi.wifi_connected) return;

    if (!mqttClient.connected()) 
    {
        g_systemValues.wifi.mqtt_connected = false;
        unsigned long now = millis();
        if (now - lastReconnectAttempt > 5000) 
        {
            lastReconnectAttempt = now;
            Serial.print("[MQTT] Attempting connection...");
            /* Connect with "tesament" */
            if (mqttClient.connect("ESP32_Node_2", "user", "pass", "casa/nod2/overdrive", 1, true, "false")) 
            {
                g_systemValues.wifi.mqtt_connected = true;
                Serial.println("connected!");

                mqttClient.subscribe("casa/nod1/LDR");
                mqttClient.subscribe("casa/nod2/overdrive"); // Overdirve (0 or 1)
                mqttClient.subscribe("casa/nod2/set_pwm");   // PWM (0-255)
            } 
            else 
            {
                Serial.print("failed, rc=");
                Serial.println(mqttClient.state());
            }
        }
        return; 
    }

    mqttClient.loop();

    unsigned long now = millis();
    if (now - lastMqttPublishTime >= MQTT_PUBLISH_INTERVAL) 
    {
        lastMqttPublishTime = now;
        
        char jsonBuffer[256];

        snprintf(jsonBuffer, sizeof(jsonBuffer),
            "{\"t\":%.2f,\"h\":%d,\"p\":%.2f,\"itu\":%.2f,\"iaq\":%.1f,\"ls\":%d,\"lp\":%d}",
            g_systemValues.bme.temp,
            g_systemValues.bme.hum,
            g_systemValues.bme.pressure,
            g_systemValues.bme.itu,
            g_systemValues.bme.iaq,
            g_systemValues.led.toggled,
            g_systemValues.led.pwm
        );

        mqttClient.publish("casa/nod2/data", jsonBuffer);
        
        //Serial.println("[MQTT] Data published to broker.");
    }
}

void MqttMng_Callback(char* topic, byte* payload, unsigned int length) 
{
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';

    //Serial.printf("[MQTT RX] Topic: %s | Msg: %s\n", topic, message);

    if (String(topic) == "casa/nod1/LDR") 
    {
        g_systemValues.mqtt.light = atof(message);
    }

    else if (String(topic) == "casa/nod2/overdrive") 
    {
        g_systemValues.mqtt.overdrive = (atoi(message) == 1);
    }
    
    else if (String(topic) == "casa/nod2/set_pwm") 
    {
        g_systemValues.mqtt.pwm = (uint8_t)atoi(message);
    }
    
}