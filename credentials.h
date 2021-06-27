/*
 * Settings for the Calendar Display
 *
 * Timezone: 
 * https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
 */

/* WiFi credentials */
const char* ssid            = "";                                      // WiFi SSID
const char* password        = "";                                      // WiFi password

/* Calendar */
String calkey               = "";                                      // Key to access calendar data
const char calserver[]      = "";                                      // Calendar Server URL

/* Open Weathermap Account */
String apikey               = "";                                      // Get the developer key for 1000 request/day
const char server[]         = "api.openweathermap.org";                // URL (don't change)

/* API Parameters Open Weathermap */
const char* zip             = "";                                      // Zip Code and country code
const char* lang            = "en";                                    // language code
const char* units           = "imperial";                              // units (imperial or metric)

/* Time and NTP Server */
const char* ntpServer       = "pool.ntp.org";                          // URL address
const char* timezoneStr     = "EST5EDT,M3.2.0/2,M11.1.0";              // Timezone Definition (https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv)
int gmtOffset_sec           = -14400;                                  // Timezone offset of your location in seconds -18000;

/* Sleep Time in Minutes */
const long SleepDuration    = 30;                                      // Display update interval in minutes
const int PowerSaveStart    = 22;                                      // Start power save where display is not updated (24 hour format)
const int PowerSaveEnd      =  5;                                      // End power save where display is not updated (24 hour format)

/* Notification sounds */
const bool playAlarm        = true;                                    // Play Calendar Alarms
const bool playBday         =  true;                                   // Play Birthday Notifications (true/false)
const int playBdayTime      =  10;                                     // Play Birthday Notification at full hour (24 hour format)
