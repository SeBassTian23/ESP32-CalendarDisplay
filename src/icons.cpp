/*
 * Drawing instructions for Weather icons
 */

#include "icons.h"

void iconCloud(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r)
{
  // top circle
  display.fillCircle(x, y, r, GxEPD_WHITE);
  // left circle
  display.fillCircle(x-r*0.85, y+r*0.8, r*0.85, GxEPD_WHITE);
  // right circle
  display.fillCircle(x+r*1.1, y+r*0.8, r*0.85, GxEPD_WHITE);
  // rectangle
  display.fillRect(x-r*0.85, y+r*0.8, (x+r*1.1)- (x-r*0.85), r*0.9, GxEPD_WHITE );

  // top circle
  float offset = 0.8;
  display.fillCircle(x, y, r*offset, GxEPD_RED);
  // left circle
  display.fillCircle(x-r*0.85, y+r*0.8, r*0.85*offset, GxEPD_RED);
  // right circle
  display.fillCircle(x+r*1.1, y+r*0.8, r*0.85*offset, GxEPD_RED);
  // rectangle
  display.fillRect(x-r*0.85, y+r*0.7, (x+r*1.1) - (x-r*0.85), r*offset, GxEPD_RED );

}

void iconSun(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r)
{
  display.drawLine(x-r*1.75, y, x+r*1.75,y, GxEPD_WHITE );
  display.drawLine(x, y-r*1.75, x, y+r*1.75, GxEPD_WHITE );
  display.drawLine(x - r*1.25, y - r*1.25, x + r*1.25, y +r*1.25, GxEPD_WHITE);
  display.drawLine(x - r*1.25, y + r*1.25, x + r*1.25, y - r*1.25, GxEPD_WHITE);
  display.fillCircle(x, y, r*1.2, GxEPD_RED);
  display.fillCircle(x, y, r, GxEPD_WHITE);
  float offset = 0.9;
  display.fillCircle(x, y, r*offset, GxEPD_RED);
}

void iconMoon(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r)
{
  float offset = 0.9;
  display.fillCircle(x, y, r, GxEPD_WHITE);
  display.fillCircle(x, y, r*offset, GxEPD_RED);
  display.fillCircle(x+r, y-r, r, GxEPD_WHITE);
  display.fillCircle(x+r, y-r, r*offset, GxEPD_RED);
  display.fillRect(x, y-r*2, r*2.5, r, GxEPD_RED);
  display.fillRect(x+r+1, y-r, r*1.5, r*1.5, GxEPD_RED);
}

void iconClearDay(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  iconSun(display, x+s/2, y+s/2, s/5);
}

void iconClearNight(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  iconMoon(display, x+s/2, y+s/2, s/5);
}

void iconRain(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5);
  display.fillRect(x+s*0.275, y+s*0.6, s/2.5, s/5, GxEPD_RED);

  float offset = 0.8;
  for (int i = 0; i <= s*0.1; i++)
  {
    display.fillCircle( x+s*0.4-i*0.5, y+s*0.65+i, s*0.02, GxEPD_WHITE);
    display.fillCircle( x+s*0.6-i*0.5, y+s*0.65+i, s*0.02, GxEPD_WHITE);
  }
  for (int i = 0; i <= s*0.16; i++)
  {
    display.fillCircle( x+s*0.5-i*0.5, y+s*0.65+i, s*0.02, GxEPD_WHITE);
  }
}

void iconSleet(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5);
  display.fillRect(x+s*0.275, y+s*0.6, s/2.5, s/5, GxEPD_RED);

  float offset = 0.8;
  for (int i = 0; i <= s*0.1; i++)
  {
    if(i < 1 || i > s*0.1 *0.5 )
    {
      display.fillCircle( x+s*0.4-i*0.5, y+s*0.65+i, s*0.02, GxEPD_WHITE);
      display.fillCircle( x+s*0.6-i*0.5, y+s*0.65+i, s*0.02, GxEPD_WHITE);
    }
  }
  for (int i = 0; i <= s*0.16; i++)
  {
    if(i < s*0.16 * 0.5 || i > s*0.16 * 0.8 )
      display.fillCircle( x+s*0.5-i*0.5, y+s*0.65+i, s*0.02, GxEPD_WHITE);
  }
}

