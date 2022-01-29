#ifndef NAIVE_SOLVER
#define NAIVE_SOLVER

int lsolve (int matrix_dimension, int* index_cols, int * cols, long double* val, long double *b_solution);
int lsolve_by_matrix (Matrix matrix, long double * b_vector);

#endif