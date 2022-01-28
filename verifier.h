#ifndef VERIFIER
#define VERIFIER

bool matrix_sol_verifier(int num_rows, int num_nonzero, int * index_rows, int * cols, long double * val, long double * sol, long double * b_vector);
void matrix_sol_write_inacc(int num_rows, int num_nonzero, int * index_rows, int * cols, long double * val, long double * sol, long double * b_vector);

void write_sol(long double * sol, int length);

#endif