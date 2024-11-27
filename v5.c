#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 20
#define MAX_BULLETS 5
#define MAX_ENEMIES 3
#define PLAYER_HEIGHT 3
#define PLAYER_WIDTH 3
#define MAX_NAME_LENGTH 20
#define MAX_HIGHSCORES 5

typedef struct {
    int x;
    int y;
    int active;
} Entity;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
} HighScore;

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
    HighScore highScores[MAX_HIGHSCORES];
    FILE *file = fopen("highscores.txt", "r");
    int numScores = 0;

    if (file == NULL) {
        for (int i = 0; i < MAX_HIGHSCORES; i++) {
            strcpy(highScores[i].name, "---");
            highScores[i].score = 0;
        }
    } 
        else {
            while (numScores < MAX_HIGHSCORES && fscanf(file, "%s %d", highScores[numScores].name, &highScores[numScores].score) == 2) {
                numScores++;
        }
        fclose(file);
    }

    system("cls");
    
    gotoxy(10, 2);
    printf("HIGH SCORES");
    
    for (int i = 0; i < MAX_HIGHSCORES; i++) {
        gotoxy(10, 4 + i);
        printf("%d. %s -> %d", i + 1, highScores[i].name, highScores[i].score);
    }
    
    gotoxy(10, 10);
    printf("Press any key to return...");
    _getch();
}

void updateHighScores(int newScore) {
    HighScore highScores[MAX_HIGHSCORES];
    int numScores = 0;
    FILE *file;

    file = fopen("highscores.txt", "r");
    if (file == NULL) {
        for (int i = 0; i < MAX_HIGHSCORES; i++) {
            strcpy(highScores[i].name, "---");
            highScores[i].score = 0;
        }
        } 
        else {
            while (numScores < MAX_HIGHSCORES && fscanf(file, "%s %d", highScores[numScores].name, &highScores[numScores].score) == 2) {
                numScores++;
            }
            fclose(file);
    }

    int insertIndex = -1;
    for (int i = 0; i < MAX_HIGHSCORES; i++) {
        if (newScore > highScores[i].score) {
            insertIndex = i;
            break;
        }
    }

    //shifting at index
    if (insertIndex != -1) {
        for (int i = MAX_HIGHSCORES - 1; i > insertIndex; i--) {
            highScores[i] = highScores[i-1];
        }

        highScores[insertIndex].score = newScore;

        system("cls");
        gotoxy(5, 10);
        printf("You score is %d",newScore);
        gotoxy(5, 12);
        printf("Enter your name: ");
        scanf("%s", highScores[insertIndex].name);
    }

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
    
    int playerX = SCREEN_WIDTH / 2 - 1;
    int playerY = SCREEN_HEIGHT - PLAYER_HEIGHT - 1;
    
    Entity bullets[MAX_BULLETS] = {0};
    Entity enemies[MAX_ENEMIES] = {0};
    int score = 0;
    int gameOver = 0;
    char key;
    
    srand(time(NULL));
    
    hideCursor();
    
    //selecting random places for enemies to spawn
    int i, j;
    for (i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].x = rand() % SCREEN_WIDTH;
        enemies[i].y = 0;
        enemies[i].active = 1;
    }
    
    while (!gameOver) {
        system("cls");
        
        gotoxy(0, 0);
        printf("Score: %d", score);
        
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
                    // shooting bullet
                    // for (i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].active) {
                            bullets[i].x = playerX + 1;  
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
        
        // if bullet is active then update its position, no printing here
        for (i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].y--;
                if (bullets[i].y < 0) {
                    bullets[i].active = 0;
                }
                
                // here it checks collision between enemies and bullets
                for (j = 0; j < MAX_ENEMIES; j++) {
                    if (enemies[j].active && bullets[i].x == enemies[j].x && bullets[i].y == enemies[j].y) {
                        bullets[i].active = 0;
                        enemies[j].active = 0;
                        score += 10;
                        
                        // basically spawning new enemies
                        enemies[j].x = rand() % SCREEN_WIDTH;
                        enemies[j].y = 0;
                        enemies[j].active = 1;
                    }
                }
            }
        }
        
        // if enemy is active then update its position, no printing here
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
                    
                    // here it checks if the player collides with bullet
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
        
        // All the printing is done here
        // enemies
        for (i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                gotoxy(enemies[i].x, enemies[i].y);
                putchar('V');
            }
        }
        
        // bullets
        for (i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                gotoxy(bullets[i].x, bullets[i].y);
                putchar('|');
            }
        }
        
        // player
        for (i = 0; i < PLAYER_HEIGHT; i++) {
            for (j = 0; j < PLAYER_WIDTH; j++) {
                gotoxy(playerX + j, playerY + i);
                putchar(player[i][j]);
            }
        }
        
        // Control game speed
        Sleep(50);  // 50ms delay (20 FPS)
    }
    
    
    updateHighScores(score);
    
    
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
        system("cls");
        
        gotoxy(10, 3);
        printf("SPACE SHOOTER");
        
        gotoxy(15, 8);
        printf("%s Play Now", choice == 0 ? ">" : " ");
        
        gotoxy(15, 10);
        printf("%s High Scores", choice == 1 ? ">" : " ");
        
        gotoxy(15, 12);
        printf("%s Exit", choice == 2 ? ">" : " ");
        
        key = _getch();
        
        // menu navigation
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
            case '\r':  // enter key
            case ' ':
                switch (choice) {
                    case 0:  
                        game();
                        break;
                    case 1: 
                        displayHighScores();
                        break;
                    case 2:  
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
