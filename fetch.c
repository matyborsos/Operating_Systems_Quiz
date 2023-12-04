#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "quiz.h"

static void curl(int sfd, char* url)
{
    if (dup2(sfd, STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    // Redirect standard error to /dev/null
    int devNull = open("/dev/null", O_WRONLY);
    if (devNull == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (dup2(devNull, STDERR_FILENO) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    close(devNull); // Close the file descriptor since it's no longer needed

    execlp("curl", "curl", "-s", url, (char*) NULL);
    perror("execl");
    exit(EXIT_FAILURE);
}

char* fetch(char* url) {
    int pfd[2];
    pid_t pid;

    if (pipe(pfd) == -1) {
        perror("pipe");
        return NULL;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pfd[0]);
        close(pfd[1]);
        return NULL;
    }

    if (pid == 0) {
        close(pfd[0]);

        curl(pfd[1], url);

        close(pfd[1]);

    } else {
        close(pfd[1]);

        char response[1024];
        ssize_t bytesRead = read(pfd[0], response, sizeof(response) - 1);

        response[bytesRead] = '\0';

        close(pfd[0]);

        int status;
        waitpid(pid, &status, 0);

        // Check if the child process exited successfully
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {

            char *result = (char *)malloc(bytesRead + 1);
            if (!result) {
                fprintf(stderr, "Memory allocation error\n");
                return NULL;
            }
            strncpy(result, response, bytesRead + 1);

            return result;
        } else {
            fprintf(stderr, "Child process failed\n");
            return NULL;
        }
    }
    return NULL;
}
