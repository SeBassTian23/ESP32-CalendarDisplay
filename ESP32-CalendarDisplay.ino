/*
 * Weather display
 * 
 * Version: see library.properties 
 * 
 * Controller (Driver):
 *  - ESP32: https://www.adafruit.com/product/3405
 * 
 * Display:
 *  - Display: https://www.waveshare.com/wiki/7.5inch_HD_e-Paper_HAT_(B)
 *  - Driver HAT: https://www.waveshare.com/product/displays/e-paper/driver-boards/e-paper-driver-hat.htm
 *  - Driver: https://github.com/ZinggJM/GxEPD2 (Jean-Marc Zingg) Version 1.2.3
 * 
 * Other:
 *  - ArduinoJson: https://arduinojson.org/  V6.x or above
 *  - WiFi: Arduino IDE
 *  - HTTPClient: Arduino IDE
 *  - WiFiClientSecure: Arduino IDE
 * 
 */

/* enable or disable GxEPD2_GFX base class */
#define ENABLE_GxEPD2_GFX 1

/* Display Dependencies */
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>

#define MAX_DISPLAY_BUFFER_SIZE 800
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))

/* Connections for Adafruit Feather */
static const uint8_t EPD_BUSY = 32; // to EPD BUSY
static const uint8_t EPD_CS   = 15; // to EPD CS
static const uint8_t EPD_RST  = 27; // to EPD RST
static const uint8_t EPD_DC   = 33; // to EPD DC
static const uint8_t EPD_SCK  =  5; // to EPD CLK
static const uint8_t EPD_MISO = 19; // Master-In Slave-Out not used, as no data from display
static const uint8_t EPD_MOSI = 18; // to EPD DIN

/* Mapping of Waveshare ESP32 Driver Board */
GxEPD2_3C < GxEPD2_750c_Z90, GxEPD2_750c_Z90::HEIGHT / 4 > display(GxEPD2_750c_Z90(/*CS=D8*/ EPD_CS, /*DC=D3*/ EPD_DC, /*RST=D4*/ EPD_RST, /*BUSY=D2*/ EPD_BUSY)); // GDEH075Z90 880x528

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

/* WiFi dependencies */
#include "credentials.h"  
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

/* JSON Parsing */
#include <ArduinoJson.h>

/* Realtime Clock */
#include "RTClib.h"
RTC_PCF8523 rtc;

/* Piezzo Buzzer */
int GPIO_12 = 12;
#include "src/buzzer.h"

/* Structures */
#include "src/structures.h"
#define max_events 10
Event_type Events[max_events];
Weather_type Weather[1];

/* Variables */
#include "src/common.h"
bool hasBirthday = false;       // Day has a birthday
bool hasNotification = false;   // Notification after refresh

/* Inital value for RTC memory */
RTC_DATA_ATTR int playAlarmType = 0;
RTC_DATA_ATTR bool ntp_update = false;
RTC_DATA_ATTR int ntp_last_update = 0;

/* UI dependencies */
#include "src/sidebar.h"
#include "src/events.h" 

/* Setup */
void setup()
{
  // Serial Port
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setup");

  // Setup RTC
  pinMode(GPIO_12, OUTPUT);

  // Check if the RTC PCF8523 is available
  if (!rtc.begin())
  {
      Serial.println("Error: RTC PCF8523 not found.");
      while (1);
  }

  // Set the clock's time if not initialized
  if (!rtc.initialized() || rtc.lostPower())
  {
      Serial.println("Warning: RTC needs to be initialized");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      ntp_update = true;
  }

  // Display
  display.init(115200);
  display.setRotation(0);
  display.setFullWindow();
  u8g2Fonts.begin(display); 

  // Current time
  DateTime now = rtc.now();
  showDate(now);

  // Check if time needs to be synced
  if(now.hour() < ntp_last_update){
    // Trigger update
    ntp_update = true;
  }
  // Set hours since last update
  ntp_last_update = now.hour();

  Serial.print("NPT hour: ");
  Serial.println( ntp_last_update );

  // Time and Timezone
  getCalendarData();

  // Set refresh timer
  long refresh = 0;

  // Build the View
  display.firstPage();
  do
  {
    // Sidebar
    sideBar(display, u8g2Fonts, rtc, Weather);

    // Event List
    refresh = eventList(display, u8g2Fonts, rtc, Events);
  }
  while (display.nextPage());

  // Play Notification sound
  if(playAlarmType == 1)
  {
    playNotification();
    playAlarmType = 0;
    Serial.println(F("Play Notification Alert"));
  }
  else if(playAlarmType == 2 )
  {
    playHappyBirthday();
    playAlarmType = 0;
    Serial.println(F("Play B-Day Alert"));
  }
  else if(playAlarmType == 3 )
  {
    playNotification();
    playHappyBirthday();
    playAlarmType = 0;
    Serial.println(F("Play Both Alerts"));
  }

  // Put ESP32 into sleep mode
  espSLEEP(refresh);
}

