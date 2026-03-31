#ifndef SIM800L_HANDLER_H
#define SIM800L_HANDLER_H

void initSIM800L();
void handleSIM800L();
void handleIncomingCall();
void dialNumber(String number);
void sendSMS(String number, String message);
void hangUp();

#endif