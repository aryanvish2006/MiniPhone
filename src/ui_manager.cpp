#include "config.h"
#include "ui_manager.h"
#include "oled_display.h"
#include "state_machine.h"
#include "file_system.h"
#include <ArduinoJson.h>

void updateUI() {
    clearOLED();
    if (subState == SELECT_CONTACT_FOR_MSG) {
        drawText(0, 0, "Select Contact for SMS");
        String contactsJson = loadContacts();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, contactsJson);
        JsonArray contacts = doc["contacts"];
        int start = max(0, selectedIndex - 2);
        for (int i = 0; i < 3 && start + i < contacts.size(); i++) {
            String name = contacts[start + i]["name"];
            drawText(0, 20 + i * 10, (i == selectedIndex - start ? ">" : " ") + name);
        }
        displayOLED();
        return;
    }

    switch (currentPage) {
        case DIALER:
            drawText(0, 0, "Dialer");
            drawText(0, 20, "Number: " + dialerNumber);
            drawText(0, 40, "Cursor: " + String(cursorPos) + " UP/DN: edit");
            break;
        case CONTACTS: {
            drawText(0, 0, "Contacts");
            String contactsJson = loadContacts();
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, contactsJson);
            JsonArray contacts = doc["contacts"];
            int start = max(0, selectedIndex - 2);
            for (int i = 0; i < 3 && start + i < contacts.size(); i++) {
                String name = contacts[start + i]["name"];
                drawText(0, 20 + i * 10, (i == selectedIndex - start ? ">" : " ") + name);
            }
            break;
        }
        case PRETYPED_MSG: {
            drawText(0, 0, "Messages");
            String messagesJson = loadMessages();
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, messagesJson);
            JsonArray messages = doc["messages"];
            int start = max(0, selectedIndex - 2);
            for (int i = 0; i < 3 && start + i < messages.size(); i++) {
                String msg = messages[start + i]["text"];
                drawText(0, 20 + i * 10, (i == selectedIndex - start ? ">" : " ") + msg.substring(0, 10));
            }
            break;
        }
        case CONTACT_UPLOAD:
            drawText(0, 0, "Upload Contacts");
            drawText(0, 20, "Via Web: 192.168.4.1");
            break;
        case SETTINGS:
            drawText(0, 0, "Settings");
            drawText(0, 20, "Placeholder");
            break;
    }
    displayOLED();
}