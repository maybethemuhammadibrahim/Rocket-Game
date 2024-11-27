#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

// Constants
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 20
#define MAX_BULLETS 5
#define MAX_ENEMIES 3
#define PLAYER_HEIGHT 3
#define PLAYER_WIDTH 3
#define MAX_NAME_LENGTH 20
#define MAX_HIGHSCORES 5

// Structures
typedef struct {
    int x;
    int y;
    int active;
} Entity;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
} HighScore;

// Function prototypes
// void gotoxy(int x, int y);
// void hideCursor();
// void game();
// void menuScreen();
// void displayHighScores();
// void updateHighScores(int newScore);

// Function to set cursor position
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to hide cursor
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void displayHighScores() {
    // Load high scores from file
    HighScore highScores[MAX_HIGHSCORES];
    FILE *file = fopen("highscores.txt", "r");
    int numScores = 0;

    if (file == NULL) {
        // If file doesn't exist, initialize with default values
        for (int i = 0; i < MAX_HIGHSCORES; i++) {
            strcpy(highScores[i].name, "---");
            highScores[i].score = 0;
        }
    } else {
        // Read high scores from file
        while (numScores < MAX_HIGHSCORES && 
               fscanf(file, "%s %d", 
               highScores[numScores].name, 
               &highScores[numScores].score) == 2) {
            numScores++;
        }
        fclose(file);
    }

    // Clear screen
    system("cls");
    
    // Display high scores
    gotoxy(10, 2);
    printf("HIGH SCORES");
    
    for (int i = 0; i < MAX_HIGHSCORES; i++) {
        gotoxy(10, 4 + i);
        printf("%d. %s - %d", i + 1, highScores[i].name, highScores[i].score);
    }
    
    gotoxy(10, 10);
    printf("Press any key to return...");
    _getch();
}

void updateHighScores(int newScore) {
    HighScore highScores[MAX_HIGHSCORES];
    int numScores = 0;
    FILE *file;

    // Read existing high scores
    file = fopen("highscores.txt", "r");
    if (file == NULL) {
        // If file doesn't exist, initialize with default values
        for (int i = 0; i < MAX_HIGHSCORES; i++) {
            strcpy(highScores[i].name, "---");
            highScores[i].score = 0;
        }
    } else {
        // Read high scores from file
        while (numScores < MAX_HIGHSCORES && 
               fscanf(file, "%s %d", 
               highScores[numScores].name, 
               &highScores[numScores].score) == 2) {
            numScores++;
        }
        fclose(file);
    }

    // Check if new score qualifies for high scores
    int insertIndex = -1;
    for (int i = 0; i < MAX_HIGHSCORES; i++) {
        if (newScore > highScores[i].score) {
            insertIndex = i;
            break;
        }
    }

    // If score qualifies, insert and shift others
    if (insertIndex != -1) {
        // Shift lower scores down
        for (int i = MAX_HIGHSCORES - 1; i > insertIndex; i--) {
            highScores[i] = highScores[i-1];
        }

        // Insert new score
        strcpy(highScores[insertIndex].name, "NEW");
        highScores[insertIndex].score = newScore;

        // Prompt for name
        system("cls");
        gotoxy(5, 10);
        printf("You score is %d",newScore);
        gotoxy(5, 12);
        printf("Enter your name: ");
        
        // Get player name
        scanf("%s", highScores[insertIndex].name);
    }

    // Save updated high scores
    file = fopen("highscores.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < MAX_HIGHSCORES; i++) {
            fprintf(file, "%s %d\n", highScores[i].name, highScores[i].score);
        }
        fclose(file);
    }
}

