#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include "MatrixStruct.h"
#include "print.h"
#include "MatrixMultiplication.h"





int main(int argc, char** argv)
{
    // initialize array of pointers to files name
    char** files = malloc(3 * sizeof(char*));

    // default values
    files[0] = "a.txt"; files[1] =  "b.txt"; files[2] =  "c.out";

    // update if files are provided in order
    for (int i = 1; i < argc; i++) {
        files[i - 1] = argv[i];
    }

    // create matrices
    matrix* a = readMatrix(files[0]);
    matrix* b = readMatrix(files[1]);


    // if there's wrong in reading files
    if (a == NULL || b == NULL) {
        printf("Cant read the file\n");
        return 1;
    }

    // try to clear file which will contains result
    if (clearFile(files[2])) {

        // struct of times
        struct timeval start, end;

        // start by row
        printf("By Row ..\n");
        printf("Number of  threads : %d\n", a->x);

        // start timing
        gettimeofday(&start, NULL);

        // if write is done successfully
        matrix *c = matrixMultRow(a, b);
        if (c != NULL)
        {
            gettimeofday(&end, NULL);
            printf("Seconds taken %lu\n", end.tv_sec - start.tv_sec);
            printf("Microseconds taken %lu\n", end.tv_usec - start.tv_usec);
            writeMatrix(files[2], c);


            for (int i = 0; i < c->x; i++) {
                free(c->ptr[i]);
            }

            free(c->ptr);
            free(c);
        }
        else
            printf("Can not perform multiplication\n");



        // second method
        printf("\n\nBy Cell ..\n");
        printf("Number of  threads : %d\n", a->x * b->y);


        // start timing
        gettimeofday(&start, NULL);

        c = matrixMultCell(a, b);

        if (c != NULL) {

            gettimeofday(&end, NULL);
            printf("Seconds taken %lu\n", end.tv_sec - start.tv_sec);
            printf("Microseconds taken %lu\n", end.tv_usec - start.tv_usec);
            writeMatrix(files[2], c);

            for (int i = 0; i < c->x; i++) {
                free(c->ptr[i]);
            }

            free(c->ptr);
            free(c);

        }
        else
            printf("Can not perform multiplication\n");


    }


    // free all my heap spaces
    free(files);

    for (int i = 0; i < a->x; i++) {
        free(a->ptr[i]);
    }

    for (int i = 0; i < b->x; i++) {
        free(b->ptr[i]);
    }

    free(a->ptr); free(b->ptr);
    free(a); free(b);


    return 0;
}
