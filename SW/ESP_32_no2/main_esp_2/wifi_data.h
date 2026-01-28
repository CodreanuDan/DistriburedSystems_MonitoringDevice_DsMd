/* wifi_data.h */

#include <stdint.h>

#ifndef WIFI_DATA_H
#define WIFI_DATA_H

typedef struct {
  char wifi_ssid[33];
  char wifi_password[65];
  uint8_t priority;
} WifiCredentials;



#endif // WIFI_DATA_H