void iconSnow(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5);
  display.fillRect(x+s*0.275, y+s*0.6, s/2.5, s/5, GxEPD_RED);

  float offset = 0.8;
  display.fillCircle( x+s/2.75, y+s*0.7, s*0.02, GxEPD_WHITE);
  display.fillCircle( x+s/1.75, y+s*0.7, s*0.02, GxEPD_WHITE);

  display.fillCircle( x+s/2.75, y+s*0.8, s*0.02, GxEPD_WHITE);
  display.fillCircle( x+s/1.75, y+s*0.8, s*0.02, GxEPD_WHITE);

  display.fillCircle( x+s/2.15, y+s*0.65, s*0.02, GxEPD_WHITE);
  display.fillCircle( x+s/2.15, y+s*0.75, s*0.02, GxEPD_WHITE);
  display.fillCircle( x+s/2.15, y+s*0.85, s*0.02, GxEPD_WHITE);

}

void iconWind(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  float offset = 0.8;
  for (int i = 0; i <= s*0.7; i++)
  {
    if(i < s*0.6)
      display.fillCircle( x+s*0.15+i, y+s*0.4, s*0.02, GxEPD_WHITE);
    if(i < s*0.5)
      display.fillCircle( x+s*0.1+i, y+s*0.5, s*0.02, GxEPD_WHITE);
    if(i < s*0.2)
      display.fillCircle( x+s*0.7+i, y+s*0.5, s*0.02, GxEPD_WHITE);
    if(i < s*0.6)
      display.fillCircle( x+s*0.2+i, y+s*0.6, s*0.02, GxEPD_WHITE);
  }
}

void iconFog(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5);
  display.fillRect(x+s*0.1, y+s*0.55, s*0.75, s/5, GxEPD_RED);

  float offset = 0.8;
  for (int i = 0; i <= s*0.7; i++)
  {
    display.fillCircle( x+s*0.1+i, y+s*0.6, s*0.02, GxEPD_WHITE);
    display.fillCircle( x+s*0.2+i, y+s*0.7, s*0.02, GxEPD_WHITE);
    display.fillCircle( x+s*0.15+i, y+s*0.8, s*0.02, GxEPD_WHITE);
  }
}

void iconCloudy(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  iconCloud(display, x+(s/4)*3, y+s/4, s/10);
  iconCloud(display, x+s/2.1, y+s/2.2, s/5);
}

void iconCloudyDay(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  iconSun(display, x+(s/3)*2, y+s/2.5, s/6);
  iconCloud(display, x+s/2.2, y+s/2.2, s/5);
}

void iconCloudyNight(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  iconMoon(display, x+(s/3)*2, y+s/3, s/6);
  iconCloud(display, x+s/2.2, y+s/2.2, s/5);
}

void iconHail(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5);
  display.fillRect(x+s*0.275, y+s*0.6, s/2.5, s/5, GxEPD_RED);

  float offset = 0.8;
  for (int i = 0; i <= s*0.1; i++)
  {
    if(i < s*0.1 *0.5 || i == s*0.1 )
    {
      display.fillCircle( x+s*0.4-i*0.5, y+s*0.65+i, s*0.02, GxEPD_WHITE);
      display.fillCircle( x+s*0.6-i*0.5, y+s*0.65+i, s*0.02, GxEPD_WHITE);
    }
  }
  for (int i = 0; i <= s*0.16; i++)
  {
    if(i < s*0.16 *0.7 || i == s*0.16 )
      display.fillCircle( x+s*0.5-i*0.5, y+s*0.65+i, s*0.02, GxEPD_WHITE);
  }
}

