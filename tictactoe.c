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

// Maze functions begin
char wall=254;
void Clear_Console();
void delay(int number_of_seconds);
int isValid(int x, int y,int size);
int isValidCell(int x, int y,int size,char maze[20][20]);
int solveMazeDFS(int size,char maze[size][size], int x, int y, int goalX, int goalY,int startX,int startY);
void Maze_Generate(int size,char maze[size][size], char travelled[size][size]);
void Prims(int size,char maze[size][size], char travelled[size][size], int x_coordinate, int y_coordinate);
void Update_Loading(int loading, int totalCells);
void displayMaze(int size, char maze[size][size],int x_coordinate,int y_coordinate);


int maze(){
     int size;

    Clear_Console();

    printf("Enter size of the maze: ");
    scanf("%d",&size);

    if(size<=0){
        Clear_Console();
        printf("!! Please enter a Valid Size !!\n");
        printf("Enter size of the maze: ");
        scanf("%d",&size);
    }

    if(size%2==0){
        size--;
    }

    printf("\n");

    char maze[size][size];
    char travelled[size][size];

    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(i==0 || i==size-1 || j==0 || j==size-1){
                maze[i][j]=wall;
            }
            else if(j%2==1 && i%2==1){
                maze[i][j]=' ';
            }
            else{
                maze[i][j]=wall;
            }
            if(i==1 && j==1){
                maze[i][j]='S';
            }
            if(i==size-2 && j==size-2){
                maze[i][j]='E';
            }
        }
    }

    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(i==0 || i==size-1 || j==0 || j==size-1){
                travelled[i][j]=wall;
            }
            else if(i%2==1 || i%2==0 && j%2==1){
                travelled[i][j]='t';
            }
            else{
                travelled[i][j]=wall;
            }
            if(i==1 && j==1){
                travelled[i][j]='S';
            }
        }
    }

    Maze_Generate(size,maze,travelled);

    char input;

    printf("Do want to Solve this maze?(Y/N/A)\nType Y for Yes\nType N for no\nType A for the programme to generate an automated solution\n ");
    scanf(" %c",&input);
    getchar();

    if(input=='y' || input=='Y'){

        int x_coordinate = 1;
        int y_coordinate = 1;

        maze[x_coordinate][y_coordinate] = 'S';

        Prims(size, maze, travelled, x_coordinate, y_coordinate);

        maze[size - 2][size - 2] = 'E';

        displayMaze(size, maze,x_coordinate, y_coordinate); // Display initial maze

        // Maze solving loop
        char move;
        while (maze[x_coordinate][y_coordinate] != 'E') {
            printf("\nEnter move (W/A/S/D to move, Q to quit): ");
            scanf(" %c", &move);

            switch (move) {
                case 'W':
                case 'w':
                    if (maze[x_coordinate - 1][y_coordinate] != wall) {
                        maze[x_coordinate][y_coordinate] = ' ';
                        x_coordinate--;
                    }
                    break;
                case 'A':
                case 'a':
                    if (maze[x_coordinate][y_coordinate - 1] != wall) {
                        maze[x_coordinate][y_coordinate] = ' ';
                        y_coordinate--;
                    }
                    break;
                case 'S':
                case 's':
                    if (maze[x_coordinate + 1][y_coordinate] != wall) {
                        maze[x_coordinate][y_coordinate] = ' ';
                        x_coordinate++;
                    }
                    break;
                case 'D':
                case 'd':
                    if (maze[x_coordinate][y_coordinate + 1] != wall) {
                        maze[x_coordinate][y_coordinate] = ' ';
                        y_coordinate++;
                    }
                    break;
                case 'Q':
                case 'q':
                    printf("Quitting...\n");
                    return 0;
                default:
                    printf("Invalid move. Please enter W/A/S/D to move, or Q to quit.\n");
                    break;
            }

            displayMaze(size, maze,x_coordinate, y_coordinate); // Display updated maze
        }

        printf("\nCongratulations! You reached the exit!\n");
    }
    else if(input=='n' || input=='N'){
        return 0;
    }

    else if(input=='A' || input=='a')
    {
          if (solveMazeDFS(size,maze, 1, 1, size-2, size-2, 1, 1))
         printf("Solved Maze:\n\n\n");

    else
    {
        printf("No solution found.\n");
    }

    Clear_Console();
    displayMaze(size, maze,1,1);
    getch();

    }
}

void Maze_Generate(int size,char maze[size][size], char travelled[size][size]){

    srand(time(NULL));

    int hold,loading=0;

    if(size<10){
        hold=5;
    }
    else if(size>10 && size<30){
        hold=3;
    }
    else if(size>30 && size<50){
        hold=2;
    }
    else{
        hold=1;
    }

    int x_coordinate = 1;
    int y_coordinate = 1;

    maze[x_coordinate][y_coordinate] = 'S';

    Prims(size,maze, travelled, x_coordinate, y_coordinate);

    maze[size-2][size-2] = 'E';

    printf(" Please wait while your maze is generating (%d %%)\n\n",(loading/(size*size))*100);


    for(int n = 0; n<size; n++)
    {
        for(int k = 0; k<size; k++)
        {
            if(n==0 && k==0){
                printf(" ");
            }
            printf("%c ", maze[n][k]);
            loading++;
            Update_Loading(loading, size*size);
            Sleep(hold);
        }

        printf("%c ", '\n');
    }

    printf("\033[3;K");
    printf("\nMaze generated successfully!\n\n");

}


void Update_Loading(int loading, int totalCells){

    printf("\033[s");
    printf("\033[3;45H");
    printf("\rMaze Completion (%.2f %%)                           ", (loading / (float)totalCells) * 100);

    printf("\033[u");

    fflush(stdout);

}

