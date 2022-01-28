#ifndef READ_MTX
#define READ_MTX

typedef struct Matrix Matrix;



void free_matrix(struct Matrix matrix);


/**
 * Writes the matrix read in mtx format
 */
void write_out_mtx(struct Matrix matrix);

/**
 * Write out the row and col indexes
 */
void write_out_indexes(struct Matrix matrix);


long double * get_b_vector(struct Matrix matrix);

/**
 * Reads the mtx file from the i_arg index, and outputs a completed struct of the Matrix
 * argc - the number of arguments
 * argv - the arguments
 * i_arg - the index of argv that the argument occurs in (must be > 1)
 * exits if no file given, or the file was invalid
 */
struct Matrix read_mtx_file(int argc, char *argv[], int i_arg);

/**
 * Tests the reading function
 * argc - the number of arguments
 * argv - the arguments
 */
void test(int argc, char *argv[]);

#endif