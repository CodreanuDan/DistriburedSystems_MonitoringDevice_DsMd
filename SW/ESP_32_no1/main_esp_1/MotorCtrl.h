/* MotorCtrl.h */

#ifndef MOTOR_MNG_H
#define MOTOR_MNG_H

#include <Arduino.h>
#include <stdlib.h>
#include "Values.h"

#define R_MOTO 1.15f

#define MOTOR_PWM_PIN    5
#define MOTOR_PWM_FREQ   1000
#define MOTOR_PWM_RES    8

#define PWM_FULL         255
#define PWM_25           ((uint8_t)(0.25f * PWM_FULL))
#define PWM_45           ((uint8_t)(0.45f * PWM_FULL))
#define PWM_50           ((uint8_t)(0.50f * PWM_FULL))
#define PWM_55           ((uint8_t)(0.55f * PWM_FULL))
#define PWM_60           ((uint8_t)(0.60f * PWM_FULL))
#define PWM_75           ((uint8_t)(0.75f * PWM_FULL))

const float MotorCtrl_KickStartSequence[] = {
    0.99f, 0.98f, 0.97f, 0.96f, 0.95f, 0.94f, 0.93f, 0.92f, 0.91f, 0.90f,
    0.89f, 0.88f, 0.87f, 0.86f, 0.85f, 0.84f, 0.83f, 0.82f, 0.81f, 0.80f,
    0.79f, 0.78f, 0.77f, 0.76f, 0.75f, 0.74f, 0.73f, 0.72f, 0.71f, 0.70f,
    0.69f, 0.68f, 0.67f, 0.66f, 0.65f, 0.64f, 0.63f, 0.62f, 0.61f, 0.60f,
    0.59f, 0.58f, 0.57f, 0.56f, 0.55f, 0.54f, 0.53f, 0.52f, 0.51f, 0.50f,
    0.49f, 0.48f, 0.47f, 0.46f, 0.45f, 0.44f, 0.43f, 0.42f, 0.41f, 0.40f,
    0.39f, 0.38f, 0.37f, 0.36f, 0.35f, 0.34f, 0.33f, 0.32f, 0.31f, 0.30f,
    0.29f, 0.28f, 0.27f, 0.26f, 0.25f
};
const uint8_t MotorCtrl_KickStartSequence_Size = sizeof(MotorCtrl_KickStartSequence) / sizeof(MotorCtrl_KickStartSequence[0]);

#define BOOST_TIME_MS    100
#define HOLD_TIME_MS     400

typedef enum
{
    MOTOR_PHASE_BOOST = 0,
    MOTOR_PHASE_HOLD
} MotorPhase_t;

static MotorPhase_t motorPhase = MOTOR_PHASE_BOOST;
static uint32_t motorPhaseTimestamp = 0;

void MotorCtrl_Init();
void MotorCtrl_MainFunction();
void MotorCtrl_ApplyCyclicPWM(uint8_t targetDuty);
void MotorCtrl_ApplyPWM(uint8_t targetDuty);
void MotorCtrl_TestFunction(void);
void MotorCtrl_ResetCycle(void);
void MotorCtrl_CheckMotorStat(uint8_t targetDuty);
void MotorCtrl_ProcessRPM();
void MotorCtrl_Kickstart(uint8_t targetDuty);
void MotorCtrl_Auto();

#endif