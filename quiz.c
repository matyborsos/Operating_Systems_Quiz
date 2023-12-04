// quiz.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "quiz.h"

volatile int interrupted = 0;

// Signal handler for CTRL-C
void handle_interrupt(int sig) {
    if (sig == SIGINT) {
        interrupted = 1;
    }
}

int main(void) {
    // Register signal handler for CTRL-C
    signal(SIGINT, handle_interrupt);

    int i = 1;
    quiz_t Quiz;
    Quiz.score = 0;

    while (!interrupted) {
        char *url = "https://opentdb.com/api.php?amount=1&category=18&type=multiple";
        char *response = fetch(url);

        if (response) {

            if (parse(&Quiz, response) != 0) {
                free(response);
                continue;  // Continue to the next iteration of the loop
            }
            else
            {

                Quiz.n = i;
                i++;
                    
                play(&Quiz);
                

                free(response);

                /* Free allocated memory when done with the quiz
                free_quiz_data(&Quiz);*/
            }
        } else {
            fprintf(stderr, "Failed to fetch data\n");
            break;  // Break out of the loop if fetching data fails
        }
    }

    // Show final score after the loop ends
    printf("Thanks for playing today.\n");
    printf("Your final score is %d/%d\n", Quiz.score, Quiz.n * 8);

    return EXIT_SUCCESS;
}
