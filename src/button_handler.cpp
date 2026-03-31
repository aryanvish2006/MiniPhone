#include <Arduino.h>
#include "config.h"
#include "button_handler.h"
#include "state_machine.h"

unsigned long lastDebounceTime[5] = {0};
bool lastButtonState[5] = {HIGH, HIGH, HIGH, HIGH, HIGH};
const int buttonPins[5] = {BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_OK};

void initButtons() {
    for (int i = 0; i < 5; i++) {
        pinMode(buttonPins[i], INPUT_PULLUP);
    }
}

void handleButtons() {
    for (int i = 0; i < 5; i++) {
        bool reading = digitalRead(buttonPins[i]);
        if (reading != lastButtonState[i]) {
            lastDebounceTime[i] = millis();
        }
        if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
            if (reading == LOW) {  // Button pressed
                switch (i) {
                    case 0: handleButtonPress(UP); break;
                    case 1: handleButtonPress(DOWN); break;
                    case 2: handleButtonPress(LEFT); break;
                    case 3: handleButtonPress(RIGHT); break;
                    case 4: handleButtonPress(OK); break;
                }
            }
        }
        lastButtonState[i] = reading;
    }
}