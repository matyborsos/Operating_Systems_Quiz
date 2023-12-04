#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz.h"

extern volatile int interrupted;

// Signal handler for CTRL-C
extern void handle_interrupt;


extern int play(quiz_t *quiz) {
    quiz->max = 8;
    printf("%s\n\n", quiz->question);

    int corect = rand() % 4;

    int j = 0;

    for (int i = 0; i < 4; i++) {

        char index = 'a' + i;
        if (i == corect) {
            printf("[%c] %s\n", index, quiz->answer);
        } else {
            printf("[%c] %s\n", index, quiz->choices[j]);
            j++;
        }
    }

    printf("\n");

    char choice;
    do {

        if (interrupted) {
            printf("Game interrupted. Exiting...\n");
            break;  
        }
        
        printf("(%dpt) > ", quiz->max);
        scanf(" %c", &choice);

        if( choice == 'a' || choice == 'b' || choice == 'c' || choice == 'd'){
        if (choice == 'a' + corect) {
            printf("Congratulations! Answer [%c] is correct. Your current score is %d/%d points.\n", choice, quiz->score + quiz->max, quiz->n * 8);
            quiz->score += quiz->max;
        } else {
            printf("Answer [%c] is wrong, try again.\n", choice);
            quiz->max /= 2;
        }}
        else {
            printf("Please enter a valid option!\n");
        }
    } while (choice != 'a' + corect);

    return 0;
}
