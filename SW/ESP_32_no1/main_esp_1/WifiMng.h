/* WifiMng.h */

#ifndef WIFI_MNG_H
#define WIFI_MNG_H

#include <WiFi.h>
#include <stdint.h>
#include "vector.h"
#include "wifi_data.h"
#include "Values.h"

#define ENABLE_SCAN_AND_CONNECT true
#define WIFI_CONNECTION_TIMEOUT_MS 10000u

extern Vector* knownNetworks;
extern unsigned long last_check_time_ms;
const unsigned long check_interval_ms = 5000UL; 
extern const char* staticNetworks[][2];
extern const uint8_t NUM_STATIC_NETWORKS;

void WifiMng_MainFunction();
void WifiMng_LoadNetworksInVector();
bool WifiMng_ConnectToNetwork();
bool WifiMng_ScanAndConnectToNetwork();

#endif // WIFI_MNG_H