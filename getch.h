#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

void initTermios(int echo);

void resetTermios(void);

char getch();
