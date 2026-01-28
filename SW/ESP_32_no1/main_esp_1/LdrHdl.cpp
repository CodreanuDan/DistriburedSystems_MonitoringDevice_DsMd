/* LdrHdl.cpp */

#include "LdrHdl.h"

void LdrHdl_Init()
{
    pinMode(LDR_PIN, INPUT);
}

void LdrHdl_MainFunction()
{
    static unsigned long LdrHdl_LastMeasureTime_ms = 0;
    unsigned long now = millis();

    if (now - LdrHdl_LastMeasureTime_ms >= LdrHdl_MeasureInterval_ms)
    {
        byte level = digitalRead(LDR_PIN);
        g_systemValues.light.level = level;

        LdrHdl_LastMeasureTime_ms = now;
    }
}