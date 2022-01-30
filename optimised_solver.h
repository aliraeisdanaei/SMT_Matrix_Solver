#ifndef OPTIMISED_SOLVER
#define OPTIMISED_SOLVER

<<<<<<< HEAD
int lsolve_parr(int matrix_dimension, int* index_rows, int* index_cols, int * cols, 
    long double* val, List * reach, List * not_reach, int first_section_length, long double *b_solution);
int lsolve_by_matrix_parr (Matrix * matrix, long double * b_vector);
=======
void lsolve_by_matrix_parr (Matrix * matrix, long double * b_vector);
>>>>>>> f3694d8 (Finished with the project to a satisfactory level. Added a report/documentation. Added full layering feature for the optimised version. Including most of the input files in this commit as well.)

#endif