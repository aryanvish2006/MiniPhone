#include "config.h"
#include "state_machine.h"
#include "ui_manager.h"
#include "sim800l_handler.h"
#include "file_system.h"
#include <ArduinoJson.h>

Page currentPage = DIALER;
int selectedIndex = 0;  // For navigation within pages

void handleButtonPress(Button button) {
    if (subState == SELECT_CONTACT_FOR_MSG) {
        // Handle contact selection for message sending
        if (button == UP) selectedIndex = max(0, selectedIndex - 1);
        else if (button == DOWN) selectedIndex = min(MAX_CONTACTS - 1, selectedIndex + 1);
        else if (button == OK) {
            // Send SMS to selected contact
            String contactsJson = loadContacts();
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, contactsJson);
            JsonArray contacts = doc["contacts"];
            if (selectedIndex < contacts.size()) {
                String number = contacts[selectedIndex]["number"];
                sendSMS(number, selectedMessage);
            }
            subState = MAIN;
            currentPage = PRETYPED_MSG;
        } else if (button == LEFT) {
            subState = MAIN;
        }
        uiDirty = true;
        return;
    }

    switch (button) {
        case LEFT:
            if (currentPage == DIALER) {
                cursorPos = max(0, cursorPos - 1);
            } else {
                currentPage = (Page)((currentPage - 1 + 5) % 5);
                selectedIndex = 0;
            }
            break;
        case RIGHT:
            if (currentPage == DIALER) {
                if (cursorPos < dialerNumber.length()) {
                    cursorPos++;
                } else if (dialerNumber.length() > 0) {
                    // Delete last digit
                    dialerNumber = dialerNumber.substring(0, dialerNumber.length() - 1);
                    cursorPos = max(0, cursorPos - 1);
                }
            } else {
                currentPage = (Page)((currentPage + 1) % 5);
                selectedIndex = 0;
            }
            break;
        case UP:
            if (currentPage == DIALER) {
                // Cycle digit up at cursor
                if (cursorPos < dialerNumber.length()) {
                    char digit = dialerNumber.charAt(cursorPos);
                    digit = (digit == '9') ? '0' : digit + 1;
                    dialerNumber.setCharAt(cursorPos, digit);
                }
            } else {
                selectedIndex = max(0, selectedIndex - 1);
            }
            break;
        case DOWN:
            if (currentPage == DIALER) {
                // Cycle digit down at cursor
                if (cursorPos < dialerNumber.length()) {
                    char digit = dialerNumber.charAt(cursorPos);
                    digit = (digit == '0') ? '9' : digit - 1;
                    dialerNumber.setCharAt(cursorPos, digit);
                }
            } else {
                selectedIndex = min(9, selectedIndex + 1);
            }
            break;
        case OK:
            if (currentPage == DIALER && dialerNumber.length() > 0) {
                dialNumber(dialerNumber);
            } else if (currentPage == CONTACTS) {
                // Dial selected contact
                String contactsJson = loadContacts();
                DynamicJsonDocument doc(1024);
                deserializeJson(doc, contactsJson);
                JsonArray contacts = doc["contacts"];
                if (selectedIndex < contacts.size()) {
                    String number = contacts[selectedIndex]["number"];
                    dialNumber(number);
                }
            } else if (currentPage == PRETYPED_MSG) {
                // Select message and go to contact selection
                String messagesJson = loadMessages();
                DynamicJsonDocument doc(1024);
                deserializeJson(doc, messagesJson);
                JsonArray messages = doc["messages"];
                if (selectedIndex < messages.size()) {
                    selectedMessage = messages[selectedIndex]["text"];
                    subState = SELECT_CONTACT_FOR_MSG;
                    selectedIndex = 0;
                }
            } else if (currentPage == CONTACT_UPLOAD) {
                // Web upload handled separately
            } else if (currentPage == SETTINGS) {
                // Placeholder
            }
            break;
    }
    uiDirty = true;
}