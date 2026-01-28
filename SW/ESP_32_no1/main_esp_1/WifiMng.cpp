/* WifiMng.cpp*/

#include "WifiMng.h"

Vector* knownNetworks = NULL;

unsigned long last_check_time_ms = 0;

const char* staticNetworks[][2] = {
    {"TP-Link_47D4", "29393145"},
    {"iPhone_Dan", "12345678"},
    {"CDIDC", "SkodaOctavia2005"}
};

const uint8_t NUM_STATIC_NETWORKS = sizeof(staticNetworks) / sizeof(staticNetworks[0]);

void WifiMng_LoadNetworksInVector()
{
    if (knownNetworks != NULL)
    {
        vector_clear(knownNetworks);   
        free(knownNetworks);           
        knownNetworks = NULL;
    }
    else
    {
        knownNetworks = vector_create();
        if (knownNetworks == NULL)
        {
            //Serial.println("[ERROR] WifiMng_LoadNetworksInVector: NO VECTOR CREATED");
            return;
        }
    }

    //Serial.println("[INFO] Add networks in vector...");

    for (uint8_t i = 0; i < NUM_STATIC_NETWORKS; i++)
    {
        WifiCredentials cred = {0};

        strncpy(cred.wifi_ssid, staticNetworks[i][0], sizeof(cred.wifi_ssid) - 1);
        strncpy(cred.wifi_password, staticNetworks[i][1], sizeof(cred.wifi_password) - 1);

        cred.wifi_ssid[sizeof(cred.wifi_ssid) - 1] = '\0';
        cred.wifi_password[sizeof(cred.wifi_password) - 1] = '\0';
        cred.priority = i + 1;

        //Serial.print("Adding: "); Serial.println(cred.wifi_ssid);
        vector_push_back(knownNetworks, cred);
        //Serial.print("[DEBUG PUSH] SSID in vector: ");
        //Serial.println(knownNetworks->data[knownNetworks->size - 1].wifi_ssid);
    }


    //Serial.println("[INFO] Vector:");
    //printVectorContents();
}

bool WifiMng_ConnectToNetwork()
{

    Serial.println("[WIFI] Connect without SCAN.");

    if (knownNetworks == NULL || vector_is_empty(knownNetworks))
    {
        //Serial.println("[ERROR] WifiMng_ConnectToNetwork: VECTOR PROBLEM");
        return false;
    }

    //Serial.println("[DEBUG] Vector before connect:");
    //printVectorContents();

    //Serial.println("[INFO] Try to connect using saved wifi credentials");

    bool wifi_connected = false;
    size_t initial_size = knownNetworks->size;

    for (uint8_t i = 0; i < initial_size; i++)
    {
        WifiCredentials current = vector_pop_front(knownNetworks);
        //WifiCredentials current = knownNetworks->data[i];
        //Serial.print("TRY: "); Serial.print(current.wifi_ssid); Serial.print(" ... ");

        WiFi.begin(current.wifi_ssid, current.wifi_password);

        unsigned start_attempt = millis();
        while (millis() - start_attempt < WIFI_CONNECTION_TIMEOUT_MS)
        {
            if (WiFi.status() == WL_CONNECTED)
            {
                //Serial.println(" → CONNECTED!");
                //Serial.print("IP: "); Serial.println(WiFi.localIP());
                wifi_connected = true;

                // Set NTP ready after 1 after wifi connect
                wifiConnectTime = millis();
                ntpReady = false;  

                break;
            }

            delay(400);
            //Serial.print(".");
        }

        if (wifi_connected == true)
        {
            break;
        }
        else
        {
            //Serial.println(" CONNECTION FAILED !.");
        }
    
    }

    if (!wifi_connected) 
    {
        //Serial.println("[ERROR] WifiMng_ConnectToNetwork: NO NETWORK IS WORKING!");
    }

    return wifi_connected;
}

bool WifiMng_ScanAndConnectToNetwork()
{

    Serial.println("[WIFI] Connect with SCAN.");

    int numNetworks = WiFi.scanNetworks();
    if (numNetworks == -1) 
    {
        return false;
    }

    if (knownNetworks == NULL || vector_is_empty(knownNetworks))
    {
        return false;
    }

    bool wifi_connected = false;
    size_t initial_size = knownNetworks->size;

    for (uint8_t i = 0; i < initial_size; i++)
    {
        WifiCredentials current = vector_pop_front(knownNetworks);
        Serial.print("TRY: "); Serial.print(current.wifi_ssid); Serial.print(" ... ");
        const char* target_ssid  = current.wifi_ssid;
 
        for (uint8_t j = 0; j < numNetworks; j++)
        {
            String scanned_ssid = WiFi.SSID(j);

            if (scanned_ssid == target_ssid)
            {
                Serial.print("[MATCH] Găsită rețea cunoscută: "); Serial.print(target_ssid);
                Serial.print(" (RSSI: "); Serial.print(WiFi.RSSI(j)); Serial.println(" dBm)");

                WiFi.begin(target_ssid, current.wifi_password);

                unsigned start_attempt = millis();
                while (millis() - start_attempt < WIFI_CONNECTION_TIMEOUT_MS)
                {
                    if (WiFi.status() == WL_CONNECTED)
                    {
                        Serial.println(" → CONNECTED!");
                        Serial.print("IP: "); Serial.println(WiFi.localIP());
                        wifi_connected = true;

                        // Set NTP ready after 1 after wifi connect
                        wifiConnectTime = millis();
                        ntpReady = false;  

                        break;
                    }

                    delay(400);
                    Serial.print(".");
                }

                if (wifi_connected == true) break;

            }
        }

        String ipStr = WiFi.localIP().toString();
        strncpy((char*)g_systemValues.wifi.ip, ipStr.c_str(), sizeof(g_systemValues.wifi.ip) - 1);
        g_systemValues.wifi.ip[sizeof(g_systemValues.wifi.ip) - 1] = '\0';
        g_systemValues.wifi.rssi = WiFi.RSSI();
        g_systemValues.wifi.wifi_connected = (WiFi.status() == WL_CONNECTED);
        
        if (wifi_connected == true) break;
    }

    WiFi.scanDelete();
    return wifi_connected;
}

void WifiMng_MainFunction()
{
    unsigned long now = millis();

    if (now - last_check_time_ms >= check_interval_ms) 
    {
        last_check_time_ms = now;

        if (WiFi.status() == WL_CONNECTED) 
        {
            //Serial.println("[OK] ESP32 connected to  WiFi");

            // Check if 1 second passed after connect to set NTP
            if (!ntpReady && (now - wifiConnectTime >= 1000UL)) 
            {
                ntpReady = true;
                Serial.println(">>> [TIME] NTP ready after WiFi connect!");
            }
        } 
        else 
        {
            ntpReady = false;  
            //Serial.println("[WARN] Connection lost, try reconnect...");
            if (knownNetworks && !vector_is_empty(knownNetworks)) 
            {
                #if ENABLE_SCAN_AND_CONNECT
                    WifiMng_ScanAndConnectToNetwork();
                #else
                    WifiMng_ConnectToNetwork();
                #endif
            } 
            else 
            {
                // Serial.println("Empty vector, no networks to try.");
            }
        }
    }
}