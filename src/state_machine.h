#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "config.h"

enum Button { UP, DOWN, LEFT, RIGHT, OK };

extern Page currentPage;
extern int selectedIndex;

void handleButtonPress(Button button);

#endif