/* LedCtrl.h */

#ifndef LED_CTRL_H
#define LED_CTRL_H

#include <Arduino.h>
#include "Values.h"

#define LED_PIN_PWM 33

#define PWM_FULL 255U
#define PWM_25           ((uint8_t)(0.25f * PWM_FULL))
#define PWM_50           ((uint8_t)(0.50f * PWM_FULL))
#define PWM_75           ((uint8_t)(0.75f * PWM_FULL))

#define LED_DIM_TIME 1000U

void LedCtrl_Init();

void LedCtrl_MainFunction();
void LedCtrl_ApplyDutyCycle(uint8_t targetDuty);
void LedCtrl_CyclicDimmer();

#endif // LED_CTRL_H