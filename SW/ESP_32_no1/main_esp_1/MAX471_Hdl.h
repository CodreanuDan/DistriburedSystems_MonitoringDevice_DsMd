/* MAX471_Hdl.h */

#ifndef MAX471_HDL
#define MAX471_HDL

#include <Arduino.h>
#include <stdlib.h>
#include "Values.h"

#define MAX_471_PIN 34u      
#define IL_LIN_FACT 1.0f        
#define R_MOTOR_OHM 6.15f
#define Max471_MeasureInterval_ms 500UL

#define VREF 3.3f
#define ADC_MAX_VAL 4095.0f
#define ADC_RESOLUTION 12u
#define MAX471_FILTER_SIZE 10u

#define MAX471_OVERSAMPLING 100u

extern volatile float dcHistory_I[MAX471_FILTER_SIZE];
extern volatile float dcSum_I;
extern volatile uint8_t dcIndex_I;

extern volatile float dcHistory_U[MAX471_FILTER_SIZE];
extern volatile float dcSum_U;
extern volatile uint8_t dcIndex_U;

void Max471_Init(uint8_t max471_pin, uint8_t adcResolution, uint8_t adcAttenuation);
void Max471_Hdl();
void Max471_MeasureCurrent();
float Max471_MovingAverageHelper(volatile float* buffer, uint8_t size, volatile uint8_t* index, float new_val, volatile float* sum);
void Max471_DebugFunction(unsigned long currentTime, float adcValue, float voltage, float current_A, float Uload);

#endif // MAX471_HDL