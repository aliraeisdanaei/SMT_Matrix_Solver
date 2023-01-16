#ifndef READ_MTX
#define READ_MTX

/** 
 * Free the matrix and any memory allocated to fields of matrix
 */
void free_matrix(Matrix * matrix);

/**
 * Writes the matrix read in mtx format
 */
void print_mtx(Matrix * matrix);

/**
 * Prints the statistics of the lists associated with the matrix
 */
// void print_mtx_reach_stats(Matrix * matrix);

/**
 * Prints the full report of the lists associated with the matrix
 */
// void print_mtx_reach_stats(Matrix * matrix);

/**
 * Write out the row and col indexes
 */
void print_indexes(Matrix * matrix);

/**
 * Return is a long double array of the val array, with all of the indexes not present in the val filled in with zero
 * matrix is assumed to be 1 dimensional
 * the matrix is freed
 */
long double * get_b_vector(Matrix * matrix);

/**
 * Reads the mtx file from the i_arg index, and outputs a completed struct of the Matrix
 * argc - the number of arguments
 * argv - the arguments
 * i_arg - the index of argv that the argument occurs in (must be > 1)
 * exits if no file given, or the file was invalid
 */
Matrix * read_mtx_file(int argc, char *argv[], int i_arg);

/**
 * Tests the reading function
 * argc - the number of arguments
 * i_argv - the index the read file is in
 */
void test_read_mtx(int argc, char *argv[], int i_agrv);

#endif