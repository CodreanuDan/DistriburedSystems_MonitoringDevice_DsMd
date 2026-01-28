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
            if (mqttClient.connect("ESP32_Node_1", "user", "pass", "casa/nod1/overdrive", 1, true, "false"))
            {
                g_systemValues.wifi.mqtt_connected = true;
                Serial.println("connected!");

                mqttClient.subscribe("casa/nod2/temp");      // Temp Ref
                mqttClient.subscribe("casa/nod1/overdrive"); // Overdrive 1 or 0
                mqttClient.subscribe("casa/nod1/set_pwm");   // Set PWM
                mqttClient.subscribe("casa/nod1/tempThres"); // Temp Threshold
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

        char jsonPayload[128];
        sprintf(jsonPayload, "{\"ldr\":%d,\"u\":%.2f,\"i\":%.2f,\"p\":%.2f,\"rpm\":%d,\"stat\":%d,\"pwm\":%d}",
                g_systemValues.light.level,
                g_systemValues.current.voltage_V,
                g_systemValues.current.current_A,
                g_systemValues.current.power_W,
                (int)g_systemValues.motor.rpm,
                g_systemValues.motor.running ? 1 : 0,
                g_systemValues.motor.pwm);

        mqttClient.publish("casa/nod1/date", jsonPayload);
        
        //Serial.println("[MQTT] Data published to broker.");
    }
}

void MqttMng_Callback(char* topic, byte* payload, unsigned int length) 
{
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';

    // Serial.printf("[MQTT RX] Topic: %s | Msg: %s\n", topic, message);

    if (String(topic) == "casa/nod2/temp") 
    {
        g_systemValues.mqtt.temp = atof(message);
    }
    
    else if (String(topic) == "casa/nod1/overdrive") 
    {
        g_systemValues.mqtt.overdirve = (atoi(message) == 1);
    }
    
    else if (String(topic) == "casa/nod1/set_pwm") 
    {
        g_systemValues.mqtt.pwm = (uint8_t)atoi(message);
    }

    else if (String(topic) == "casa/nod1/tempThres") 
    {
        g_systemValues.mqtt.threshold = (float)atoi(message);
    }
}