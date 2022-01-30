#ifndef VERIFIER
#define VERIFIER

/**
 * Return is true iff all of the rows of the val multiply with the computed solution gives the original b_value
 * Equality is measuered with the ACCEPTABLE_DIFF
 */ 
bool matrix_sol_verifier(int num_rows, int num_nonzero, int * index_rows, int * cols, long double * val, long double * sol, long double * b_vector);

/** 
 * Records all of the innaccuracies with between the b_vector and the values of rows multipled by the solution vector
 * Equality is measuered with the ACCEPTABLE_DIFF
 */
void matrix_sol_write_inacc(int num_rows, int num_nonzero, int * index_rows, int * cols, long double * val, long double * sol, long double * b_vector);

/** 
 * Writes the sol to a csv file called sol.csv
 */
void write_sol(long double * sol, int length);

#endif