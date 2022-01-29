#ifndef OPTIMISED_SOLVER
#define OPTIMISED_SOLVER

int lsolve_parr(int matrix_dimension, int* index_rows, int* index_cols, int * cols, 
    long double* val, List * reach, List * not_reach, int first_section_length, long double *b_solution);
int lsolve_by_matrix_parr (Matrix * matrix, long double * b_vector);

#endif