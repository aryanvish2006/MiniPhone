#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "config.h"
#include "oled_display.h"

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

void initOLED() {
    Wire.begin(OLED_SDA, OLED_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED init failed");
        while (1);
    }
    display.clearDisplay();
}

void clearOLED() {
    display.clearDisplay();
}

void drawText(int x, int y, String text) {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(x, y);
    display.println(text);
}

void displayOLED() {
    display.display();
}