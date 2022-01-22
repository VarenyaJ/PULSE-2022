#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

int rows, cols;
int current_token_pos = 0;

static struct termios old, new;

void initTermios(int echo) {
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

char getch() {
    char ch;
    initTermios(0);
    ch = getchar();
    resetTermios();
    return ch;
}


void print_game(int game_board[rows][cols], int piece) {

    char token = 'O';
    int i, j;

    // DRAW THE MOVING TOKEN
    printf("\n");
    printf(" ");
    for (i = 0; i < cols; i++) {
        if (i == current_token_pos) {
            if (piece == 0) {
                printf(RED" O "RESET);
            }
            else {
                printf(BLUE" O "RESET);
            }
        }
        else {
		    printf("   "); // top box char
        }
    }
    printf(" ");
    printf("\n");

    // DRAW THE GAME BOARD
    printf("\u2554"); // topleft box char
    for (i = 0; i < cols * 3; i++) {
		printf("\u2550"); // top box char
    }
    printf("\u2557\n"); //top right char

    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if (game_board[i][j] == -1) {
                printf("   ");
            }
            else {
                switch( game_board[i][j] ) { //print colored text
                    case 0:
                        printf(" "RED"%c"RESET" ", token);
                        break;
                    case 1:
                        printf(" "BLUE"%c"RESET" ", token);
                        break;
                    }

            }
        }
	    printf("\u2551\n"); //print right wall and newline
    }

    printf("\u255A"); // print bottom left char
	for (i = 0; i < cols * 3; i++) {
	    printf("\u2550"); // bottom char
    }

	printf("\u255D\n"); //bottom right char

}

void place_token(int game_board[rows][cols], int last_available_row[cols], int piece) {
    // no available row in the current
    if (last_available_row[current_token_pos] == -1) {
        return;
    }

    int availableRow = last_available_row[current_token_pos];
    last_available_row[current_token_pos]--;
    game_board[availableRow][current_token_pos] = piece;
    system("clear");
    print_game(game_board, piece);
}

int isGameOver(int game_board[rows][cols], int piece) {
    
    int r, c;
    
    // horizontal
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols - 3; c++) {
            if (game_board[r][c] == piece && game_board[r][c + 1] == piece && game_board[r][c + 2] == piece && game_board[r][c + 3] == piece)
                return 1;
        }
    }

    // vertical
    for (c = 0; c < cols; c++) {
        for (r = 0; r < rows - 3; r++) {
            if (game_board[r][c] == piece && game_board[r + 1][c] == piece && game_board[r + 2][c] == piece && game_board[r + 3][c] == piece)
                return 1;
        }
    }
    
    // positive diagonal
    for (c = 0; c < cols - 3; c++) {
        for (r = 0; r < rows - 3; r++) {
            if (game_board[r][c] == piece && game_board[r + 1][c + 1] == piece && game_board[r + 2][c + 2] == piece && game_board[r + 3][c + 3] == piece)
                return 1;
        }
    }

    // negative diagonal
    for (c = 0; c < cols - 3; c++) {
        for (r = 3; r < rows; r++) {
            if (game_board[r][c] == piece && game_board[r - 1][c + 1] == piece && game_board[r - 2][c + 2] == piece && game_board[r - 3][c + 3] == piece)
                return 1;
        }
    }
        
    return 0;
}

int main() {

    system("clear"); // clear the screen

    // get input from user 
	char buf[200];
	char garbage[2];
	printf("CONNECT FOUR: Enter dimensions (rows columns):");
	if (NULL == fgets(buf,200,stdin)) {
		printf("\nProgram Terminated.\n");
		return 2;
	}
	if (2 != sscanf(buf,"%d%d%1s", &rows, &cols, garbage) || rows < 0 || cols < 0) {
		printf("invalid dimensions\n");
		return 2;
    }

    int i, j;
    int game_board[rows][cols];
    int last_available_row[cols];

    // initialize the game board
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            game_board[i][j] = -1;
        }
    }

    // initialize last available row
    for (i = 0; i < cols; i++) {
        last_available_row[i] = rows - 1;
    }

    int piece = 0; // to keep track of whose turn
    print_game(game_board, piece);

    char c;
    do {
        c = getch(); 
        switch(c) {
            case 'a': 
                if (current_token_pos > 0) {
                    current_token_pos--;
                }
                break;
            case 'd': 
                if (current_token_pos < cols - 1) {
                    current_token_pos++;
                }
                break;
            case '\n':
                place_token(game_board, last_available_row, piece);
                if (isGameOver(game_board, piece) == 1) {
                    printf("Player %d has won! Game over.\n", piece + 1);
                    return 0;
                }
                current_token_pos = 0;
                piece = (piece + 1) % 2;
        }
    system("clear");
    print_game(game_board, piece);

    } while (c != 'q'); 

    return 0;
}
