/* LcdMng.cpp */

#include "LcdMenuMng.h"

extern LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

SinglyLinkedListMenu* menuList = NULL;
ScreenId currentScreen = SCREEN_STATUS;

unsigned long lastLiveUpdate = 0;

static bool actionDone = false;

void LcdMng_Init()
{
    lcd.init();
    lcd.backlight();
    lcd.clear();

    /* Cusotm chars */
    lcd.createChar(0, customCharsByte.vmoto);   // Vmoto
    lcd.createChar(1, customCharsByte.imoto);   // Imoto
    lcd.createChar(2, customCharsByte.mqtt);    // Mqtt
    lcd.createChar(3, customCharsByte.motor);   // Motor
    lcd.createChar(4, customCharsByte.wifi);    // Wifi
    lcd.createChar(5, customCharsByte.light);   // Light
    lcd.createChar(6, customCharsByte.on);      // ON (checkmark)
    lcd.createChar(7, customCharsByte.off);     // OFF (X mark)
  
    pinMode(BTN_PIN, INPUT_PULLUP);  

    LcdMng_BuildMenu();
    LcdMng_RefreshLive(); 
}

void LcdMng_BuildMenu()
{
    menuList = sll_create();

    /* Node 1 - Status  */
    MenuNode n1;
    memset(&n1, 0, sizeof(MenuNode));
    strncpy(n1.text, "Status Screen", sizeof(n1.text) - 1);
    n1.text[sizeof(n1.text) - 1] = '\0';
    sll_push_back(menuList, n1);

    /* Node 2 - Motor */
    MenuNode n2;
    memset(&n2, 0, sizeof(MenuNode));
    strncpy(n2.text, "Motor Screen", sizeof(n2.text) - 1);
    n2.text[sizeof(n2.text) - 1] = '\0';
    sll_push_back(menuList, n2);

    /* Node 3 - WiFi Info */
    MenuNode n3;
    memset(&n3, 0, sizeof(MenuNode));
    strncpy(n3.text, "WiFi Screen", sizeof(n3.text) - 1);
    n3.text[sizeof(n3.text) - 1] = '\0';
    sll_push_back(menuList, n3);
}

void LcdMng_MainFunction()
{
    unsigned long now = millis();

    /* Update live  500ms */
    if (now - lastLiveUpdate >= LIVE_UPDATE_MS)
    {
        lastLiveUpdate = now;
        LcdMng_RefreshLive();
    }

    /* Check button state g_systemValues ( BtnMng) */
    if (g_systemValues.button.buttonState == BUTTON_PRESSED)
    {
        if (!actionDone)
        {
            LcdMng_NextScreen();
            Serial.println("Next screen (buttonState)");
            actionDone = true;  /* block the action while the button is presssed */
        }
    }
    else
    {
        /* Reset when button is released */
        actionDone = false;
    }
}

void LcdMng_NextScreen()
{
    currentScreen = static_cast<ScreenId>((currentScreen + 1) % SCREEN_COUNT);
    LcdMng_RefreshLive();
}

void LcdMng_RefreshLive()
{
    lcd.clear();

    MenuNode* node = sll_get_at(menuList, currentScreen);
    if (node == NULL)
    {
        lcd.setCursor(0, 0);
        lcd.print("No menu");
        return;
    }

    switch (currentScreen)
    {
        case SCREEN_STATUS:
        {
            LcdMng_Set_SCREEN_STATUS();
            break;
        }
        case SCREEN_MOTOR:
        {
            LcdMng_Set_SCREEN_MOTOR();
            break;
        }
        case SCREEN_WIFI_INFO:
        {
            LcdMng_Set_SCREEN_WIFI_INFO();
            break;
        }
        case SCREEN_COUNT:
            break;
    }
}

