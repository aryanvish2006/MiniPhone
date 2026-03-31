#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

void initOLED();
void clearOLED();
void drawText(int x, int y, String text);
void displayOLED();

#endif