/* LcdMng.h */

#ifndef LCD_MNG_H
#define LCD_MNG_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "SLL.h"          
#include "TimeMng.h"
#include "WifiMng.h"
#include "MAX471_Hdl.h"
#include "Values.h"  

#define RUNTIME_COUNTER false

#define LCD_ADDR 0x3F
#define LCD_COLS 16
#define LCD_ROWS 2

#define LIVE_UPDATE_MS 500u  /* update live 500ms */

typedef enum
{
    SCREEN_STATUS = 0U,
    SCREEN_MOTOR,
    SCREEN_WIFI_INFO,
    SCREEN_COUNT
} ScreenId;

/*
 * Struct to keep the byte vectors for custom chars
 */
typedef struct {
  byte vmoto[8];
  byte imoto[8];
  byte mqtt[8];
  byte motor[8];
  byte wifi[8];
  byte light[8];
  byte on[8];
  byte off[8];
} CustomCharsByte;

/*
 * Global instance with custom character definitions
 * Note: Not const because lcd.createChar() requires non-const pointer
 */
static CustomCharsByte customCharsByte = {
    // Vmoto (Voltage symbol)
    {  
        0b10001,
        0b10001,
        0b10001,
        0b01010,
        0b10101,
        0b11011,
        0b10101,
        0b10001
    },
    // Imoto (Current symbol)
    {
        0b01110,
        0b00100,
        0b00100,
        0b01110,
        0b10001,
        0b11011,
        0b10101,
        0b10001
    },
    // MQTT symbol
    {  
        0b11100,
        0b11000,
        0b10100,
        0b00010,
        0b01000,
        0b00101,
        0b00011,
        0b00111
    },
    // Motor symbol
    {
        0b10001,
        0b11011,
        0b10101,
        0b10001,
        0b10001,
        0b01110,
        0b11011,
        0b01110
    },
    // WiFi symbol
    {
        0b00000,
        0b00001,
        0b00001,
        0b00101,
        0b00101,
        0b10101,
        0b10101,
        0b10101
    },
    // Light bulb symbol
    {
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b01010,
        0b01110,
        0b01110,
        0b00100
    },
    // ON symbol (checkmark)
    {
        0b00000,
        0b00001,
        0b00011,
        0b10110,
        0b11100,
        0b01000,
        0b00000,
        0b00000
    },
    // OFF symbol (X mark)
    {
        0b00000,
        0b10001,
        0b01010,
        0b00100,
        0b01010,
        0b10001,
        0b00000,
        0b00000
    }
};

/* Public functions */
void LcdMng_Init();
void LcdMng_MainFunction();
void LcdMng_BuildMenu();
void LcdMng_NextScreen();
void LcdMng_RefreshLive();
void LcdMng_SetMotorState(bool state);
void LcdMng_SetLightState(int state);

void LcdMng_SetClockDisplay();
void LcdMng_Set_SCREEN_STATUS();
void LcdMng_Set_SCREEN_MOTOR();
void LcdMng_Set_SCREEN_WIFI_INFO();

extern ScreenId currentScreen;

#endif /* LCD_MNG_H */