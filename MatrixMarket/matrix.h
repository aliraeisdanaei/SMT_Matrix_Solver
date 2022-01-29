#ifndef MTX
#define MTX

struct _Matrix {
    MM_typecode matcode;
    int num_rows, num_cols, num_nonzero;
    int * rows, * cols, * index_rows, * index_cols;
    long double * val;
};
typedef struct _Matrix Matrix;

#endif