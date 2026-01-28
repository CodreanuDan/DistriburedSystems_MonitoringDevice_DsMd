/* MotorCtrl.cpp */

#include "MotorCtrl.h"

static uint8_t last_applied_pwm = 0;
static float last_temp_threshold = 0;

void MotorCtrl_Init(void)
{
    g_systemValues.motor.running = false;

    pinMode(MOTOR_PWM_PIN, OUTPUT);

    ledcAttach(MOTOR_PWM_PIN, MOTOR_PWM_FREQ, MOTOR_PWM_RES);
    ledcWrite(MOTOR_PWM_PIN, 0);

}

void MotorCtrl_MainFunction()
{
    if (g_systemValues.wifi.mqtt_connected == true)
    {
        if (g_systemValues.mqtt.overdirve == true)
        {
            MotorCtrl_ApplyPWM(g_systemValues.mqtt.pwm);
        }
        else if (g_systemValues.mqtt.overdirve == false)
        {
            MotorCtrl_Auto();
        }
    }
    else
    {
        MotorCtrl_ApplyPWM(0);
    }

    MotorCtrl_ProcessRPM();
}

void MotorCtrl_ApplyPWM(uint8_t targetDuty)
{
    if ( g_systemValues.motor.running == false && targetDuty > 10)
    {
        MotorCtrl_Kickstart(targetDuty);
        ledcWrite(MOTOR_PWM_PIN, targetDuty);
        last_applied_pwm = targetDuty;
        MotorCtrl_CheckMotorStat(targetDuty);
    }
    else if (targetDuty < 10)
    {
        ledcWrite(MOTOR_PWM_PIN, targetDuty);
        last_applied_pwm = targetDuty;
        MotorCtrl_CheckMotorStat(targetDuty);
    }
    else 
    {
        if (targetDuty != last_applied_pwm && targetDuty < last_applied_pwm) 
        {
            MotorCtrl_Kickstart(targetDuty);
        }
        ledcWrite(MOTOR_PWM_PIN, targetDuty);
        last_applied_pwm = targetDuty;
        MotorCtrl_CheckMotorStat(targetDuty);
    }
}

void MotorCtrl_CheckMotorStat(uint8_t targetDuty)
{
    if (g_systemValues.current.voltage_V > 0.5f && g_systemValues.current.current_A >= 0.1f)
    {
        g_systemValues.motor.running = true;
        g_systemValues.motor.pwm =  (uint8_t)((uint32_t)targetDuty * 100 / PWM_FULL);
    }
    else if (targetDuty == 0)
    {
        g_systemValues.motor.running = false;
        g_systemValues.motor.pwm =  0U; 
    }
    else 
    {
        g_systemValues.motor.running = false;
        g_systemValues.motor.pwm =  0U;
    }
}

void MotorCtrl_ProcessRPM() 
{

    const float Kv = 1866.6f; 
    const float Ri = R_MOTO;     

    /* Calculate Back-EMF Eb = V_applied - (I * R_intern) */
    float drop = g_systemValues.current.current_A * Ri;
    float Eb = g_systemValues.current.voltage_V - (g_systemValues.current.current_A * Ri);

    // Serial.print("V:"); Serial.print(g_systemValues.current.voltage_V);
    // Serial.print(" | Drop:"); Serial.print(drop);
    // Serial.print(" | Eb:"); Serial.println(Eb);

    /* Filter value */ 
    if (Eb < 0 || g_systemValues.motor.running == false) 
    {
        g_systemValues.motor.rpm = 0.0f;
    } 
    else 
    {
        /* Calculate estimated RPM*/ 
        g_systemValues.motor.rpm = (float)((uint32_t)((Eb * Kv) + 0.5f));
    }

}

