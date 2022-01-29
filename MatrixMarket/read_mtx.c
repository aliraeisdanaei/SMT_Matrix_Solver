#include <stdio.h>
#include <stdlib.h>

#include "mmio.h"
#include "matrix.h"

/**
 * Writes the matrix read in mtx format
 */
void write_out_mtx(Matrix * matrix){
    int i;
    mm_write_banner(stdout, matrix->matcode);
    mm_write_mtx_crd_size(stdout, matrix->num_rows, matrix->num_cols, matrix->num_nonzero);
    for (i = 0; i < matrix->num_nonzero; i++){
        fprintf(stdout, "%d %d %20.19Le\n", matrix->rows[i] + 1, matrix->cols[i] + 1, matrix->val[i]);
    }
}

/**
 * Write out the row and col indexes
 */
void write_out_indexes(Matrix * matrix){
    int i;
    printf("Rows: \n");
    for(i = 0; i < matrix->num_rows; i++){
        fprintf(stdout, "%d\n", matrix->index_rows[i]);
    }
    printf("Cols: \n");
    for(i = 0; i < matrix->num_cols; i++){
        fprintf(stdout, "%d\n", matrix->index_cols[i]);
    }
}

long double * get_b_vector(Matrix * matrix){ 
    long double * b_vector = (long double *) malloc(matrix->num_rows * sizeof(long double));
    int i;
    for(i = 0; i < matrix->num_rows; b_vector[i++] = 0.0);
    for(i = 0; i < matrix->num_nonzero; i++) b_vector[matrix->rows[i]] = matrix->val[i];
    free(matrix);
    return b_vector;
}

/**
 * Reads the the mtx file in the coordinate form from argv at the index i_arg, 
 * and outputs a completed struct of the Matrix
 * argc - the number of arguments
 * argv - the arguments
 * i_arg - the index of argv that the argument occurs in (must be > 1)
 * exits if no file given, or the file was invalid
 */
Matrix * read_mtx_file(int argc, char *argv[], int i_arg){
    FILE *f;
    if (argc < i_arg + 1){
		fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
		exit(1);
	}else{ 
        if ((f = fopen(argv[i_arg], "r")) == NULL){
            exit(1);
        }
    }

    Matrix * matrix = (Matrix *) malloc(sizeof(Matrix));
    matrix->num_rows = 0;
    matrix->num_cols = 0;
    matrix->num_nonzero = 0;

    int ret_code, i, i_row, i_col;
    long double tmp_val;

    if (mm_read_banner(f, &matrix->matcode) != 0){
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    if (mm_is_complex(matrix->matcode) && mm_is_matrix(matrix->matcode) && 
            mm_is_sparse(matrix->matcode)){
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matrix->matcode));
        exit(1);
    }

    // find out size of sparse matrix ....
    if ((ret_code = mm_read_mtx_crd_size(f, &matrix->num_rows, &matrix->num_cols, &matrix->num_nonzero)) !=0){
        exit(1);
    }

    matrix->rows = (int *) malloc(matrix->num_nonzero * sizeof(int));
    matrix->cols = (int *) malloc(matrix->num_nonzero * sizeof(int));
    matrix->val = (long double *) malloc(matrix->num_nonzero * sizeof(long double));

    matrix->index_cols = (int *) malloc(matrix->num_cols * sizeof(int));
    matrix->index_rows = (int *) malloc(matrix->num_rows * sizeof(int));

    for(i = 0; i < matrix->num_rows; matrix->index_rows[i++] = -1);
    for(i = 0; i < matrix->num_cols; matrix->index_cols[i++] = -1);

    int j = 0;
    for (i = 0; i < matrix->num_nonzero; i++){
        fscanf(f, "%d %d %Le\n", &i_row, &i_col, &tmp_val);

        matrix->rows[i] = i_row - 1;  
        matrix->cols[i] = i_col - 1;
        matrix->val[i] = tmp_val;

        if(matrix->index_rows[matrix->rows[i]] == -1){
            matrix->index_rows[matrix->rows[i]] = i;
        }

        if(matrix->index_cols[matrix->cols[i]] == -1){
            matrix->index_cols[matrix->cols[i]] = i;
        }
    }
    matrix->num_nonzero -= j;

    if (f !=stdin) fclose(f);
    return matrix;
}

/**
 * Tests the reading function
 * argc - the number of arguments
 * argv - the arguments
 */
void test_read_mtx(int argc, char *argv[]){
    Matrix * matrix = read_mtx_file(argc, argv, 1);
    write_out_mtx(matrix);
    write_out_indexes(matrix);
}