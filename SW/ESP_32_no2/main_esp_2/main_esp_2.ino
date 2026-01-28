/* main_esp_1.ino */

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include "WifiMng.h"
#include "BME680Mng.h"
#include "BtnMng.h"
#include "LcdMenuMng.h"
#include "LedCtrl.h"
#include "TimeMng.h"
#include "MqttMng.h"

TaskHandle_t TaskBME680_hdl;

#define DBG_SERVICE_PRINT_INTERVAL 2000UL
void DebuggerService();

void BME680_TaskFunction(void * pvParameters);

void setup() 
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("=================> ESP NO.2 <==================");

    WifiMng_LoadNetworksInVector();
    TimeMng_Init();  /* ← inițializează NTP */
    LcdMng_Init();  /* initializeaza LCD + meniu */
    MqttMng_Init(); // <--- Inițializare MQTT
    BME680_Init();

    xTaskCreatePinnedToCore
    (
      BME680_TaskFunction,   /* Funcția task-ului */
      "BME680TASK",          /* Numele task-ului */
      8096,                  /* Stack size în words */
      NULL,                  /* Parametri */
      1,                     /* Prioritate */
      &TaskBME680_hdl,       /* Handle */
      0                      /* Core 0 */
    );
}

void loop() 
{
    WifiMng_MainFunction(); 
    MqttMng_MainFunction(); 
    
    LedCtrl_MainFunction();

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

        Serial.print("*T:"); Serial.print(g_systemValues.bme.temp); Serial.print("C ");
        Serial.print("H:"); Serial.print(g_systemValues.bme.hum); Serial.print("% ");
        Serial.print("P:"); Serial.print(g_systemValues.bme.pressure); Serial.print("hPa ");
        Serial.print("R:"); Serial.print(g_systemValues.bme.RkOhm); Serial.print(" kOhms");
        Serial.print("ITU:"); Serial.print(g_systemValues.bme.itu); Serial.print(" ");
        Serial.print("IAQ:"); Serial.print(g_systemValues.bme.iaq); Serial.print(" ");
        Serial.print("Alt:"); Serial.print(g_systemValues.bme.alt); Serial.print("m ");

        Serial.print("*LED:"); Serial.print(g_systemValues.led.toggled); Serial.print(" ");
        Serial.print("PWM:");  Serial.print(g_systemValues.led.pwm); Serial.print(" %");

        Serial.print("*OVD:");  Serial.print(g_systemValues.mqtt.overdrive); Serial.print(" ");
        Serial.print("LDR:");  Serial.print(g_systemValues.mqtt.light); Serial.print(" ");
        Serial.print("PWM:");  Serial.print(g_systemValues.mqtt.pwm); Serial.print(" ");

        Serial.println();
        
        last_print_time = now;
    }
}

void BME680_TaskFunction(void * pvParameters) 
{

    Serial.print("[BME680TASK]: Runs on CORE "); Serial.println(xPortGetCoreID());

    for(;;) 
    {
        BME680_MainFunction();
        vTaskDelay(pdMS_TO_TICKS(100)); 
    }
}
 




