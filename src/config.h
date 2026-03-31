#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Pin definitions
#define OLED_SDA 4   // D2
#define OLED_SCL 5   // D1
#define OLED_RESET -1 // No reset pin

#define SIM800L_RX 3  // D9 (NodeMCU RX)
#define SIM800L_TX 1  // D10 (NodeMCU TX)
#define SIM800L_RING 14 // D5

#define BUTTON_UP 12    // D6
#define BUTTON_DOWN 13  // D7
#define BUTTON_LEFT 15  // D8
#define BUTTON_RIGHT 2  // D9
#define BUTTON_OK 0     // D10

// Constants
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define DEBOUNCE_DELAY 50  // ms
#define MAX_CONTACTS 10
#define MAX_MESSAGES 10
#define MAX_NUMBER_LENGTH 15

// UI Pages
enum Page { DIALER, CONTACTS, PRETYPED_MSG, CONTACT_UPLOAD, SETTINGS };

// Sub-states for actions
enum SubState { MAIN, SELECT_CONTACT_FOR_MSG };

// File paths
#define CONTACTS_FILE "/contacts.json"
#define MESSAGES_FILE "/pretypedmsg.json"

// Global flags
extern volatile bool incomingCallFlag;
extern bool uiDirty;

// Global data
extern String dialerNumber;
extern String callerID;
extern int cursorPos;
extern SubState subState;
extern String selectedMessage;

#endif