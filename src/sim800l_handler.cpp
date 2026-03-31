#include <SoftwareSerial.h>
#include "config.h"
#include "sim800l_handler.h"
#include "oled_display.h"
#include "state_machine.h"

SoftwareSerial simSerial(SIM800L_RX, SIM800L_TX);

void initSIM800L() {
    simSerial.begin(9600);
    delay(1000);
    simSerial.println("AT");  // Test
    delay(100);
    simSerial.println("AT+CLIP=1");  // Enable caller ID
    simSerial.println("AT+CMGF=1");  // SMS text mode
}

void handleSIM800L() {
    if (simSerial.available()) {
        String response = simSerial.readString();
        Serial.println("SIM800L: " + response);
        if (response.indexOf("+CLIP:") != -1) {
            // Extract caller ID
            int start = response.indexOf("\"") + 1;
            int end = response.indexOf("\"", start);
            callerID = response.substring(start, end);
        }
    }
}

void handleIncomingCall() {
    // Display incoming call
    clearOLED();
    drawText(0, 0, "Incoming Call");
    drawText(0, 20, "From: " + callerID);
    drawText(0, 40, "OK: Accept, LEFT: Reject");
    displayOLED();

    // Wait for button
    unsigned long startTime = millis();
    while (millis() - startTime < 10000) {  // 10s timeout
        handleButtons();
        if (digitalRead(BUTTON_OK) == LOW) {
            simSerial.println("ATA");  // Answer
            delay(1000);  // Wait for connection
            break;
        } else if (digitalRead(BUTTON_LEFT) == LOW) {
            hangUp();
            break;
        }
        delay(100);
    }
    uiDirty = true;
}

void dialNumber(String number) {
    simSerial.println("ATD" + number + ";");
    Serial.println("Dialing: " + number);
    delay(5000);  // Allow time for call setup
}

void sendSMS(String number, String message) {
    simSerial.println("AT+CMGS=\"" + number + "\"");
    delay(100);
    simSerial.print(message);
    delay(100);
    simSerial.write(26);  // Ctrl+Z to send
    Serial.println("SMS sent to " + number + ": " + message);
}

void hangUp() {
    simSerial.println("ATH");
    Serial.println("Call hung up");
}