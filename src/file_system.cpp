#include <LittleFS.h>
#include <ArduinoJson.h>
#include "config.h"
#include "file_system.h"

void initFileSystem() {
    if (!LittleFS.exists(CONTACTS_FILE)) {
        File file = LittleFS.open(CONTACTS_FILE, "w");
        file.println("{\"contacts\": [{\"name\": \"John\", \"number\": \"1234567890\"}]}");
        file.close();
    }
    if (!LittleFS.exists(MESSAGES_FILE)) {
        File file = LittleFS.open(MESSAGES_FILE, "w");
        file.println("{\"messages\": [{\"text\": \"Hello!\"}]}");
        file.close();
    }
}

String loadContacts() {
    File file = LittleFS.open(CONTACTS_FILE, "r");
    if (!file) return "{}";
    String data = file.readString();
    file.close();
    return data;
}

String loadMessages() {
    File file = LittleFS.open(MESSAGES_FILE, "r");
    if (!file) return "{}";
    String data = file.readString();
    file.close();
    return data;
}