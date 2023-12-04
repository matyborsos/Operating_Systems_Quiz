#ifndef PARSE_C
#define PARSE_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parson/parson.c"
#include "quiz.h"

// Function to replace HTML entities with corresponding characters
char* replaceHtmlEntities(const char* input) {
    // Implement your HTML entity replacement logic here
    // This is a simplified example, you may need a more comprehensive solution
    // Consider using a library for a more robust solution

    char* output = strdup(input);
    // Replace &quot; with "
    char* pos = strstr(output, "&quot;");
    while (pos != NULL) {
        memcpy(pos, "\"", 1);
        memmove(pos + 1, pos + 6, strlen(pos + 6) + 1);
        pos = strstr(output, "&quot;");
    }
    pos = strstr(output, "&#039;");
    while (pos != NULL) {
        memcpy(pos, "'", 1);
        memmove(pos + 1, pos + 6, strlen(pos + 6) + 1);
        pos = strstr(output, "&#039;");
    }

    pos = strstr(output, "&lt;");
    while (pos != NULL) {
        memcpy(pos, "<", 1);
        memmove(pos + 1, pos + 4, strlen(pos + 4) + 1);
        pos = strstr(output, "&lt;");
    }

    // Replace &gt; with >
    pos = strstr(output, "&gt;");
    while (pos != NULL) {
        memcpy(pos, ">", 1);
        memmove(pos + 1, pos + 4, strlen(pos + 4) + 1);
        pos = strstr(output, "&gt;");
    }

    return output;
}

int parse(quiz_t *quiz, char *msg) {
    JSON_Value *json_value = json_parse_string_with_comments(msg);

    if (json_value == NULL) {
        fprintf(stderr, "JSON parsing failed\n");
        return -1;
    }

    JSON_Object *root_object = json_value_get_object(json_value);

    JSON_Array *results_array = json_object_get_array(root_object, "results");

    if (results_array == NULL) {
        json_value_free(json_value);
        return -1;
    }

    JSON_Object *results_object = json_array_get_object(results_array, 0);

    if (results_object == NULL) {
        fprintf(stderr, "No object found in 'results' array\n");
        json_value_free(json_value);
        return -1;
    }

    const char *question = json_object_get_string(results_object, "question");
    const char *correct_answer = json_object_get_string(results_object, "correct_answer");

    JSON_Array *incorrect_answers_array = json_object_get_array(results_object, "incorrect_answers");

    if (incorrect_answers_array == NULL) {
        fprintf(stderr, "No 'incorrect_answers' array found in JSON\n");
        json_value_free(json_value);
        return -1;
    }

    for (size_t i = 0; i < 3; ++i) {
        const char *ians = json_array_get_string(incorrect_answers_array, i);
        char* Answer = replaceHtmlEntities(ians);
        quiz->choices[i] = strdup(Answer);
    }

    printf("\n");

    // Replace HTML entities in the question
    char* decodedQuestion = replaceHtmlEntities(question);
    char* decodedAnswer = replaceHtmlEntities(correct_answer);

    // Allocate memory for quiz structure members
    quiz->question = strdup(decodedQuestion);
    quiz->answer = strdup(decodedAnswer);

    if (quiz->question == NULL || quiz->answer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        json_value_free(json_value);
        return -1;  // Return an error code
    }

    json_value_free(json_value);

    return 0;  // Return success
}

void free_quiz_data(quiz_t *quiz) {
    // Free allocated memory for question and answer
    free(quiz->question);
    free(quiz->answer);

    // Free allocated memory for choices
    for (size_t i = 0; i < 3; ++i) {
        free(quiz->choices[i]);
    }
}

#endif // PARSE_C
