#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED

#include <stdbool.h>
#include "MatrixStruct.h"

matrix* readMatrix(char* file);

bool writeMatrix(char* file, matrix* c);

bool clearFile(char* file);
#endif // PRINT_H_INCLUDED
