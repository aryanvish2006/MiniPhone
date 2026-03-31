#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "config.h"
#include "web_server.h"

AsyncWebServer server(80);

void initWebServer() {
    WiFi.softAP("GSMPhone", "password");
    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Upload received");
    }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        static File file;
        if (!index) {
            file = LittleFS.open(CONTACTS_FILE, "w");
        }
        file.write(data, len);
        if (final) {
            file.close();
        }
    });
    server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, CONTACTS_FILE, "application/json");
    });
    server.begin();
}

void handleWebServer() {
    // Asynchronous handling
}