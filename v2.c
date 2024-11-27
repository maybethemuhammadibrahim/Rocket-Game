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

typedef struct {
    int x;
    int y;
    int active;
} Entity;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void game(){
    char player[PLAYER_HEIGHT][PLAYER_WIDTH] = {
        {' ', '^', ' '},
        {'/', '|', '\\'},
        {'|', ' ', '|'}
    };
    
    int playerX = SCREEN_WIDTH / 2 - 1;
    int playerY = SCREEN_HEIGHT - PLAYER_HEIGHT;
    
    Entity bullets[MAX_BULLETS] = {0};
    Entity enemies[MAX_ENEMIES] = {0};
    int score = 0;
    int gameOver = 0;
    char key;
    
    srand(time(NULL));
    
    hideCursor();
    
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
                    if (playerX > 0) playerX--;
                    break;
                case 'd':
                case 'D':
                    if (playerX < SCREEN_WIDTH - PLAYER_WIDTH) playerX++;
                    break;
                case ' ':
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
        
        for (i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].y--;
                if (bullets[i].y < 0) {
                    bullets[i].active = 0;
                }
                
                for (j = 0; j < MAX_ENEMIES; j++) {
                    if (enemies[j].active && bullets[i].x == enemies[j].x && bullets[i].y == enemies[j].y) {
                        bullets[i].active = 0;
                        enemies[j].active = 0;
                        score += 10;
                        
                        enemies[j].x = rand() % SCREEN_WIDTH;
                        enemies[j].y = 0;
                        enemies[j].active = 1;
                    }
                }
            }
        }
        
        static int enemyMoveCounter = 0;
        if (++enemyMoveCounter >= 10) {
            enemyMoveCounter = 0;
            for (i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].active) {
                    enemies[i].y++;
                    if (enemies[i].y >= SCREEN_HEIGHT) {
                        enemies[i].y = 0;
                        enemies[i].x = rand() % SCREEN_WIDTH;
                    }
                    
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
        
        
        for (i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                gotoxy(enemies[i].x, enemies[i].y);
                putchar('V');
            }
        }
      
        for (i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                gotoxy(bullets[i].x, bullets[i].y);
                putchar('|');
            }
        }
        
       
        for (i = 0; i < PLAYER_HEIGHT; i++) {
            for (j = 0; j < PLAYER_WIDTH; j++) {
                gotoxy(playerX + j, playerY + i);
                putchar(player[i][j]);
            }
        }
        
        Sleep(50); 
    }
    
    system("cls");
    printf("Game Over!\nFinal Score: %d\n", score);
    printf("Press any key to exit...");
    _getch();
    
}
void menuScreen(){

}
int main() {
    game();
    return 0;    
}