void LcdMng_SetClockDisplay()
{
    #if RUNTIME_COUNTER
        unsigned long totalSec = millis() / 1000;
        int hours = (totalSec / 3600) % 24;
        int minutes = (totalSec / 60) % 60;
        int seconds = totalSec % 60;
        char timeStr[9];
        snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
        lcd.print(timeStr);
    #else
        if (g_systemValues.wifi.wifi_connected == true && ntpReady == true)
        {
            String timestamp = TimeMng_GetTimestamp();      // timestamp
            String timeOnly = timestamp.substring(11, 19);  // HH:MM:SS 
            lcd.print("T:");
            lcd.print(timeOnly);
        }
        else
        {
            unsigned long totalSec = millis() / 1000;
            int hours = (totalSec / 3600) % 24;
            int minutes = (totalSec / 60) % 60;
            int seconds = totalSec % 60;
            char timeStr[9];
            snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
            lcd.print(timeStr); 
        }
    #endif
}

void LcdMng_Set_SCREEN_STATUS()
{
    /*  Row 1: WiFi icon, status, time (hh:mm:ss) */
    lcd.setCursor(0, 0);
    lcd.write(4);  // WiFi icon
    lcd.print(":");
    lcd.write(g_systemValues.wifi.wifi_connected ? 6 : 7);  // ✓ or ✗
    lcd.print(" ");
    LcdMng_SetClockDisplay();

    /*  Row 2: Light, Motor, MQTT status */
    lcd.setCursor(0, 1);
    lcd.write(5);  // Light bulb icon
    lcd.print(":");
    lcd.write(g_systemValues.light.level ? 7 : 6);  // ✗ or ✓
    lcd.print(" ");
    
    lcd.write(2);  // MQTT icon
    lcd.print(":");
    lcd.write(g_systemValues.wifi.mqtt_connected ? 6 : 7);  
    lcd.print(" ");

    lcd.write(3);  // Motor icon
    lcd.print(":");
    lcd.write(g_systemValues.motor.running ? 6 : 7);  // ✓ or ✗
    lcd.print(":");
    lcd.print(g_systemValues.mqtt.overdirve ? "MAN" : "AUT");
    
}

void LcdMng_Set_SCREEN_MOTOR()
{
    /*  Row 1: Motor icon, Power */
    lcd.setCursor(0, 0);
    lcd.write(3);  // Motor icon
    lcd.print("> P:");
    
    // Power  and Duty Cycle
    lcd.print(g_systemValues.current.power_W, 1);
    lcd.print("W");

    lcd.print(" D:");
    lcd.print(g_systemValues.motor.pwm);
    lcd.print("%");

    /*  Row 2: Voltage and Current */
    lcd.setCursor(0, 1);
    lcd.write(0);  // Uload icon (Vmoto)
    lcd.print(":");
    lcd.print(g_systemValues.current.voltage_V, 2);
    lcd.print("V ");
    
    lcd.write(1);  // Iload icon (Imoto)
    lcd.print(":");
    lcd.print(g_systemValues.current.current_A, 2);
    lcd.print("A");

}

void LcdMng_Set_SCREEN_WIFI_INFO()
{
    /*  Helper buffer for volatile char array conversion */
    char ipBuffer[17];

    /*  Row 1: WiFi icon, RSSI */
    lcd.setCursor(0, 0);
    
    lcd.write(4);  // WiFi icon
    lcd.print("> RSSI:");
    lcd.print(g_systemValues.wifi.rssi);
    lcd.print("dBm");

    /* Row 2: IP address */
    lcd.setCursor(0, 1);
    lcd.print("IP:");
    
    /* Copy volatile char array to non-volatile buffer Use noInterrupts() to safely read volatile data */
    noInterrupts();
    strncpy(ipBuffer, (const char*)g_systemValues.wifi.ip, sizeof(ipBuffer) - 1);
    interrupts();
    ipBuffer[sizeof(ipBuffer) - 1] = '\0';
    lcd.print(ipBuffer);
}