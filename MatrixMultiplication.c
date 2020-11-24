#include "MatrixMultiplication.h"
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool failed = false;

// creat matrixMult structure
matrixMult* createMatrixMult(matrix* a, matrix* b) {

    // check for NULLS
    if (a == NULL || b == NULL) return NULL;

    // assign rows and cols
    int a_row = a->x, a_col = a->y;

    int b_row = b->x, b_col = b->y;

    // check for mult
    if (a_col != b_row) {
        printf("dimensions error\n");
        return NULL;
    }


    // alloc space for c
    matrix* c = malloc(sizeof(matrix));

    // if failed
    if (c == NULL) return NULL;

    c->x = a_row;
    c->y = b_col;

    // alloc space for 2d matrix
    c->ptr = malloc(a_row * sizeof(int*));

    // if failed
    if (c->ptr == NULL) return NULL;

    for (int i = 0; i < a_row; i++) {
        c->ptr[i] = malloc(b_col * sizeof(int));

        if (c->ptr[i] == NULL) return NULL;

    }
    // build a struct to pass to thread
    matrixMult* ab = malloc(sizeof(matrixMult));

    if (ab == NULL)
        return NULL;

    ab->a = a;
    ab->b = b;
    ab->c = c;

    return ab;
}

void* computeRow(void* args) {

    // check if something is passed in
    if (args == NULL) {
        failed = true;
        pthread_exit(NULL);
    }

    // casting to row struct
    matrixMultByRow* data = (matrixMultByRow*) args;


    // extract matrices
    matrix* a = data->matrices->a;
    matrix* b = data->matrices->b;
    matrix* c = data->matrices->c;

    int row = data->requiredRow, cols = b->y;
    int row_length = a->y;



    // calculate row for c
    for (int i = 0; i < cols; i++) {
        c->ptr[row][i] = 0;
        for (int j = 0; j < row_length; j++) {
            c->ptr[row][i] += (a->ptr[row][j] * b->ptr[j][i]);
        }
    }

    // free our work
    free(args);


    pthread_exit(NULL);

}


matrix* matrixMultRow(matrix* a, matrix* b) {

    // check for nulls
    if (a == NULL || b == NULL)
        return NULL;

    int rows = a->x;
    matrixMult* data = createMatrixMult(a, b); // create matrix structure that will hold all data

    // if failed
    if (data == NULL) {
        //printf("\nCan't Multiply those matrices\n");
        return NULL;
    }

    // create threads
    pthread_t tid[rows];

    for (int i = 0; i < rows; i++) {
        matrixMultByRow* byRow = malloc(sizeof(matrixMultByRow));
        byRow->requiredRow = i;
        byRow->matrices = data;

        pthread_create(&tid[i], NULL, computeRow, byRow);

        //free(byRow);
    }

    // wait for all threads to finish
    for (int i = 0; i < rows; i++) {
        pthread_join(tid[i], NULL);
    }

    // if one thread failed
    if (failed) {
        failed = false;
        free(data);
        return NULL;
    }

/*debug
    printf("\n");
    for (int i = 0; i < 2; i++) {

        for (int j = 0; j < 2; j++) {
            printf("%d ", data->c->ptr[i][j]);

        }
        printf("\n");

    }
//enddebug*/
    matrix* c  = data->c;
    free(data); // free some pointers

    return c;
}



void* computeCell(void* args) {

    // check for parameters
    if (args == NULL) {
        failed = true;
        pthread_exit(NULL);
    }


    // cast to cell struct
    matrixMultByCell* data = (matrixMultByCell*) args;


    //extract matrices
    matrix* a = data->matrices->a;
    matrix* b = data->matrices->b;
    matrix* c = data->matrices->c;

    int row = data->requiredRow, col = data->requiredCol;
    int row_length = a->y;

    // calculate the cell
    c->ptr[row][col] = 0;
    for (int i = 0; i < row_length; i++) {
        c->ptr[row][col] += a->ptr[row][i] * b->ptr[i][col];
    }

    //free our work
    free(args);

    pthread_exit(NULL);
}


matrix* matrixMultCell(matrix* a, matrix* b) {

    // check for NULLS
    if (a == NULL || b == NULL)
        return NULL;

    int rows = a->x, cols = b->y;
    matrixMult* data = createMatrixMult(a, b);

    //if failed
    if (data == NULL) {
        //printf("\nCan't Multiply those matrices\n");
        return NULL;
    }

    pthread_t tid[rows * cols];

    // create threads to compute cells
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrixMultByCell* byCell = malloc(sizeof(matrixMultByCell));
            byCell->requiredRow = i;
            byCell->requiredCol = j;
            byCell->matrices = data;

            pthread_create(&tid[i * cols + j], NULL, computeCell, byCell);

          //  free(byCell);
        }
    }

    // wait for threads
    for (int i = 0; i < rows * cols; i++) {
        pthread_join(tid[i], NULL);
    }

    //if one thread failed
    if (failed) {
        failed = false;
        free(data);
        return NULL;
    }
    /*
//debug
    printf("\n");
    for (int i = 0; i < 2; i++) {

        for (int j = 0; j < 2; j++) {
            printf("%d ", data->c->ptr[i][j]);

        }
        printf("\n");

    }
//enddebug*/

    matrix* c  = data->c;
    free(data);

    return c;

}