void iconThunderstorm(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5);
  display.fillRect(x+s*0.275, y+s*0.6, s/2.5, s/5, GxEPD_RED);

  float offset = 0.8;
  for (int i = 0; i <= s*0.1; i++)
  {
    display.fillCircle( x+s*0.6-i*0.5, y+s*0.65+i, s*0.02, GxEPD_WHITE);
  }
  for (int i = 0; i <= s*0.16; i++)
  {
    display.fillCircle( x+s*0.5-i*0.5, y+s*0.65+i, s*0.02, GxEPD_WHITE);
  }
  display.fillTriangle( x+s*0.3, y+s*0.75, x+s*0.325, y+s*0.65, x+s*0.375, y+s*0.65, GxEPD_WHITE);
  display.fillTriangle( x+s*0.3, y+s*0.75, x+s*0.4, y+s*0.7, x+s*0.33, y+s*0.7, GxEPD_WHITE);
  display.fillTriangle( x+s*0.3, y+s*0.85, x+s*0.35, y+s*0.7, x+s*0.4, y+s*0.7, GxEPD_WHITE);

}

void fillEllipsis(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c)
{
  for(int yi=-h; yi<=h; yi++) {
    for(int xi=-w; xi<=w; xi++) {
        if(xi*xi*h*h+yi*yi*w*w <= h*h*w*w)
            display.writePixel(x+xi, y+yi, c);
    }
  }
}

void iconTornado(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s)
{
  // 1
  fillEllipsis(display, x+s*0.33, y+s*0.7, s/12*1.2, s/18*1.2, GxEPD_WHITE);
  fillEllipsis(display, x+s*0.33, y+s*0.7, s/12, s/18, GxEPD_RED);
  // 2
  fillEllipsis(display, x+s*0.32, y+s*0.65, s/9*1.2, s/16*1.2, GxEPD_WHITE);
  fillEllipsis(display, x+s*0.32, y+s*0.65, s/9, s/16, GxEPD_RED);
  // 3
  fillEllipsis(display, x+s*0.35, y+s*0.55, s/7*1.2, s/12*1.2, GxEPD_WHITE);
  fillEllipsis(display, x+s*0.35, y+s*0.55, s/7, s/12, GxEPD_RED);
  // 4
  fillEllipsis(display, x+s*0.425, y+s*0.425, s/5*1.2, s/8*1.2, GxEPD_WHITE);
  fillEllipsis(display, x+s*0.425, y+s*0.425, s/5, s/8, GxEPD_RED);
  // 5
  fillEllipsis(display, x+s*0.5, y+s*0.3, s/4*1.2, s/7*1.2, GxEPD_WHITE);
  fillEllipsis(display, x+s*0.5, y+s*0.3, s/4, s/7, GxEPD_RED);
}

void iconBatteryEmpty( GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, uint16_t color)
{
  display.drawRect( x + ( s * 0.1), y + ( s * 0.25 ), ( s - ( s * 0.05) ), s * 0.5, color);
  display.fillRect( x, y + (s * 0.14 ) + ( s * 0.25 ), ( s * 0.1), s * 0.30, color);
  display.fillRect( x + s - (s*0.3), y + ( s * 0.25 ), ( s * 0.3 ), s * 0.5,  color);
}

// Task Icon
void iconTask(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, uint16_t color, uint16_t bgcolor){
    // checkbox
    display.fillRoundRect( x, y, s-s/10, s-s/10, s/10, color );
    display.fillRoundRect( x+s/10 ,y+s/10, s-s/5-s/10, s-s/5-s/10, s/20, bgcolor );

    // gap for the check
    for (int i=0; i<s/2-(s/10); i++ ){
        display.drawLine( x+s/2, y+(s/2)-(s/10)+i, x+s, y-(s/10)+i, bgcolor );    
    }
    
    // right arm of the check
    for (int i=0; i<s/5; i++ ){
        display.drawLine( x+s/2, y+s/2+i, x+s, y+i, color );    
    }
    display.fillTriangle( x+s, y, x+s , y+s/5, x+s-s/5, y, bgcolor);

    // left arm of the check
    for (int i=0; i<s/5; i++ ){
        display.drawLine( x+s/4, y+s/4+i, x+s/2, y+s/2+i, color );    
    }
    display.fillTriangle( x+s/4, y+s/4+s/5, x+s/4 , y+s/4, x+s/4+s/5, y+s/4, bgcolor);
}

