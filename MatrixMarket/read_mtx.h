#ifndef READ_MTX
#define READ_MTX

void free_matrix(Matrix * matrix);

/**
 * Writes the matrix read in mtx format
 */
void write_out_mtx(Matrix * matrix);

/**
 * Write out the row and col indexes
 */
void write_out_indexes(Matrix * matrix);


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
 * argv - the arguments
 */
void test_read_mtx(int argc, char *argv[]);

#endif