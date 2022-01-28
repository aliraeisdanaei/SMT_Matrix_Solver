#ifndef MATRIX
#define MATRIX

struct Matrix {
    MM_typecode matcode;
    int num_rows, num_cols, num_nonzero;
    int * rows, * cols, * index_rows, * index_cols;
    long double * val;
};

#endif