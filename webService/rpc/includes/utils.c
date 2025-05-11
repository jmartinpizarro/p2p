/*
* Implementación de las interfaces definidas en utils.c
*/


#include "utils.h"

int DEBUG = 1;
int ERROR = 1;
int TEST = 1; 
int INFO = 1; 


void printn(int number) {
    for (int i = 0; i < number; i++) {
        printf("\n");
    }
}


void print(const char *format, ...) {
    if (DEBUG) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        printf("\n");
        va_end(args);
    }
}



void printd(const char* name, const char *format, ...) {
    // Only proceed if DEBUG is true
    if (DEBUG) {
        va_list args;
        va_start(args, format);
        printf(ORANGE);
        printf("=== DEBUG %s === ",name);
        vprintf(format, args);
        printf("\n");
        printf(RESET);
        va_end(args);
    }
}


void printi(const char *name, const char *format, ...) {
    // Only proceed if DEBUG is true
    if (INFO) {
        va_list args;
        va_start(args, format);
        printf(BLUE);
        printf("=== INFO: %s === ", name);
        vprintf(format, args);
        printf("\n");
        printf(RESET);
        va_end(args);
    }
}


void printe(const char *name, const char *format) {
    // Only proceed if DEBUG is true
    if (ERROR) {
        printf(RED);
        printf("=== ERROR: %s === ", name);
        perror(format);
        printf("\n");
        printf(RESET);
        fflush(stdout);
    }
}


void printt(int result, int expected, char *name) {
    // Only proceed if DEBUG is true
    if (TEST) {
        if (result == expected) {
            printf(GREEN);
            printf("=== TEST %s === SUPERADO", name);
        } else {
            printf(RED);
            printf("=== TESTS %s === NO SUPERADO", name);
        }
        printf("\n");
        printf(RESET);
    }
}