void MotorCtrl_Auto()
{
    float target = (float)g_systemValues.mqtt.threshold; 
    float currentTemp = g_systemValues.mqtt.temp;
    
    /* Calculate the difference (Delta T) */
    float diff = currentTemp - target; 
    g_systemValues.motor.threshold = diff; 

    /* Threshold > +2.0: Set PWM to 100% */
    if (diff >= 2.0f) 
    {
        MotorCtrl_ApplyPWM(PWM_FULL);
    }
    /* Threshold between +1.0 and +2.0: Set PWM to 75% */
    else if (diff >= 1.0f && diff < 2.0f) 
    {
        MotorCtrl_ApplyPWM(PWM_75);
    }
    /* Threshold between +0.25 and +1.0: Set PWM to 60% */
    else if (diff > 0.25f && diff < 1.0f) 
    {
        MotorCtrl_ApplyPWM(PWM_60);
    }
    /* Threshold between -0.25 and +0.25: Stop the Motor (Deadband) */
    else if (diff >= -0.25f && diff <= 0.25f) 
    {
        MotorCtrl_ApplyPWM(0);
    }
    /* Threshold between -1.0 and -0.25: Set PWM to 55% */
    else if (diff >= -1.0f && diff < -0.25f) 
    {
        MotorCtrl_ApplyPWM(PWM_55);
    }
    /* Threshold between -2.0 and -1.0: Set PWM to 50% */
    else if (diff >= -2.0f && diff < -1.0f) 
    {
        MotorCtrl_ApplyPWM(PWM_50);
    }
    /* Threshold < -2.0: Keep PWM at 45% */
    else if (diff < -2.0f) 
    {
        MotorCtrl_ApplyPWM(PWM_45);
    }
}

void MotorCtrl_Kickstart(uint8_t targetDuty)
{
    Serial.println("[KICSTART ACTIVE]");
    for (uint8_t i = 0; i < MotorCtrl_KickStartSequence_Size; i++)
    {
        if ( MotorCtrl_KickStartSequence[i] >= targetDuty)
        {
            uint8_t pace = ((uint8_t)(MotorCtrl_KickStartSequence[i] * PWM_FULL));
            ledcWrite(MOTOR_PWM_PIN, pace);
            vTaskDelay(pdMS_TO_TICKS(50));

        }
    }
    Serial.println("[KICSTART DISABLED]");
}

/* Test Functions */

void MotorCtrl_ApplyCyclicPWM(uint8_t targetDuty)
{
    uint32_t now = millis();
    MotorCtrl_CheckMotorStat(targetDuty);

    if (targetDuty >= PWM_FULL)
    {
        ledcWrite(MOTOR_PWM_PIN, PWM_FULL);
        motorPhase = MOTOR_PHASE_BOOST;
        motorPhaseTimestamp = now;
        return;
    }

    switch (motorPhase)
    {
        case MOTOR_PHASE_BOOST:
            ledcWrite(MOTOR_PWM_PIN, PWM_FULL);

            if (now - motorPhaseTimestamp >= BOOST_TIME_MS)
            {
                motorPhase = MOTOR_PHASE_HOLD;
                motorPhaseTimestamp = now;
            }
            break;

        case MOTOR_PHASE_HOLD:
            ledcWrite(MOTOR_PWM_PIN, targetDuty);

            if (now - motorPhaseTimestamp >= HOLD_TIME_MS)
            {
                motorPhase = MOTOR_PHASE_BOOST;
                motorPhaseTimestamp = now;
            }
            break;
    }
}

void MotorCtrl_TestFunction(void)
{
    static uint32_t lastChange = 0;
    static uint8_t  pwmLevelIndex = 0;

    const uint8_t pwmLevels[] =
    {
        PWM_25,
        PWM_50,
        PWM_75,
        PWM_FULL
    };

    uint32_t now = millis();

    if (now - lastChange >= 20000)
    {
        lastChange = now;

        pwmLevelIndex = (pwmLevelIndex + 1) % (sizeof(pwmLevels) / sizeof(pwmLevels[0]));

        Serial.println("--------------------------------------------------");
        Serial.printf("[Motor TEST] New target PWM: %u / 255 (%.1f %%)\n",
                      pwmLevels[pwmLevelIndex],
                      (pwmLevels[pwmLevelIndex] * 100.0f) / 255.0f);

        MotorCtrl_ResetCycle();
    }

    MotorCtrl_ApplyCyclicPWM(pwmLevels[pwmLevelIndex]);

    static uint32_t lastPrint = 0;
    if (now - lastPrint >= 500)
    {
        lastPrint = now;

        Serial.printf("[Motor PWM] Phase: %s | Target: %u | Active PWM: %u\n",
                      (motorPhase == MOTOR_PHASE_BOOST) ? "BOOST" : "HOLD",
                      pwmLevels[pwmLevelIndex],
                      (motorPhase == MOTOR_PHASE_BOOST) ? PWM_FULL : pwmLevels[pwmLevelIndex]);
    }
}

void MotorCtrl_ResetCycle(void)
{
    motorPhase = MOTOR_PHASE_BOOST;
    motorPhaseTimestamp = millis();
}
