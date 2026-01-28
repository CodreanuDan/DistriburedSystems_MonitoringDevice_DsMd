/* LedCtrl.cpp */

#include "LedCtrl.h"

void LedCtrl_Init()
{
    g_systemValues.led.toggled = false;
    pinMode(LED_PIN_PWM, OUTPUT);
    analogWrite(LED_PIN_PWM, 0); 
}

void LedCtrl_MainFunction()
{
    if (g_systemValues.wifi.mqtt_connected == true)
    {
        if (g_systemValues.mqtt.overdrive == true)
        {
            LedCtrl_ApplyDutyCycle(g_systemValues.mqtt.pwm);
        }
        else if (g_systemValues.mqtt.overdrive == false)
        {
            if (g_systemValues.mqtt.light == 0)
            {
                LedCtrl_ApplyDutyCycle(0);
                g_systemValues.led.pwm = 0;
            }
            else if (g_systemValues.mqtt.light == 1)
            {
                LedCtrl_CyclicDimmer();
            }
        }
    }
    else
    {
        LedCtrl_ApplyDutyCycle(0);
    }

}

void LedCtrl_ApplyDutyCycle(uint8_t targetDuty)
{
    if (targetDuty > 0)
    {
        g_systemValues.led.toggled = true;
        g_systemValues.led.pwm = (uint8_t)((uint32_t)targetDuty * 100 / PWM_FULL);
    }
    else 
    {
        g_systemValues.led.toggled = false;
    }

    analogWrite(LED_PIN_PWM, targetDuty);
}

void LedCtrl_CyclicDimmer()
{
    static uint32_t lastChange = 0;
    static uint8_t  pwmLevelIndex = 0;
    const uint8_t pwmLevels[] ={0, PWM_25, PWM_50, PWM_75, PWM_FULL};
    uint32_t now = millis();

    if (now - lastChange >= LED_DIM_TIME)
    {
        lastChange = now;
        pwmLevelIndex = (pwmLevelIndex + 1) % (sizeof(pwmLevels) / sizeof(pwmLevels[0]));

        // Serial.println("--------------------------------------------------");
        // Serial.printf("[LED TEST] New target PWM: %u / 255 (%.1f %%)\n",
        //               pwmLevels[pwmLevelIndex],
        //               (pwmLevels[pwmLevelIndex] * 100.0f) / 255.0f);
    }

    LedCtrl_ApplyDutyCycle(pwmLevels[pwmLevelIndex]);
}
