/* BtnMng.h */

#ifndef BTN_MNG_H
#define BTN_MNG_H

#include <Arduino.h>
#include "Values.h"

#define BTN_PIN 4

#define BUTTON_PRESSED_LEVEL  0x0 /* LOW */
#define BUTTON_RELEASED_LEVEL 0x1 /* HIGH */

#define BtnMng_debounceTime 10UL
extern volatile unsigned long BtnMng_lastChangeTime;

typedef enum
{
    BUTTON_RELEASED = 0U,
    BUTTON_PRESS_PENDING,
    BUTTON_PRESSED,
    BUTTON_RELEASE_PENDING
} ButtonStates;

void BtnMng_MainFunction(uint8_t pin, unsigned long debounceTime);
ButtonStates BtnMng_HdlReleasedState(byte level, unsigned long debounceTime, unsigned long lastChangeTime, unsigned long now);
ButtonStates BtnMng_HdlPressPendingState(byte level, unsigned long debounceTime, unsigned long lastChangeTime, unsigned long now);
ButtonStates BtnMng_HdlPressedState(byte level, unsigned long debounceTime, unsigned long lastChangeTime, unsigned long now);
ButtonStates BtnMng_HdlReleasePendingState(byte level, unsigned long debounceTime, unsigned long lastChangeTime, unsigned long now);


#endif // BTN_MNG_H

