/*
 * Drawing function for the calendars top bar
 * 
 * display  display class
 * 
 */

#include <GxEPD2_GFX.h>
#include <U8g2_for_Adafruit_GFX.h>
#include "RTClib.h"
#include "structures.h"

void sideBar(GxEPD2_GFX& display, U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, RTC_PCF8523& rtc, Weather_type* weather );