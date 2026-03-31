#include <Arduino.h>
#include <Wire.h>
#include <LittleFS.h>
#include "config.h"
#include "button_handler.h"
#include "state_machine.h"
#include "ui_manager.h"
#include "sim800l_handler.h"
#include "oled_display.h"
#include "file_system.h"
#include "web_server.h"

// Global variables
volatile bool incomingCallFlag = false;
bool uiDirty = true;
String dialerNumber = "";
String callerID = "";
int cursorPos = 0;
SubState subState = MAIN;
String selectedMessage = "";

void IRAM_ATTR ringISR() {
    incomingCallFlag = true;
    uiDirty = true;
}

void setup() {
    Serial.begin(115200);
    LittleFS.begin();

    // Initialize OLED
    initOLED();

    // Initialize SIM800L
    initSIM800L();

    // Initialize buttons
    initButtons();

    // Initialize file system
    initFileSystem();

    // Initialize web server
    initWebServer();

    // Attach interrupt for RING
    pinMode(SIM800L_RING, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SIM800L_RING), ringISR, FALLING);

    // Initial UI draw
    updateUI();
}

void loop() {
    handleButtons();
    handleSIM800L();
    handleWebServer();

    if (incomingCallFlag) {
        handleIncomingCall();
        incomingCallFlag = false;
    }

    if (uiDirty) {
        updateUI();
        uiDirty = false;
    }

    delay(10);  // Small delay for stability
}