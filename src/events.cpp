/*
 * Drawing instructions for Weather icons
 * 
 */

#include "events.h"
#include "icons.h"
#include "text.h"
#include "cal.h"
#include "common.h"

int eventItem(GxEPD2_GFX& display, U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, int y, const char *description, const char *location, const char *start, const char *end, const char *category, bool current ){

    int y_start = y;

    // Events Description
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    if(current)
        u8g2Fonts.setForegroundColor(GxEPD_RED);
    u8g2Fonts.setFont(u8g2_font_helvR18_tf);

    if( String(category) == "Birthdays"){
        iconBirthday( display, 365, y+2, 20, GxEPD_RED, GxEPD_WHITE);
        textEllipsis(u8g2Fonts, 390, y+20, (800-390), description );
    }
    else if( String(category) == "Tasks"){
        iconTask( display, 365, y+2, 20, GxEPD_RED, GxEPD_WHITE);
        textEllipsis(u8g2Fonts, 390, y+20, (800-390), description );    
    }
    // else if( String(category) == "Holidays"){
    //     iconAppointmentAlt( display, 365, y+1, 20, GxEPD_RED, GxEPD_WHITE);
    //     textEllipsis(u8g2Fonts, 390, y+20, (800-390), description );    
    // }
    else{
        textEllipsis(u8g2Fonts, 365, y+20, (800-365), description );
    }

    // Event Start Time
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setFont(u8g2_font_helvB12_tf);
    textRight(u8g2Fonts, 865, y+20-6, start );

    if( String(end).length()  > 0 ){
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.setFont(u8g2_font_helvR08_tf);
        textRight(u8g2Fonts, 865, y+20+10, end );
    }

    if( String(location).length() > 0 ){
        // Event Location
        y += 20;
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.setFont(u8g2_font_helvR08_tf);
        textEllipsis(u8g2Fonts, 365, y+20, (800-365), location );
    }

    // Bottom margin
    y += 18 +20;

    // Side bar
    display.drawLine( 355, y_start, 355, y, GxEPD_BLACK );

    return y;
}

int eventDate(U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, int y, const char *day, const char *weekday){
    
    // Day
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setFont(u8g2_font_helvB18_tf);
    textRight(u8g2Fonts, 345, y+18, day);

    // Weekday
    y += 12;
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setFont(u8g2_font_helvR08_tf);
    textRight(u8g2Fonts, 345, y+18, weekday);
    return y;
}

int eventGap(GxEPD2_GFX& display, int y){
    y += 10;
    for(int i=0;i<20;i=i+8){
        display.drawLine( 355, y+i, 355, y+i+4, GxEPD_BLACK );
    }
    y += 15;
    return y;
}

void eventNext(GxEPD2_GFX& display, U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, const char *description, const char *duration, const char *location )
{

    u8g2Fonts.setFontMode(1);
    u8g2Fonts.setFontDirection(0);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);

    // Header
    u8g2Fonts.setForegroundColor(GxEPD_RED);
    u8g2Fonts.setFont(u8g2_font_helvB14_tf);
    u8g2Fonts.setCursor(315,445);
    u8g2Fonts.print(F("UP NEXT"));

    // Event
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setFont(u8g2_font_helvR24_tf);
    textEllipsis(u8g2Fonts, 315, 484, (865-315), description );

    // Duration
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setFont(u8g2_font_helvR14_tf);
    u8g2Fonts.setCursor(315,514);
    u8g2Fonts.print(duration);

    if(location != ""){
        // Location Icon
        iconLocation( display, 490, 500, 15, GxEPD_RED, GxEPD_WHITE );

        // Location
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.setFont(u8g2_font_helvR14_tf);
        textEllipsis(u8g2Fonts, 515, 514, (865-515), location );
    }
}

long eventList(GxEPD2_GFX& display, U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, RTC_PCF8523& rtc, Event_type* events )
{

    u8g2Fonts.setFontMode(1);
    u8g2Fonts.setFontDirection(0);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);

    // Day and Weekday
    int start = 20; // Top padding
    int mday;
    int yday;
    bool active = false;
    bool next = false;
    DateTime now = rtc.now();

    // Refresh Time min
    long refresh = 0;

    // Build List
    bool isEmpty = true;
    for (byte r = 0; r < 10; r++) {

        // Check if entry should be skipped
        if(events[r].starttime == 0 || String(events[r].description).length() == 0)
            continue;

        // At least one item that is not empty
        isEmpty = false;

        // Time Objects
        time_t sTime = events[r].starttime;
        struct tm *lt = localtime(&sTime);

        // Add gap indicator
        if(r > 0 && (lt->tm_yday - yday) > 1){
            start = eventGap(display, start);
        }

        // Add gap indicator if today has no events
        if(r == 0 && lt->tm_mday > now.day() ){
            start = eventGap(display, start - 20);
            start += 15;
        }

        // Stop if no more room for a new event
        if ( 528 - start < 58 )
            break;

        // Add new day
        if(r == 0 || mday != lt->tm_mday){
            if(r > 0)
                start += 15; // Add padding

            eventDate(u8g2Fonts, start, String(lt->tm_mday).c_str(), daysOfWeekDDD[lt->tm_wday]);
        }

        // Check if there is a birthday today
        if( String(events[r].categories) == "Birthdays" && now.day() == lt->tm_mday )
        {
            hasBirthday = true;
        }

        // Display event
        if(events[r].allday){
            start = eventItem(display, u8g2Fonts, start, events[r].description, events[r].location, "all-day", "", events[r].categories, false );
        }
        else{
            String startTime;
            char min[3];
            sprintf(min, "%02d", lt->tm_min);

            if(lt->tm_hour > 12)
                startTime = String(lt->tm_hour-12) + ":" + String(min) + " pm";
            else
                startTime = String(lt->tm_hour) + ":" + String(min) + " am";

            String endTime = "";
            if( events[r].endtime > 0){
                time_t eTime = events[r].endtime;
                struct tm *ltend = localtime(&eTime);
                
                sprintf(min, "%02d", ltend->tm_min);

                if(ltend->tm_hour > 12)
                    endTime = String(ltend->tm_hour-12) + ":" + String(min) + " pm";
                else
                    endTime = String(ltend->tm_hour) + ":" + String(min) + " am";

                if( sTime <= now.unixtime() && eTime >= now.unixtime() ){
                    active = true;
                    // Refresh time in minutes
                    refresh = ( events[r].endtime - now.unixtime() );
                }

            }

            start = eventItem(display, u8g2Fonts, start, events[r].description, events[r].location, startTime.c_str(), endTime.c_str(), events[r].categories, active );

            // Check if the event is up next
            if( !active && !next && sTime > now.unixtime() && now.day() == lt->tm_mday){
                eventNext(display, u8g2Fonts, events[r].description, String(startTime + " - " + endTime).c_str(), events[r].location);
                next = true;

                // Refresh time in minutes
                if(refresh == 0)
                {
                    refresh = ( events[r].endtime - now.unixtime() );
                    hasNotification = true;
                }
            }
        }

        // Terminate if next is shown
        if (start > 350 && next)
            break;
        
        // Terminate if list would overflow
        if (start > 490 )
            break;

        // set day
        mday = lt->tm_mday;
        yday = lt->tm_yday;
        active = false;
    }

    if (isEmpty == true) {
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.setFont(u8g2_font_helvR18_tf);
        textCenter(u8g2Fonts, 365 + ((800-365)/2), 275, "No Events" );
    }
    
    return refresh;
}
