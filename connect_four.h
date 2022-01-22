#include "getch.h"

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

int rows, cols;

void print_game(int game_board[rows][cols], int piece);

void place_token(int game_board[rows][cols], int last_available_row[cols], int piece);

int isGameOver(int game_board[rows][cols], int piece);