/* Program Loop */
void loop()
{
}

/* Start WiFi and Connect to Network */
uint8_t StartWiFi()
{
  Serial.print("\r\nConnecting to: "); Serial.println(String(ssid));
  // IPAddress dns(8, 8, 8, 8); // Google DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  unsigned long start = millis();
  uint8_t connectionStatus;
  bool AttemptConnection = true;
  while (AttemptConnection) {
    connectionStatus = WiFi.status();
    if (millis() > start + 15000) { // Wait 15-secs maximum
      AttemptConnection = false;
    }
    if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED) {
      AttemptConnection = false;
    }
    delay(50);
  }
  if (connectionStatus == WL_CONNECTED) {
    Serial.println("WiFi connected at: " + WiFi.localIP().toString());

    if( ntp_update ){

      Serial.println("Current Time from RTC");
      showDate( rtc.now() );

      Serial.println("Start NTP Server Update");
      configTime(0, 0, ntpServer);
      delay(2000);

      Serial.println("Updated Time from ESP");

      setenv("TZ", timezoneStr, 1);

      time_t ESPnow = time(nullptr);
      Serial.println(ctime(&ESPnow));

      struct tm *timeinfo;
      time(&ESPnow);
      timeinfo = localtime(&ESPnow);

      Serial.println(timeinfo->tm_isdst);

      Serial.println("Updated Time from RTC");
      rtc.adjust(DateTime( (timeinfo->tm_year + 1900) , timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec));
      showDate( rtc.now() );
      
      ntp_update = false;
    }
    
  }
  else Serial.println("WiFi connection *** FAILED ***");
  return connectionStatus;
}

/* Turn WiFi off */
void StopWiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

/* Turn ESP32 off */
void espSLEEP(long refresh)
{
  display.powerOff();
  long SleepTimer = SleepDuration * 60; // Sleep duration in seconds

  DateTime now = rtc.now();

  // Change Refresh time to longer duration to save power
  if(now.hour() >= PowerSaveStart || now.hour() < PowerSaveEnd )
  {
    SleepTimer = 7200;

    if( now.hour() < PowerSaveEnd && (now.hour() + ( SleepTimer / 3600 )) > PowerSaveEnd  )
    {
        SleepTimer  =  (PowerSaveEnd * 3600) - ( (now.hour() * 3600) + (now.minute() * 60) + now.second() );
        if( SleepTimer < (SleepDuration * 60) )
        {
            SleepTimer = SleepTimer + SleepDuration * 60;
        }
    }
  }

  // Check if the sleep lasts into the next day. In that case adjust the sleep time to update the date.
  if( now.unixtime() )

  // Play Alarm sounds here, since the display is done refreshing
  if(hasNotification)
  {
    playAlarmType = 1;
  }
  
  // Check if the Birthday sound needs to be played.

  // 1 - Alarm
  // 2 - Bday
  // 3 - Both

  if(hasBirthday && playBday)
  {
    // Get the set time for a birthday alarm
    DateTime bday (now.year(), now.month(), now.day(), playBdayTime, 0, 0);

    // Refresh has been set by appointment
    if( refresh > 0 && ( now.unixtime() + refresh ) > bday.unixtime() && now.unixtime() < bday.unixtime() )
    {
      refresh = bday.unixtime() - now.unixtime();
      if(playAlarmType == 1)
      {
        playAlarmType = 3;
      }
      else{
        playAlarmType = 2;
      }
    }

    // Standard refresh is used
    if( refresh == 0 && ( now.unixtime() + SleepDuration * 60 ) > bday.unixtime() && now.unixtime() < bday.unixtime() )
    {
      refresh = bday.unixtime() - now.unixtime();
      if(playAlarmType == 1)
      {
        playAlarmType = 3;
      }
      else{
        playAlarmType = 2;
      }
    }
  }


  if(refresh > 0)
  {
    SleepTimer = refresh;
  }

  if(refresh > SleepDuration * 60)
  {
    SleepTimer = SleepDuration * 60;
    playAlarmType = 0;
  }

  if(!playAlarm)
  {
    playAlarmType = 0;
  }

  // Prevent the Sleeptimer from being too short
  if(SleepTimer == 0)
  {
    SleepTimer = SleepDuration * 60;
  }

  // Set the ESP sleeptimer
  esp_sleep_enable_timer_wakeup(SleepTimer * 1000000LL);

  Serial.println("Deep-sleep for " + String(SleepTimer) + " seconds");

  // Initiate Deep Sleep
  esp_deep_sleep_start();
}

