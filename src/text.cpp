/*
 * Simple Text formatting functions
 */

#include "text.h"

void textCenter(U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, int x, int y, const char *text)
{
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w = u8g2Fonts.getUTF8Width(text);
  u8g2Fonts.setCursor(x - w / 2, y);
  u8g2Fonts.print(F(text));
}

void textRight(U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, int x, int y, const char *text)
{
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w = u8g2Fonts.getUTF8Width(text);
  u8g2Fonts.setCursor(x - w, y);
  u8g2Fonts.print(F(text));
}

void textEllipsis(U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, int x, int y, int width, const char *text)
{
  uint16_t w = u8g2Fonts.getUTF8Width(text);
  uint16_t w_substr = w;
  String ellipsis = text;
  int length = ellipsis.length();
  if(w_substr > width){
    while(w_substr > width)
    {
      length -= 1;
      ellipsis = ellipsis.substring(0,length);
      w_substr = u8g2Fonts.getUTF8Width(ellipsis.c_str());
    }
    ellipsis = ellipsis.substring(0,length-3) + "...";
  }
  u8g2Fonts.setCursor(x, y);
  u8g2Fonts.print(F(ellipsis.c_str()));
}