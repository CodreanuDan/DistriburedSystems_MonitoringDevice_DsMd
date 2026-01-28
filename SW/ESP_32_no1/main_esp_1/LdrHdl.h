/* LdrHdl.h */

#ifndef LDR_HDL_H
#define LDR_HDL_H

#include <Arduino.h>
#include "Values.h"

#define LDR_PIN 23
#define LdrHdl_MeasureInterval_ms 500UL

void LdrHdl_Init();
void LdrHdl_MainFunction();

#endif // LDR_HDL_H