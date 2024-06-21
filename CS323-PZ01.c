#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> 

#define MAX_QUESTIONS 50
#define MAX_OPTIONS 4
#define MAX_QUESTION_LENGTH 255
#define MAX_ANSWER_LENGTH 100
#define NUM_RANDOM_QUESTIONS 8

struct Question {
    char* question;
    char* answers[MAX_OPTIONS];
    int correct_answer_index;
};

void loadQuestionsFromFile(const char* filename, struct Question** questions, int* num_questions) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    *num_questions = 0;
    *questions = malloc(MAX_QUESTIONS * sizeof(struct Question));
    if (*questions == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_QUESTION_LENGTH];
    while (*num_questions < MAX_QUESTIONS && fgets(buffer, sizeof(buffer), file) != NULL) {
        (*questions)[*num_questions].question = _strdup(buffer);
        if ((*questions)[*num_questions].question == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < MAX_OPTIONS; ++i) {
            fgets(buffer, sizeof(buffer), file);
            (*questions)[*num_questions].answers[i] = _strdup(buffer);
            if ((*questions)[*num_questions].answers[i] == NULL) {
                perror("Memory allocation error");
                exit(EXIT_FAILURE);
            }
        }

        fscanf_s(file, "%d\n", &(*questions)[*num_questions].correct_answer_index);

        (*num_questions)++;
    }

    fclose(file);
}

void freeQuestions(struct Question* questions, int num_questions) {
    for (int i = 0; i < num_questions; ++i) {
        free(questions[i].question);
        for (int j = 0; j < MAX_OPTIONS; ++j) {
            free(questions[i].answers[j]);
        }
    }
    free(questions);
}

int getRandomQuestionIndex(int num_questions) {
    return rand() % num_questions;
}

void displayQuestion(const struct Question* question) {
    printf("Pitanje: %s", question->question);
    printf("Odgovori:\n");
    for (int i = 0; i < MAX_OPTIONS; ++i) {
        printf("%c. %s", 'A' + i, question->answers[i]);
    }
}

char convertToLowercase(char answer) {
    if (answer >= 'a' && answer <= 'z') {
        return answer - 'a' + 'A';
    }
    return answer;
}

int main() {
    struct Question* questions;
    int num_questions = 0;

    srand((unsigned int)time(NULL));

    loadQuestionsFromFile("pitanja.txt", &questions, &num_questions);

    int score = 0;
    int max_questions = NUM_RANDOM_QUESTIONS; 

    printf("Dobrodosli u Mini kviz Milioner!\n\n");

    while (max_questions > 0 && num_questions > 0) {
        int random_index = getRandomQuestionIndex(num_questions);
        struct Question* current_question = &questions[random_index];

        displayQuestion(current_question);

        printf("Unesite slovo odgovora (A, B, C ili D): ");
        char user_answer;
        scanf_s(" %c", &user_answer);

        user_answer = convertToLowercase(user_answer);

        int user_answer_index = user_answer - 'A';

        if (user_answer_index >= 0 && user_answer_index < MAX_OPTIONS) {
            if (user_answer_index == current_question->correct_answer_index) {
                printf("Tacan odgovor!\n");
                score++;
            }
            else {
                printf("Pogresan odgovor. Tacan odgovor je: %c\n", 'A' + current_question->correct_answer_index);
            }
        }
        else {
            printf("Niste uneli validan odgovor.\n");
        }

        free(questions[random_index].question);
        for (int i = 0; i < MAX_OPTIONS; ++i) {
            free(questions[random_index].answers[i]);
        }
        for (int i = random_index; i < num_questions - 1; ++i) {
            questions[i] = questions[i + 1];
        }
        num_questions--;

        max_questions--;
    }

    printf("\nKraj kviza. Osvojili ste %d poena.\n", score);

    freeQuestions(questions, num_questions);

    return 0;
}
