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
    printf("\033[0;35m");
    printf("HIGH SCORES");
    printf("\033[0m");
    for (int i = 0; i < MAX_HIGHSCORES; i++) {
        gotoxy(10, 4 + i);
        printf("%d. %s -> %d", i + 1, highScores[i].name, highScores[i].score);
    }
    
    gotoxy(10, 10);
    printf("\033[0;33m");
    printf("Press any key to return...");
    printf("\033[0m");
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