void game(){
    char player[PLAYER_HEIGHT][PLAYER_WIDTH] = {
        {' ', '^', ' '},
        {'/', '|', '\\'},
        {'|', ' ', '|'}
    };
    
    // Player position
    int playerX = SCREEN_WIDTH / 2 - 1;
    int playerY = SCREEN_HEIGHT - PLAYER_HEIGHT - 1;
    
    // Game variables
    Entity bullets[MAX_BULLETS] = {0};
    Entity enemies[MAX_ENEMIES] = {0};
    int score = 0;
    int gameOver = 0;
    char key;
    
    // Initialize random seed
    srand(time(NULL));
    
    // Hide cursor
    hideCursor();
    
    // Initialize enemies
    int i, j;
    for (i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].x = rand() % SCREEN_WIDTH;
        enemies[i].y = 0;
        enemies[i].active = 1;
    }
    
    // Game loop
    while (!gameOver) {
        // Clear screen
        system("cls");
        
        // Draw score
        gotoxy(0, 0);
        printf("Score: %d", score);
        
        // Check for input
        if (_kbhit()) {
            key = _getch();
            switch (key) {
                case 'a':
                case 'A':
                    if (playerX > 0){
                        playerX--;
                    }
                    break;
                case 'd':
                case 'D':
                    if (playerX < SCREEN_WIDTH - PLAYER_WIDTH){
                        playerX++;
                    }
                    break;
                case ' ':
                    // Shoot bullet
                    // for (i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].active) {
                            bullets[i].x = playerX + 1;  // Center of player
                            bullets[i].y = playerY - 1;
                            bullets[i].active = 1;
                            break;
                        }
                    // }
                    break;
                case 'q':
                case 'Q':
                    gameOver = 1;
                    break;
            }
        }
        
        // Update bullets
        for (i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].y--;
                if (bullets[i].y < 0) {
                    bullets[i].active = 0;
                }
                
                // Check collision with enemies
                for (j = 0; j < MAX_ENEMIES; j++) {
                    if (enemies[j].active && bullets[i].x == enemies[j].x && bullets[i].y == enemies[j].y) {
                        bullets[i].active = 0;
                        enemies[j].active = 0;
                        score += 10;
                        
                        // Respawn enemy
                        enemies[j].x = rand() % SCREEN_WIDTH;
                        enemies[j].y = 0;
                        enemies[j].active = 1;
                    }
                }
            }
        }
        
        // Update enemies
        static int enemyMoveCounter = 0;
        if (++enemyMoveCounter >= 10) {
            enemyMoveCounter = 0;
            for (i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].active) {
                    enemies[i].y++;
                    if (enemies[i].y >= SCREEN_HEIGHT) {
                        enemies[i].y = 0;
                        enemies[i].x = rand() % SCREEN_WIDTH;
                        score -= 10;
                    }
                    
                    // Check collision with player
                    for (j = 0; j < PLAYER_HEIGHT; j++) {
                        for (int k = 0; k < PLAYER_WIDTH; k++) {
                            if (enemies[i].x == playerX + k && enemies[i].y == playerY + j) {
                                gameOver = 1;
                            }
                        }
                    }
                }
            }
        }
        
        // Draw game state
        // Draw enemies
        for (i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                gotoxy(enemies[i].x, enemies[i].y);
                putchar('V');
            }
        }
        
        // Draw bullets
        for (i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                gotoxy(bullets[i].x, bullets[i].y);
                putchar('|');
            }
        }
        
        // Draw player
        for (i = 0; i < PLAYER_HEIGHT; i++) {
            for (j = 0; j < PLAYER_WIDTH; j++) {
                gotoxy(playerX + j, playerY + i);
                putchar(player[i][j]);
            }
        }
        
        // Control game speed
        Sleep(50);  // 50ms delay (20 FPS)
    }
    
    // Update high scores after game ends
    updateHighScores(score);
    
    // Game over screen
    system("cls");
    printf("Game Over!");
    printf("\nFinal Score: %d\n", score);
    printf("Press any key to return to menu...");
    _getch();
}

void menuScreen(){
    int choice = 0;
    int key;
    
    while (1) {
        // Clear screen
        system("cls");
        
        // Draw game title
        gotoxy(10, 3);
        printf("SPACE SHOOTER");
        
        // Draw menu options
        gotoxy(15, 8);
        printf("%s Play Now", choice == 0 ? ">" : " ");
        
        gotoxy(15, 10);
        printf("%s High Scores", choice == 1 ? ">" : " ");
        
        gotoxy(15, 12);
        printf("%s Exit", choice == 2 ? ">" : " ");
        
        // Get user input
        key = _getch();
        
        // Handle menu navigation
        switch (key) {
            case 'w':
            case 'W':
                if(choice > 0){
                    choice = choice-1;
                }
                break;
            case 's':
            case 'S':
                if(choice < 2){
                    choice = choice+1;
                }
                break;
            case '\r':  // Enter key
            case ' ':
                // Handle menu selection
                switch (choice) {
                    case 0:  // Play Now
                        game();
                        break;
                    case 1:  // High Scores
                        displayHighScores();
                        break;
                    case 2:  // Exit
                        return;
                }
                break;
        }
    }
}

int main() {
    menuScreen();
    return 0;    
}
