#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "print.h"

#define BUFFER_SIZE 1024



int readInt(char* s) {

    if (s == NULL)
        return 0;

    int num = 0;
    //printf("hey");
    while (*s != '\n' && *s != '\0') {
        if (*s > '9' || *s < '0') {
            break;
        }
        else {
            num *= 10;
            num += (*s - '0');
        }
        s++;
    }

    return num;

}


matrix* readMatrix(char* file) {

    FILE *fptr = fopen(file, "r+");

    if (fptr == NULL)
        return NULL;

    char buffer[BUFFER_SIZE];

    if (fgets(buffer, BUFFER_SIZE, fptr) == NULL) {
        return NULL;
    }


    char* row;
    char* col;
    if ((row = strstr(buffer, "row=")) == NULL || (col = strstr(buffer, "col=")) == NULL) {
        return NULL;
    }


    row += 4; col += 4;
    int r = readInt(row), c = readInt(col);

    int** mat = malloc(r * sizeof(int*));

    for (int i = 0; i < r; i++) {
        mat[i] = malloc(c * sizeof(int));
        for (int j = 0; j < c; j++) {
            mat[i][j] = 0;
            if (fscanf(fptr, "%d", &mat[i][j]) == 0)
                return NULL;
        }
    }

    matrix* x = malloc(sizeof(matrix));

    if (x == NULL) return x;

    x->ptr = mat;
    x->x = r;
    x->y = c;

    return x;

    fclose(fptr);

}
bool writeMatrix(char* file, matrix* c) {

    FILE* fptr = fopen(file, "a+");

    if (fptr == NULL || c == NULL) {
        printf("Cant write to the file\n");
        return false;
    }

    fprintf(fptr, "row=%d col=%d\n", c->x, c->y);

    for (int i = 0; i < c->x; i++) {

        for (int j = 0; j < c->y; j++) {
            fprintf(fptr,"%d ", c->ptr[i][j]);
        }
        fprintf(fptr, "\n");
    }
    fprintf(fptr, "\n");

    fclose(fptr);
    return true;

}


bool clearFile(char* file) {

    FILE* fptr = fopen(file, "w+");

    if (fptr == NULL) {
        printf("Cant write to the file\n");
        return false;
    }

    fprintf(fptr, "");
    fclose(fptr);
    return true;
}
