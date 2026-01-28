/* TimeMng.cpp */

#include "TimeMng.h"

void TimeMng_Init()
{
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
    Serial.println(">>> [TIME] NTP initialized. Syncing on WiFi connect.");

    ntpReady = false;
}

String TimeMng_GetTimestamp()
{
    if (!ntpReady) 
    {
        unsigned long totalSec = millis() / 1000;
        int hours = (totalSec / 3600) % 24;
        int minutes = (totalSec / 60) % 60;
        int seconds = totalSec % 60;
        char timeStr[9];
        snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
        return String(timeStr);
    }

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 1000))  // timeout 1 
    {
        Serial.println("<<< [TIME] NTP sync failed. Using fallback.");
        ntpReady = false;
        return "00:00:00";  // fallback
    }
    ntpReady = true;

    char timeString[20];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(timeString);
}