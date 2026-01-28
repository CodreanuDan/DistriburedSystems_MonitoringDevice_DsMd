/* BtnMng.cpp */

#include "BtnMng.h"

volatile unsigned long BtnMng_lastChangeTime = 0;
ButtonStates buttonState = BUTTON_RELEASED;

void BtnMng_MainFunction(uint8_t pin, unsigned long debounceTime)
{
    ButtonStates detectedState = BUTTON_RELEASED;
    byte current_reading = digitalRead(pin);
    unsigned long now = millis();

    switch(buttonState)
    {
        case BUTTON_RELEASED:
            /* Check if we detect a button press */
            detectedState = BtnMng_HdlReleasedState(current_reading, debounceTime, BtnMng_lastChangeTime, now);
            buttonState = detectedState;
            break;
        case BUTTON_PRESS_PENDING:
            /* If we detected a press and the time interval is ok validate pressing */
            detectedState = BtnMng_HdlPressPendingState(current_reading, debounceTime, BtnMng_lastChangeTime, now);
            buttonState = detectedState;
            break;
        case BUTTON_PRESSED:
            /* Check if we detect a button release */
            detectedState = BtnMng_HdlPressedState(current_reading, debounceTime, BtnMng_lastChangeTime, now);
            buttonState = detectedState;
            break;
        case BUTTON_RELEASE_PENDING:
            /* If we detected a press and the time interval is pressed validate pressing */
            detectedState = BtnMng_HdlReleasePendingState(current_reading, debounceTime, BtnMng_lastChangeTime, now);
            buttonState = detectedState;
            break;
        default:
            buttonState = BUTTON_RELEASED;
            break;
    }

    g_systemValues.button.buttonState = buttonState;
}

ButtonStates BtnMng_HdlReleasedState(byte level, unsigned long debounceTime, unsigned long lastChangeTime, unsigned long now)
{
    ButtonStates currentState = BUTTON_RELEASED;

    if (level == BUTTON_PRESSED_LEVEL)
    {
        currentState = BUTTON_PRESS_PENDING;
        BtnMng_lastChangeTime = now;
    }

    return currentState;
}

ButtonStates BtnMng_HdlPressPendingState(byte level, unsigned long debounceTime, unsigned long lastChangeTime, unsigned long now)
{
    ButtonStates currentState = BUTTON_PRESS_PENDING;

    if (now - lastChangeTime >= debounceTime)
    {
        if (level == BUTTON_PRESSED_LEVEL)
        {
            currentState = BUTTON_PRESSED;
            Serial.println("Button pressed: Go to next menu page...");
        }
    }
    else
    {
        currentState = BUTTON_RELEASED;
    }

    return currentState;
}

ButtonStates BtnMng_HdlPressedState(byte level, unsigned long debounceTime, unsigned long lastChangeTime, unsigned long now)
{
    ButtonStates currentState = BUTTON_PRESSED;

    if (level == BUTTON_RELEASED_LEVEL)
    {
        currentState = BUTTON_RELEASE_PENDING;
        BtnMng_lastChangeTime = now;
    }

    return currentState;
}

ButtonStates BtnMng_HdlReleasePendingState(byte level, unsigned long debounceTime, unsigned long lastChangeTime, unsigned long now)
{
    ButtonStates currentState = BUTTON_PRESSED;

    if (now - lastChangeTime >= debounceTime)
    {
        if (level == BUTTON_RELEASED_LEVEL)
        {
          currentState = BUTTON_RELEASED;
        }
    }
    else
    {
        currentState = BUTTON_PRESSED;
    }

    return currentState;
}
