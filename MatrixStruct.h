#ifndef MATRIXSTRUCT_H_INCLUDED
#define MATRIXSTRUCT_H_INCLUDED

typedef struct {
    int** ptr;
    int x, y;
} matrix;

typedef struct {
    matrix* a;
    matrix* b;

    matrix* c;
} matrixMult;


typedef struct {

    matrixMult* matrices;
    int requiredRow;

} matrixMultByRow;

typedef struct {

    matrixMult* matrices;
    int requiredRow;
    int requiredCol;


} matrixMultByCell;


#endif // MATRIXSTRUCT_H_INCLUDED
