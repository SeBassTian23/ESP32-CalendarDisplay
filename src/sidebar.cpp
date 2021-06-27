/*
 * Drawing instructions for Weather icons
 * 
 */

#include "Arduino.h"
#include "sidebar.h"
#include "icons.h"
#include "text.h"
#include "cal.h"

#include "../fonts/u8g2_arialB42_tf.c"

/* Battery Volatage */
#define ADC_PIN A13

bool isLeapYear(int year)
{
    if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
        return true;
    return false;
}

int getDaysInMonth(int month, int year)
{
    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month != 2)
        return daysInMonth[(month-1)%12];
    if(isLeapYear(year))
        return 29;
    return 28;
}

int getDayOfWeek(int year, int month, int day)
{
    uint16_t months[] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365         };   // days until 1st of month
    
    uint32_t days = year * 365;        // days until year 
    for (uint16_t i = 4; i < year; i += 4) if (isLeapYear(i) ) days++;     // adjust leap years, test only multiple of 4 of course
    
    days += months[month-1] + day;    // add the days of this year
    if ((month > 2) && isLeapYear(year)) days++;  // adjust 1 if this year is a leap year, but only after febr
    
    // make Sunday 0
    days--;
    if(days < 0)
        days+= 7;
    return days % 7;   // remove all multiples of 7
}

void miniCalendar(GxEPD2_GFX& display, U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, RTC_PCF8523& rtc )
{

    // Time and Date
    DateTime now = rtc.now();

    int offset = 230;

    const char* month = monthsOfTheYear[(now.month()-1)];
    u8g2Fonts.setFont(u8g2_font_helvB14_tf);

    // Add the days of the week
    float col_width = (286-15)/7;
    u8g2Fonts.setForegroundColor(GxEPD_WHITE);
    u8g2Fonts.setFont(u8g2_font_helvB12_tf);
    
    offset += 30;

    for(byte r = 0; r < 7; r++){
        textCenter(u8g2Fonts, ( 15 + (col_width*r) + (col_width/2) ), offset, daysOfWeekDD[r] );
    }

    u8g2Fonts.setFont(u8g2_font_helvR08_tf);

    offset += 10;

    display.drawLine( 25, offset, 275, offset , GxEPD_WHITE);

    offset += 24;

    int days = getDaysInMonth( now.month(), now.year() );
    int weekday = getDayOfWeek(now.year(), now.month(), 1);

    u8g2Fonts.setFont(u8g2_font_helvR12_tf);

    int day = 0;
    for(byte w = 0; w < 6; w++){
        for(byte r = 0; r < 7; r++){
            if ( r != weekday && day == 0) {
                continue;
            }
            if( r == weekday && day == 0) {
                day = 1;
            }
            if( day > days)
                break;

            if( now.day() == day){
                int sw = u8g2Fonts.getUTF8Width( String(day).c_str() );

                u8g2Fonts.setForegroundColor(GxEPD_WHITE);
                u8g2Fonts.setBackgroundColor(GxEPD_RED);

                display.fillCircle( (15 + (col_width*r) + (col_width/2) ), (offset - 6 + ( 30 * w)), 12, GxEPD_WHITE);

                u8g2Fonts.setForegroundColor(GxEPD_RED);
                u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
                u8g2Fonts.setFont(u8g2_font_helvB12_tf);
            }
            textCenter(u8g2Fonts, ( 15 + (col_width*r) + (col_width/2) ), offset + ( 30 * w), String(day).c_str() );
            u8g2Fonts.setFont(u8g2_font_helvR12_tf);
            u8g2Fonts.setBackgroundColor(GxEPD_RED);
            u8g2Fonts.setForegroundColor(GxEPD_WHITE);
            day += 1;
        }
    }
}

