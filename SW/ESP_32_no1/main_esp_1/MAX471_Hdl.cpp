/* MAX471_Hdl.cpp */

#include "MAX471_Hdl.h"

volatile float dcHistory_I[MAX471_FILTER_SIZE] = {0};
volatile float dcSum_I = 0;
volatile uint8_t dcIndex_I = 0;

volatile float dcHistory_U[MAX471_FILTER_SIZE] = {0};
volatile float dcSum_U = 0;
volatile uint8_t dcIndex_U = 0;

void Max471_Init(uint8_t max471_pin, uint8_t adcResolution, uint8_t adcAttenuation)
{
    memset((void*)dcHistory_I, 0, sizeof(dcHistory_I));
    dcSum_I = 0;
    dcIndex_I = 0;

    memset((void*)dcHistory_U, 0, sizeof(dcHistory_U));
    dcSum_U = 0;
    dcIndex_U = 0;

    /* Mark pin as input */
    pinMode(max471_pin, INPUT);
    /* Set the ADC resolution to 12 bits */
    analogReadResolution(adcResolution);
    /* Set the ADC attenuation  for the MAX471 sensor pin
    ADC_0db:   ~~> 1.1V
    ADC_2_5db: ~~> 1.5V
    ADC_6db:   ~~> 2.2V
    ADC_11db:  ~~> 3.3V */
    /* {!!!} use static cast for adcAttenuation beacuse the func expects elements from adc_attenuation_t enum */
    analogSetPinAttenuation(max471_pin, static_cast<adc_attenuation_t>(adcAttenuation));
}

void Max471_Hdl()
{
    Max471_MeasureCurrent();
}

void Max471_MeasureCurrent()
{
    static unsigned long Max471_LastMeasureTime_ms = 0;
    unsigned long now = millis();

    if (now - Max471_LastMeasureTime_ms >= Max471_MeasureInterval_ms)
    {
        /* Read raw adc value */
        uint32_t rawSum = 0;
        for(int i = 0; i < MAX471_OVERSAMPLING; i++) 
        {
            rawSum += analogRead(MAX_471_PIN);
            ets_delay_us(10); 
        }
        g_systemValues.current.raw_adc = rawSum / MAX471_OVERSAMPLING;
        /* Convert raw adc val in voltage */
        float voltage = (g_systemValues.current.raw_adc / ADC_MAX_VAL) * VREF;
        /* Convert read voltage in current: 1V -> 1A and apply mov.avg filter*/
        float IL_unfilt = voltage * IL_LIN_FACT;
        float IL_filt = Max471_MovingAverageHelper(dcHistory_I, MAX471_FILTER_SIZE, &dcIndex_I, IL_unfilt, &dcSum_I);
        /* Convert read voltage in volatge using Ohm Law U = I * Rmoto and apply mov.avg filter */
        float UL_unfilt = voltage * R_MOTOR_OHM;
        float UL_filt = Max471_MovingAverageHelper(dcHistory_U, MAX471_FILTER_SIZE, &dcIndex_U, UL_unfilt, &dcSum_U);
        
        g_systemValues.current.raw_voltage  = voltage;
        g_systemValues.current.current_A    = IL_filt;
        g_systemValues.current.voltage_V    = UL_filt;
        g_systemValues.current.power_W      = IL_filt * UL_filt;

        //Max471_DebugFunction(now, raw_val, voltage, IL_filt, UL_filt);

        Max471_LastMeasureTime_ms = now;
    }
}

void Max471_DebugFunction(unsigned long currentTime, float adcValue, float voltage, float current_A, float Uload)
{
      Serial.print("dT:");    Serial.print(currentTime / 1000.0f, 1);   Serial.print(" s ");
      Serial.print(" ADC: ");  Serial.print(adcValue);                   Serial.print(" ");
      Serial.print(" Vout: "); Serial.print(voltage, 3);                 Serial.print(" V ");
      Serial.print(" IL: ");   Serial.print(current_A, 3);               Serial.print(" A");
      Serial.print(" UL: ");   Serial.print(Uload, 3); Serial.print(" V");
      Serial.println();
}

float Max471_MovingAverageHelper(volatile float* buffer, uint8_t size, volatile uint8_t* index, float new_val, volatile float* sum)
{
    *sum -= buffer[*index];
    *sum += new_val;

    buffer[*index] = new_val;

    *index = (*index + 1) % size;
    return *sum / size;
}