/*
 * Simple Text formatting functions
 */

#include <U8g2_for_Adafruit_GFX.h>

void textCenter(U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, int x, int y, const char *text);
void textRight(U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, int x, int y, const char *text);
void textEllipsis(U8G2_FOR_ADAFRUIT_GFX& u8g2Fonts, int x, int y, int width, const char *text);