void displayMaze(int size, char maze[size][size], int x_coordinate, int y_coordinate) {
    Clear_Console();
    for (int n = 0; n < size; n++) {
        for (int k = 0; k < size; k++) {
            if(n==0 && k==0){
                printf(" ");
            }
            if (n == x_coordinate && k == y_coordinate) {
                printf("\033[0;31m");
                printf("X ");
                 printf("\033[0m");
            }
            else
            {
               if(maze[n][k]=='X')
               printf("\033[0;31m");

                printf("%c ", maze[n][k]);
                printf("\033[0m");
            }
        }
        printf("%c ", '\n');
    }
}

void Clear_Console(){

    system("cls");

}

void Prims(int size,char maze[size][size], char travelled[size][size], int x_coordinate, int y_coordinate) {

    int randomnumber = rand()%4;

    while(travelled[x_coordinate-1][y_coordinate-1] == 't'  || travelled[x_coordinate][y_coordinate-1] == 't'  || travelled[x_coordinate+1][y_coordinate-1] == 't'  || travelled[x_coordinate-1][y_coordinate] == 't'  || travelled[x_coordinate][y_coordinate] == 't'  || travelled[x_coordinate+1][y_coordinate] == 't'  || travelled[x_coordinate-1][y_coordinate+1] == 't'  || travelled[x_coordinate][y_coordinate+1] == 't'  || travelled[x_coordinate+1][y_coordinate+1] == 't' ){

        randomnumber = rand()%4;

        if(randomnumber == 0){

            if((x_coordinate >1) && (travelled[x_coordinate-2][y_coordinate] == 't' )){

                x_coordinate--;

                maze[x_coordinate][y_coordinate] = ' ';

                travelled[x_coordinate][y_coordinate]= ' ';

                x_coordinate--;

                travelled[x_coordinate][y_coordinate]= ' ';

                Prims(size,maze,travelled, x_coordinate, y_coordinate);

                x_coordinate+=2;
            }

            else if(travelled[x_coordinate-2][y_coordinate] != 't' ){

                travelled[x_coordinate-1][y_coordinate] = wall;

            }
        }

        else if(randomnumber == 1) {

            if((x_coordinate <size-2) && (travelled[x_coordinate+2][y_coordinate] == 't' )) {

                x_coordinate++;
                maze[x_coordinate][y_coordinate] = ' ';
                travelled[x_coordinate][y_coordinate]= ' ';
                x_coordinate++;
                travelled[x_coordinate][y_coordinate]= ' ';
                Prims(size,maze,travelled, x_coordinate, y_coordinate);
                x_coordinate-=2;
            }

            else if(travelled[x_coordinate+2][y_coordinate] != 't' ){

                travelled[x_coordinate+1][y_coordinate] = wall;

            }
        }

        else if(randomnumber == 2) {

            if((y_coordinate >1) && (travelled[x_coordinate][y_coordinate-2] == 't' )) {

                y_coordinate--;
                maze[x_coordinate][y_coordinate] = ' ';
                travelled[x_coordinate][y_coordinate]= ' ';
                y_coordinate--;
                travelled[x_coordinate][y_coordinate]= ' ';
                Prims(size,maze,travelled, x_coordinate, y_coordinate);
                y_coordinate+=2;
            }

            else if(travelled[x_coordinate][y_coordinate-2] != 't' ) {

                travelled[x_coordinate][y_coordinate-1] = wall;

            }
        }

        else {

            if((y_coordinate <size-2) && (travelled[x_coordinate][y_coordinate+2] == 't' )) {
                y_coordinate++;
                maze[x_coordinate][y_coordinate] = ' ';
                travelled[x_coordinate][y_coordinate]= ' ';
                y_coordinate++;
                travelled[x_coordinate][y_coordinate]= ' ';
                Prims(size,maze,travelled, x_coordinate, y_coordinate);
                y_coordinate-=2;
            }

            else if(travelled[x_coordinate][y_coordinate+2] != 't' ) {

                travelled[x_coordinate][y_coordinate+1] = wall;

            }
        }
    }
}

int isValid(int x, int y,int size) {
    return (x >= 0 && x < size && y >= 0 && y < size);
}

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

int isValidCell(int x, int y,int size,char maze[20][20]) {
    if(x >= 0 && x < size && y >= 0 && y < size && maze[x][y] == wall)
    return 1;

    else
    return 0;
}

int solveMazeDFS(int size,char maze[size][size], int x, int y, int goalX, int goalY,int startX,int startY) {

// for animation effct
    system("cls");
  displayMaze(size, maze, x, y);
    delay(0.01);

    if (!isValid(x, y,size) || maze[x][y] == wall || maze[x][y] == '.') {
        return 0;
    }


    if (x == goalX && y == goalY) {
        maze[x][y] = 'X';
        return 1;
    }

   // '.'  means path checked/in process of checking
    maze[x][y] = '.';

    // Explore neighbors in a specific order: up, right, down, left
    int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    for (int i = 0; i < 4; i++) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];

        if (solveMazeDFS(size,maze, newX, newY, goalX, goalY,startX,startY)) {

            maze[x][y] = 'X';
            return 1;
        }
    }

    return 0;
}


int main() {
    int choice;

    do {
        // Displaying the main menu
        printf("\nWelcome to the Board Game Hub! Choose the option number for the game you wish to play.\n");
        printf("1. Tic Tac Toe\n");
        printf("2. Maze Generation and Solving\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                playtictactoe();
                break;
            case 2:
                maze();
                break;
            case 3:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Please enter a number between 1 and 4.\n");
        }
    } while (1);
    return 0;
}