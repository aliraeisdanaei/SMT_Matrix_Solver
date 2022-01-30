#ifndef MTX
#define MTX

struct _Matrix {
    MM_typecode matcode;
    int num_reach_threads_l1, num_reach_threads_l2;
    List ** reach_threads_l1, ** reach_threads_l2;
    List * not_reach;
    int num_rows, num_cols, num_nonzero, rows_in_first_section;
    int * rows, * cols, * index_rows, * index_cols;
    long double * val;
};
typedef struct _Matrix Matrix;

#endif