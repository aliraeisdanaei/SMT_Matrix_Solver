#ifndef NAIVE_SOLVER
#define NAIVE_SOLVER

<<<<<<< HEAD
int lsolve (int matrix_dimension, int* index_cols, int * cols, long double* val, long double *b_solution);
=======
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
>>>>>>> f3694d8 (Finished with the project to a satisfactory level. Added a report/documentation. Added full layering feature for the optimised version. Including most of the input files in this commit as well.)
int lsolve_by_matrix (Matrix * matrix, long double * b_vector);

#endif