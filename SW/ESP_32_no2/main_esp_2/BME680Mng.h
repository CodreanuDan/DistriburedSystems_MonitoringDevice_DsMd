/* BME680Mng.h */

#ifndef BME680_MNG
#define BME680_MNG

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "Values.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define BME680_PRESSURE_DOWNSCALE_FACTOR 100.0f
#define BME680_GAS_RES_DOWNSCALE_FACTOR 1000.0f

#define BME680_READ_TIME 2000U

static Adafruit_BME680 bme;

void BME680_Init();

void BME680_MainFunction();
void BME680_ReadData();

float BME680_CalcITU(float temp, float hum);
float BME680_CalcIAQ(float Rgas, float hum);

#endif // BME680_MNG