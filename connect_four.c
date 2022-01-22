#include "connect_four.h"

// COLORS
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

int current_token_pos = 0; // GLOBAL VARIABLE TO KEEP TRACK OF TOKEN COLUMN

/* 
 * print_game
 *   DESCRIPTION: This function prints out the current state of game board. 
 *   INPUTS: game_board (2D array), piece (0 or 1 depending on whose turn it is)
 *   OUTPUTS: current state of the game board
 *   RETURN VALUE: none (this is a void function)
*/
void print_game(int game_board[rows][cols], int piece) {

    // OUR TOKEN WILL LOOK LIKE THE LETTER O
    char token = 'O';

    // ITERATORS
    int i, j;

    // DRAW THE MOVING TOKEN AT THE TOP
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
    printf("\u2554");
    for (i = 0; i < cols * 3; i++) {
		printf("\u2550"); 
    }
    printf("\u2557\n"); 

    for(i = 0; i < rows; i++){
		printf("\u2551"); 
        for(j = 0; j < cols; j++){
            if (game_board[i][j] == -1) {
                printf("   ");
            }
            else {
                switch( game_board[i][j] ) { 
                    case 0:
                        printf(" "RED"%c"RESET" ", token);
                        break;
                    case 1:
                        printf(" "BLUE"%c"RESET" ", token);
                        break;
                    }

            }
        }
	    printf("\u2551\n"); 
    }

    printf("\u255A"); 
	for (i = 0; i < cols * 3; i++) {
	    printf("\u2550"); 
    }
	printf("\u255D\n"); 
}

/* 
 * place_token
 *   DESCRIPTION: Places a new token in the game board. 
 *   INPUTS: game_board (2D array), last_available_row (1D array) piece (0 or 1 depending on whose turn it is)
 *   OUTPUTS: the state of the game board after a new token has been placed
 *   RETURN VALUE: none (this is a void function)
*/
void place_token(int game_board[rows][cols], int first_available_row[cols], int piece) {

    // CHECK IF THERE IS NO AVAILABLE ROW WITH CURRENT COLUMN POSITION
    if (first_available_row[current_token_pos] == -1) {
        return;
    }

    // GET FIRST AVAILABLE ROW IN THE COLUMN
    int availableRow = first_available_row[current_token_pos];

    // UPDATE THE LAST AVAILABLE ROW IN THAT COLUMN (Hint: Decrement the value)
    first_available_row[current_token_pos]--;

    // PLACE TOKEN
    game_board[availableRow][current_token_pos] = piece;

    // CLEAR SCREEN AND PRINT OUT NEW STATE OF THE GAME
    system("clear");
    print_game(game_board, piece);
}

/* 
 * isGameOver
 *   DESCRIPTION: Checks if the game is over (if player has gotten 4 consecutive)
 *   INPUTS: game_board (2D array), piece (0 or 1 depending on whose turn it is)
 *   OUTPUTS: none
 *   RETURN VALUE: 1 if the game is over, 0 if the game is not over
*/
int isGameOver(int game_board[rows][cols], int piece) {
    
    // ITERATORS
    int r, c;
    
    // CHECK FOR HORIZONTAL WIN
    // "O" "O" "O" "O"
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols - 3; c++) {
            if (game_board[r][c] == piece && game_board[r][c + 1] == piece && game_board[r][c + 2] == piece && game_board[r][c + 3] == piece)
                return 1;
        }
    }

    // CHECK FOR VERTICAL WIN
    /* 
       "O"
       "O"
       "O"
       "O"
    */ 
    for (c = 0; c < cols; c++) {
        for (r = 0; r < rows - 3; r++) {
            if (game_board[r][c] == piece && game_board[r + 1][c] == piece && game_board[r + 2][c] == piece && game_board[r + 3][c] == piece)
                return 1;
        }
    }
    
    // CHECK FOR POSITIVE DIAGONAL WIN
    /*
                "O"
            "O"
        "O"
    "O"
    */
    for (c = 0; c < cols - 3; c++) {
        for (r = 0; r < rows - 3; r++) {
            if (game_board[r][c] == piece && game_board[r + 1][c + 1] == piece && game_board[r + 2][c + 2] == piece && game_board[r + 3][c + 3] == piece)
                return 1;
        }
    }

    // CHECK FOR NEGATIVE DIAGONAL WIN
    /*
    "O"
        "O"
            "O"
                "O"
    */
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

    // GET INPUT DIMENSIONS FROM USER
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

    // INITIALIZE USEFUL VARIABLES
    int i, j;
    int game_board[rows][cols];
    int first_available_row[cols];

    // INITIALIZE THE GAME BOARD TO EMPTY CELLS
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            game_board[i][j] = -1; // -1 means that (row, col) is available
        }
    }

    // INITIALIZE THE FIRST AVAILABLE ROW
    for (i = 0; i < cols; i++) {
        first_available_row[i] = rows - 1; // 0-indexed
    }

    // VARIABLE TO KEEP TRACK OF PLAYER TURN
    int piece = 0; // 0 means Player 1, 1 means Player 2

    print_game(game_board, piece);

    char c;
    do {
        c = getch(); // get keyboard input
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
                // THE USER PRESSED ENTER, SO LET'S PLACE THE TOKEN
                place_token(game_board, first_available_row, piece);

                // CHECK IF GAME IS OVER
                if (isGameOver(game_board, piece) == 1) {
                    printf("Player %d has won! Game over.\n", piece + 1);
                    return 0;
                }

                current_token_pos = 0; // reset token column position
                piece = (piece + 1) % 2; // update to next player's turn
        }

    system("clear");
    print_game(game_board, piece);

    } while (c != 'q'); 

    return 0;
}
