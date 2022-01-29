#ifndef MTX
#define MTX

struct _Matrix {
    MM_typecode matcode;
    List * reach_first_section, * not_reach_first_section;
    int num_rows, num_cols, num_nonzero, rows_in_first_section;
    int * rows, * cols, * index_rows, * index_cols;
    long double * val;
};
typedef struct _Matrix Matrix;

#endif