// Appointment Icon
void iconAppointment(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, uint16_t color, uint16_t bgcolor){
    // checkbox
    display.fillRoundRect( x, y+s/10, s, s-s/10, s/10, color );

    // gap for the check
    // for (int i=0; i<3; i++ ){
    //     display.fillRoundRect( x + s/16 + (s/3)*i, y, s/4, s/4, s/16, color );
    //     display.fillRoundRect( x + s/8  + (s/3)*i, y + s/16, s/8, s/6, s/32, bgcolor );
    // }
    display.fillRoundRect( x + s/2 - s/16 - s/4, y, s/4, s/4, s/16, color );
    display.fillRoundRect( x + s/2 - s/8 - s/8, y + s/16, s/8, s/6, s/32, bgcolor );
    display.fillRoundRect( x + s/2 + s/16, y, s/4, s/4, s/16, color );
    display.fillRoundRect( x + s/2 + s/8, y + s/16, s/8, s/6, s/32, bgcolor );

    // tiles
    for (int i=0; i<4; i++ ){

        int padding = (s - ((s/6) * 4 +  (s/16) * 3) ) / 2;

        display.fillRect( x + padding + (s/6)*i + (s/16)*i, y + s/3, s/6, s/6, bgcolor );
        display.fillRect( x + padding + (s/6)*i + (s/16)*i, y + s/3 + s/6 + s/16, s/6, s/6, bgcolor );
        display.fillRect( x + padding + (s/6)*i + (s/16)*i, y + s/3 + (s/6 + s/16)*2, s/6, s/6, bgcolor );
    }
}

// Appointment Alternative Icon
void iconAppointmentAlt(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, uint16_t color, uint16_t bgcolor){
    // checkbox
    display.fillRoundRect( x, y+s/10, s, s-s/10, s/10, color );

    display.fillRoundRect( x + s/2 - s/16 - s/4, y, s/4, s/4, s/16, color );
    display.fillRoundRect( x + s/2 - s/8 - s/8, y + s/16, s/8, s/6, s/32, bgcolor );
    display.fillRoundRect( x + s/2 + s/16, y, s/4, s/4, s/16, color );
    display.fillRoundRect( x + s/2 + s/8, y + s/16, s/8, s/6, s/32, bgcolor );

    // tile
    display.fillRect( x + s/10, y + s/3, s - s/5, s/2 + s/10, bgcolor );
}

// Task Icon
void iconBirthday(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, uint16_t color, uint16_t bgcolor){
    
    // box
    display.fillRoundRect( x + s/10, y+s/3, s-(s/10)*2, (s/3)*2, s/10, color );
    display.fillRoundRect( x, y+s/3, s, s/4, s/20, color );
    display.fillRoundRect( x+s/2-s/8, y, (s/8)*2, s-s/6, s/20, bgcolor);
    
    //bow
    display.fillCircle( x+s/3-s/12, y+s/6, s/6, color );
    display.fillTriangle( x+s/3-s/12, y+(s/6)*2, x+(s/3)*2+s/12, y+(s/6)*2, x+s/2-s/8, y+s/12, color );
    display.fillCircle( x+(s/3)*2+s/12, y+s/6, s/6, color );
    display.fillTriangle( x+s/3-s/12, y+(s/6)*2, x+(s/3)*2+s/12, y+(s/6)*2, x+s/2+s/8, y+s/12, color );
    // Bow inside
    display.fillCircle( x+s/3-s/12, y+s/6, s/16, bgcolor );
    display.fillTriangle( x+s/3-s/12, y+s/6+s/16, x+s/3+s/12, y+s/6+s/16, x+s/3-s/12, y+s/6-s/16, bgcolor );
    display.fillCircle( x+(s/3)*2+s/12, y+s/6, s/16, bgcolor );
    display.fillTriangle( x+(s/3)*2+s/12, y+s/6+s/16, x+(s/3)*2-s/12, y+s/6+s/16, x+(s/3)*2+s/12, y+s/6-s/16, bgcolor );
}

// Location Icon
void iconLocation(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, uint16_t color, uint16_t bgcolor){
    display.fillCircle(x+s/2, y+s/3, s/3, color );
    display.fillTriangle( x+s/2, y+s, (x+s/2) - s/3.75 , y+s/2, (x+s/2) + s/3.5, y+s/2, color );
    display.fillCircle(x+s/2, y+s/3, s/6, bgcolor );
}
