/* BME680Mng.cpp */

#include "BME680Mng.h"

void BME680_Init()
{
    if (!bme.begin()) 
    {
        Serial.println(F("[BME680]>>[ERROR]:Could not find a valid BME680 sensor, check wiring!"));
        while (1);
    }

    /* Set up oversampling and filter initialization */
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void BME680_MainFunction()
{
    static volatile unsigned long BME680_lastReadTime = 0;
    unsigned long now = millis();

    if (now - BME680_lastReadTime >= BME680_READ_TIME)
    {
        BME680_ReadData();
        g_systemValues.bme.itu = BME680_CalcITU(g_systemValues.bme.temp, g_systemValues.bme.hum);
        g_systemValues.bme.iaq = BME680_CalcIAQ(g_systemValues.bme.RkOhm, g_systemValues.bme.hum);
        BME680_lastReadTime = now;
    }
}

void BME680_ReadData()
{
    unsigned long endTime = bme.beginReading();
    if (endTime == 0) 
    {
        Serial.println(F("[BME680]>>[ERROR]:Failed to begin reading :( "));
        return;
    }

    // Serial.print(F("[BME680]>>[INFO]:Reading started at ")); Serial.print(millis());Serial.print(F(" and will finish at ")); Serial.println(endTime);

    if (!bme.endReading()) 
    {
        Serial.println(F("[BME680]>>[ERROR]:Failed to complete reading :( "));
        return;
    }

    // Serial.print(F("[BME680]>>[INFO]:Reading completed at ")); Serial.println(millis());

    g_systemValues.bme.temp = bme.temperature;
    g_systemValues.bme.hum = bme.humidity;
    g_systemValues.bme.pressure = bme.pressure / 100.0;
    g_systemValues.bme.RkOhm = bme.gas_resistance / 1000.0;
    g_systemValues.bme.alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
}

float BME680_CalcITU(float temp, float hum)
{
    float tempF = (temp * 1.8f) + 32.0f; 
    float itu = tempF - (0.55f - 0.0055f * hum) * (tempF - 58.0f);
    return itu;
}

float BME680_CalcIAQ(float Rgas, float hum)
{
    float gas_reference = 250000.0; // Clean air resistance (250k Ohmi)
    float hum_reference = 40.0;     // Optimal RH (40%)

    //  Gas Score (0-75)
    float gas_score = (Rgas / gas_reference) * 75.0;
    if (gas_score > 75.0)
    {
        gas_score = 75.0;
    }

    // RH Scor (0-25)
    float hum_score;
    if (hum >= hum_reference) 
    {
        hum_score = 0.25 * 100.0 * (100.0 - hum) / (100.0 - hum_reference);
    } 
    else 
    {
        hum_score = 0.25 * 100.0 * (hum) / (hum_reference);
    }

    return gas_score + hum_score; //  Final Score 0-100
}