/* main_esp_1.ino */

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include "WifiMng.h"
#include "MAX471_Hdl.h"
#include "BtnMng.h"
#include "LcdMenuMng.h"
#include "LdrHdl.h"
#include "TimeMng.h"
#include "MqttMng.h"
#include "MotorCtrl.h"

#define DBG_SERVICE_PRINT_INTERVAL 2000UL
void DebuggerService();


void setup() 
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("=================> ESP NO.1 <==================");

    Max471_Init(MAX_471_PIN, ADC_RESOLUTION, ADC_0db);
    LdrHdl_Init();

    MotorCtrl_Init();

    WifiMng_LoadNetworksInVector();
    TimeMng_Init(); 
    LcdMng_Init();   
    MqttMng_Init(); 

}

void loop() 
{
    // MotorCtrl_TestFunction();
    MotorCtrl_MainFunction(); 
    // MotorCtrl_ApplyCyclicPWM(PWM_25);
    // MotorCtrl_ApplyPWM(PWM_75);
    // MotorCtrl_ProcessRPM();
    WifiMng_MainFunction(); 
    MqttMng_MainFunction(); 
    
    Max471_Hdl();
    LdrHdl_MainFunction();

    BtnMng_MainFunction(BTN_PIN, BtnMng_debounceTime);
    LcdMng_MainFunction();
    DebuggerService();

    delay(20); 
}

void DebuggerService()
{
    static unsigned long last_print_time = 0;
    unsigned long now = millis();

    if (now - last_print_time >= DBG_SERVICE_PRINT_INTERVAL)
    {

        /* Timestamp live */
        Serial.print("*Δt:"); Serial.print(now / 1000); Serial.print(" ");

        Serial.print("*IP:"); Serial.print((const char*)g_systemValues.wifi.ip); Serial.print(" ");
        Serial.print("RSSI:"); Serial.print(g_systemValues.wifi.rssi); Serial.print("dBm ");
        Serial.print("MQTT:"); Serial.print(g_systemValues.wifi.mqtt_connected); Serial.print(" ");

        Serial.print("*UL:"); Serial.print(g_systemValues.current.voltage_V); Serial.print("V ");
        Serial.print("IL:"); Serial.print(g_systemValues.current.current_A); Serial.print("A ");
        Serial.print("PM:"); Serial.print(g_systemValues.current.power_W); Serial.print("W ");
        Serial.print("ADC:"); Serial.print(g_systemValues.current.raw_adc); Serial.print(" ");

        Serial.print("*LDR:"); Serial.print(g_systemValues.light.level); Serial.print(" ");

        Serial.print("*RPM:");  Serial.print(g_systemValues.motor.rpm); Serial.print(" ");
        Serial.print("*TEMP:");  Serial.print(g_systemValues.mqtt.temp); Serial.print("C ");
        Serial.print("OVD:");  Serial.print(g_systemValues.mqtt.overdirve); Serial.print(" ");
        Serial.print("THR_SET:");  Serial.print(g_systemValues.mqtt.threshold); Serial.print(" ");
        Serial.print("PWM:");  Serial.print(g_systemValues.mqtt.pwm); Serial.print(" ");

        Serial.println();
        
        last_print_time = now;
    }
}
 




