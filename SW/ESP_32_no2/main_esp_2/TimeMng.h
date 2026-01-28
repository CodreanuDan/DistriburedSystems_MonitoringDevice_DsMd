/* TimeMng.h */

#ifndef TIME_MNG_H
#define TIME_MNG_H

#include <Arduino.h>
#include <time.h>  
#include "Values.h"

#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 7200     /* Romania GMT+2 */
#define DAYLIGHT_OFFSET_SEC 3600 /* DST +1h */

/* Init NTP (se sincronizeaza automat cand ai WiFi) */
void TimeMng_Init();

/* Timestamp  String "YYYY-MM-DD HH:MM:SS" */
String TimeMng_GetTimestamp();

#endif /* TIME_MNG_H */