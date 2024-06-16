#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include<limits.h>

char plr,comp;

typedef struct {
    int row, col;
} MOV;


//function for player to choose what element to start with
void choose(char* p,char* q, int initial) {
    if (initial) {
        printf("Choose your symbol (X/O) : ");
        scanf(" %c",p);

        if(*p=='X'||*p=='x') {
            *p='X';
            *q='O';
        } else if(*p=='O' || *p=='o') {
            *q='X';
            *p='O';
        } else {
            printf("Invalid response. Please enter X or O.\n");
            getchar();
            choose(p, q, initial);
        }
    } else {
        // Only prompt for input during the initial symbol selection
        *p = *q = ' ';
    }
}

// Function to display the board
void displayboard(char grid[3][3]) {
    printf("\n");
    printf("\t\t+---+---+---+\n");
    printf("\t\t| %c | %c | %c |\n", grid[0][0], grid[0][1], grid[0][2]);
    printf("\t\t+---+---+---+\n");
    printf("\t\t| %c | %c | %c |\n", grid[1][0], grid[1][1], grid[1][2]);
    printf("\t\t+---+---+---+\n");
    printf("\t\t| %c | %c | %c |\n", grid[2][0], grid[2][1], grid[2][2]);
    printf("\t\t+---+---+---+\n");
    printf("\n");
}

// Function to check if any move is possible (possible = 1, not possible = 0)
int gameOn(char grid[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i][j] == ' ') return 1;
    return 0;
}

// Function to evaluate the current grid for any result
int evaluate(char grid[3][3]) {
    // Checking for rows, columns, and diagonals for victory.
    for (int row = 0; row < 3; row++) {
        if (grid[row][0] == grid[row][1] && grid[row][1] == grid[row][2]) {
            if (grid[row][0] == comp) return 10;
            else if (grid[row][0] == plr) return -10;
        }
    }

    for (int col = 0; col < 3; col++) {
        if (grid[0][col] == grid[1][col] && grid[1][col] == grid[2][col]) {
            if (grid[0][col] == comp) return 10;
            else if (grid[0][col] == plr) return -10;
        }
    }

    if (grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) {
        if (grid[0][0] == comp) return 10;
        else if (grid[0][0] == plr) return -10;
    }

    if (grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0]) {
        if (grid[0][2] == comp) return 10;
        else if (grid[0][2] == plr) return -10;
    }

    // No winner yet
    return 0;
}


int minimax(char grid[3][3], int depth, int CompMove) {
    //Maximizer = Computer
    //Minimizer = User
    int score = evaluate(grid);

    //Maximizer wins game -> return 10
    if (score == 10)
        return score;

    //Minimizer wins the game -> return -10
    if (score == -10)
        return score;

    // No moves remaining and no win yet -> return 0
    if (!gameOn(grid))
        return 0;

    //Maximizer's move (CompMove=1)
    if (CompMove) {
        int best = -11;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                //Checking if cell is empty
                if (grid[i][j] == ' ') {
                    //Making move to check its score
                    grid[i][j] = comp;

                    //Call minimax recursively and choose the maximum value
                    if (best > minimax(grid, depth + 1, !CompMove)) {
                    //best = best
                    } else {
                    best = minimax(grid, depth + 1, !CompMove);
                    }
                    //Clear grid
                    grid[i][j] = ' ';
                }
            }
        }
        return best;
    }
    //Minimizer's move(CompMove = 0)
    else {
        int best = 11;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (grid[i][j] == ' ') {
                    //Make the move
                    grid[i][j] = plr;

                    //Call minimax recursively and choose the minimum value
                    //Depth-1 is also used where a max depth is used and it is decremented by one
                    if (best < minimax(grid, depth + 1, !CompMove)) {
                    //best = best;
                    } else {
                    best = minimax(grid, depth + 1, !CompMove);
                    }
                    // Undo the move
                    grid[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

//Function to find the best move using minimax algorithm
MOV findBestMove(char grid[3][3]) {
    int bestVal = INT_MIN;
    MOV best_move;
    best_move.row = -1;
    best_move.col = -1;

    //Evaluate minimax function for all empty cells and return the cell with optimal value.
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i][j] == ' ') {
                //Making move, noting evaluation and clearing move
                grid[i][j] = comp;
                int moveVal = minimax(grid, 0, 0);
                grid[i][j] = ' ';
                //If the value of the current move is more than best value, then update best_move
                if (moveVal > bestVal) {
                    best_move.row = i;
                    best_move.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }

    return best_move;
}

void playtictactoe() {
    printf("Welcome to Tic Tac Toe v/s THE COMPUTER. Good luck!\n");
    char grid[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    int replay = 1;

    while (replay) {
        //Start a new game
        choose(&plr, &comp,replay);
        displayboard(grid);

        while (gameOn(grid)) {
            //Player's move
            int moverow, movecolumn;
            printf("Enter row (1-3) and column (1-3) for your move: ");
            scanf("%d %d", &moverow, &movecolumn);
            if (moverow < 1 || moverow > 3 || movecolumn < 1 || movecolumn > 3 || grid[moverow - 1][movecolumn - 1] != ' ') {
            printf("Invalid move, try again.\n");
            continue;
            } else {
            grid[moverow - 1][movecolumn - 1] = plr;
            }

            displayboard(grid);
            if (evaluate(grid) == -10) {
                printf("Player wins!\n");
                break;
            }

            //Computer's move
            printf("Computer's move: \n");
            MOV best_move = findBestMove(grid);
            grid[best_move.row][best_move.col] = comp;
            displayboard(grid);
            if (evaluate(grid) == 10) {
                printf("Computer wins!\n");
                break;
            }
        }
        if (!gameOn(grid))
            printf("It's a draw!\n");


        printf("Play again? (Any character other than 0 for Yes, 0 for Exit): ");
        scanf("%d", &replay);
        while(getchar() != '\n'); // Clear input buffer
        if(replay==0) {
            printf("Goodbye!\n");
        }
        //Reset grid
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                grid[i][j] = ' ';
            }
        }
    }
}

int main() {
                playtictactoe();
            
    return 0;
}
