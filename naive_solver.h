#ifndef NAIVE_SOLVER
#define NAIVE_SOLVER

/*
* Lower triangular solver val=b -- single thread
* L is stored in the compressed column storage format
* n: the matrix dimension
* index_cols: the column pointer of L
* index_rows: the row index of L
* val: the values of L
* In/Out:
* b_solution : the right hand-side b at start and the solution x at the end.
 */
int lsolve (int matrix_dimension, int* index_cols, int * cols, long double* val, long double *b_solution);

/**
 * Return is lsolve, but called with the convenience of the matrix struct
 */
int lsolve_by_matrix (Matrix * matrix, long double * b_vector);

#endif