void sideBar(GxEPD2_GFX& display, U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, RTC_PCF8523& rtc, Weather_type* weather )
{

    // Time and Date
    DateTime now = rtc.now();

    // Day, Weekday, Month and Year
    char day[3];
    itoa(now.day(),day,10);
    const char* weekday = daysOfTheWeek[now.dayOfTheWeek()];
    const char* month = monthsOfTheYear[(now.month()-1)];
 
    // Red sidebar container
    display.fillRect(0, 0, 300, 528, GxEPD_RED);

    // Set up fonts
    u8g2Fonts.setFontMode(1);
    u8g2Fonts.setFontDirection(0);
    u8g2Fonts.setForegroundColor(GxEPD_WHITE);
    u8g2Fonts.setBackgroundColor(GxEPD_RED);

    // Day
    u8g2Fonts.setForegroundColor(GxEPD_WHITE);

    //u8g2_font_logisoso92_tn u8g2_font_logisoso78_tn u8g2_font_logisoso62_tn

    u8g2Fonts.setFont(u8g2_font_logisoso92_tn);
    // u8g2Fonts.setFont(&OpenSans_Regular64pt7bBitmaps);
    // textCenter(u8g2Fonts, 150, 70, day); //u8g2_font_fub49_tn
    textCenter(u8g2Fonts, 150, 112, day);

    // int16_t tbx, tby; uint16_t tbw, tbh;
    // uint16_t xx = ((display.width() - tbw) / 2) - tbx;
    // uint16_t yy = ((display.height() - tbh) / 2) - tby;

    // display.setTextColor(GxEPD_BLACK);
    // display.setFont(&OpenSans_Regular64pt7bBitmaps);
    // display.getTextBounds("19", 0, 0, &tbx, &tby, &tbw, &tbh);
    // display.setCursor(xx, yy);
    // display.print("19");

    // Day's ordinate
    int w = u8g2Fonts.getUTF8Width(day);
    u8g2Fonts.setFont(u8g2_font_helvB12_tf);
    u8g2Fonts.setCursor(155 + w/2, 34);
    if(strcmp(day, "1") == 0 || strcmp(day, "21") == 0  || strcmp(day, "31") == 0 )
        u8g2Fonts.print(F("st"));
    else if(strcmp(day, "2") == 0 || strcmp(day, "22") == 0 )
        u8g2Fonts.print(F("nd"));
    else if(strcmp(day, "3") == 0 || strcmp(day, "23") == 0 )
        u8g2Fonts.print(F("rd"));
    else
        u8g2Fonts.print(F("th"));

    // Weekday
    u8g2Fonts.setFont(u8g2_font_arialB42_tf);
    // textCenter(u8g2Fonts, 150, 125, weekday);
    textCenter(u8g2Fonts, 150, 167, weekday);

    // Month
    w = u8g2Fonts.getUTF8Width(weekday);
    u8g2Fonts.setFont(u8g2_font_helvR14_tf);
    // u8g2Fonts.setCursor( 150 - w/2 ,150);
    u8g2Fonts.setCursor( 150 - w/2 ,192);
    u8g2Fonts.print(month);

    // Year
    int m = u8g2Fonts.getUTF8Width(month);
    int space = u8g2Fonts.getUTF8Width(" ");
    // u8g2Fonts.setCursor( 150 - w/2 + m + space ,150);
    u8g2Fonts.setCursor( 150 - w/2 + m + space ,192);
    u8g2Fonts.print(now.year());

    // Add Mini calendar if there are no task
    miniCalendar(display, u8g2Fonts, rtc );

    if( String(weather[0].temperature).length() > 0 && String(weather[0].summary).length() > 0 ){

        // Current Temperature
        u8g2Fonts.setForegroundColor(GxEPD_WHITE);
        u8g2Fonts.setFont(u8g2_font_helvB18_tf);
        textRight(u8g2Fonts, 185, 480, (String( (int)weather[0].temperature)+"°").c_str());
    }
    //     // Day icon	Night icon	Description
    //     // 01d.png 	01n.png 	clear sky
    //     // 02d.png 	02n.png 	few clouds
    //     // 03d.png 	03n.png 	scattered clouds
    //     // 04d.png 	04n.png 	broken clouds
    //     // 09d.png 	09n.png 	shower rain
    //     // 10d.png 	10n.png 	rain
    //     // 11d.png 	11n.png 	thunderstorm
    //     // 13d.png 	13n.png 	snow
    //     // 50d.png 	50n.png 	mist

    if( String(weather[0].icon).length() > 0 && String(weather[0].summary).length() > 0 ){
        // Current Weather Icon
        if( strcmp(weather[0].icon, "01d") == 0 )
            iconClearDay(display, 195, 435, 90);
        else if(strcmp(weather[0].icon, "01n") == 0 )
            iconClearNight(display, 195, 435, 90);
        else if(strcmp(weather[0].icon, "02d") == 0 )
            iconCloudyDay(display, 195, 435, 90);
        else if(strcmp(weather[0].icon, "02n") == 0 )
            iconCloudyNight(display, 195, 435, 90);
        else if(strcmp(weather[0].icon, "03d") == 0 || strcmp(weather[0].icon, "03n") == 0 )
            iconCloudy(display, 195, 435, 90);
        else if(strcmp(weather[0].icon, "04d") == 0 || strcmp(weather[0].icon, "04d") == 0 )
            iconCloudy(display, 195, 435, 90);
        else if(strcmp(weather[0].icon, "09d") == 0 || strcmp(weather[0].icon, "09d") == 0 )
            iconRain(display, 195, 435, 90);
        else if(strcmp(weather[0].icon, "10d") == 0 || strcmp(weather[0].icon, "10n") == 0 )
            iconRain(display, 195, 435, 90);
        else if(strcmp(weather[0].icon, "11d") == 0 || strcmp(weather[0].icon, "11d") == 0 )
            iconThunderstorm(display, 195, 435, 90);
        else if(strcmp(weather[0].icon, "13d") == 0 || strcmp(weather[0].icon, "13n") == 0 )
            iconSnow(display, 195, 435, 90);
        else if(strcmp(weather[0].icon, "50d") == 0 || strcmp(weather[0].icon, "50n") == 0)
            iconFog(display, 195, 435, 90);
        else{
            iconCloudy(display, 195, 435, 90);
        }
    }

    if( String(weather[0].summary).length() > 0 ){
        // Forcast Text
        u8g2Fonts.setForegroundColor(GxEPD_WHITE);
        u8g2Fonts.setFont(u8g2_font_helvR14_tf);
        textRight(u8g2Fonts, 185,505, weather[0].summary);
    }
    else{
        u8g2Fonts.setForegroundColor(GxEPD_WHITE);
        u8g2Fonts.setFont(u8g2_font_helvR14_tf);
        textCenter(u8g2Fonts, 150,505, "Weather not available");
    }

    if(analogRead(ADC_PIN) < 2000){
        iconBatteryEmpty( display, 10, 500, 20, GxEPD_WHITE);
    }
}