/* Http Request */
bool httpsRequest(WiFiClient& client, int req)
{
  // close connection before sending a new request
  client.stop();
  HTTPClient http;
  http.setTimeout(60000); // the local RPi Zero is very slow, so a long time out is needed
  if(req == 1)
  {
    http.begin( "http://" + String(calserver) + "/calendar?token=" + calkey);
  }
  if(req == 2)
  {
    String uri = "/data/2.5/weather?appid=" + apikey + "&zip=" + zip + "&units=" + units + "&lang=" + lang;
    http.begin(client, server, 443, uri);
  }
  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK) {
    if (req == 1 && !parseIcalJSON( http.getString() ) )
        return false;
    if (req == 2 && !parseWeathermapJSON( http.getStream() ) )
        return false;
    client.stop();
    http.end();
    return true;
  }
  else
  {
    Serial.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
    Serial.println(httpCode);
    client.stop();
    http.end();
    return false;
  }
  http.end();
  return true;
}

/* Receive Data from Calendar Server */
void getCalendarData()
{
  if (StartWiFi() == WL_CONNECTED) {
    byte Attempts = 1;
    bool iCal = false, weather = false;
    WiFiClientSecure client;   // wifi client object
    client.setInsecure();

    while ((iCal == false || weather == false) && Attempts < 2) { // Try up-to 2 time for Weather and Forecast data
      if (weather  == false) weather = httpsRequest(client,2);
      if (iCal == false)     iCal    = httpsRequest(client,1);
      Attempts++;

      if (iCal && weather)
      {
        Serial.println("iCal:    "+ String(iCal));
        Serial.println("Weather: "+ String(iCal));
        StopWiFi(); // Reduces power consumption
        // Start displaying data
      }
    }
  }
}

/* Parse the calendar JSON */
bool parseIcalJSON(String json)
{
  Serial.print(F("\nCreating object... Calendar"));

  // allocate the JsonDocument
  DynamicJsonDocument doc(20 * 1024);
  
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json.c_str());
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }

  Serial.println(doc.memoryUsage());

  if(doc.containsKey("events"))
  {
    if(doc["events"].size() > 0){
      for(byte r = 0; r < 10; r++){
        if(doc["events"][r].containsKey("gmtoffset")){
          Events[r].starttime = doc["events"][r]["start"].as<time_t>() + doc["events"][r]["gmtoffset"].as<int>() * 60;
          Events[r].endtime = doc["events"][r]["end"].as<time_t>() + doc["events"][r]["gmtoffset"].as<int>() * 60;
        }
        else{
          Events[r].starttime = doc["events"][r]["start"].as<time_t>() + doc["gmtoffset"].as<int>() * 60;
          Events[r].endtime = doc["events"][r]["end"].as<time_t>() + doc["gmtoffset"].as<int>() * 60;
        }

        Events[r].description = doc["events"][r]["summary"].as<const char*>();
        if(doc["events"][r]["location"] == nullptr) {
          Events[r].location = "";
        }
        else{
          Events[r].location = doc["events"][r]["location"].as<const char*>();
        }

        if(doc["events"][r]["categories"] == nullptr) {
          Events[r].categories = "";
        }
        else{
          Events[r].categories = doc["events"][r]["categories"].as<const char*>();
        }
        Events[r].alarm = false;
        Events[r].allday = doc["events"][r]["allday"];
      }
    }
  }

  doc.clear();
  return true;
}

/* Parse the weather JSON */
bool parseWeathermapJSON(WiFiClient& json)
{
  Serial.print(F("Parsing Weather"));

  // allocate the JsonDocument
  DynamicJsonDocument doc(3 * 1024);
  
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }

  Serial.println(doc.memoryUsage());

  // JsonObject root = doc.as<JsonObject>();

  if(doc.containsKey("weather"))
  {
    if(doc["weather"].size() > 0){
      Weather[0].summary     = doc["weather"][0]["description"].as<const char*>();
      Weather[0].icon        = doc["weather"][0]["icon"].as<const char*>();
    }
  }
  if(doc.containsKey("main")){
    Weather[0].temperature = doc["main"]["temp"].as<float>();
  }

  if(doc.containsKey("timezone"))
  {
    gmtOffset_sec = doc["timezone"].as<int>();
  }

  doc.clear();
  return true;
}

/* Show Date over Serial */
void showDate(const DateTime& dt) {
    Serial.print(dt.year(), DEC);
    Serial.print('/');
    Serial.print(dt.month(), DEC);
    Serial.print('/');
    Serial.print(dt.day(), DEC);
    Serial.print(' ');
    Serial.print(dt.hour(), DEC);
    Serial.print(':');
    Serial.print(dt.minute(), DEC);
    Serial.print(':');
    Serial.print(dt.second(), DEC);
    Serial.println();
}
