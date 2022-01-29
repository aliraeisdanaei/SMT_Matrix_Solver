#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <time.h>

#include "MatrixMarket/mmio.h"
#include "MatrixMarket/matrix.h"
#include "MatrixMarket/read_mtx.h"
#include "naive_solver.h"
#include "optimised_solver.h"
#include "verifier.h"

int main(int argc, char *argv[]){
    test_read_mtx(argc, argv);
    // clock_t start_time, end_time;
    // Matrix * matrix = read_mtx_file(argc, argv, 2);

    // // write_out_mtx(matrix);
    // // write_out_indexes(matrix);

    // long double * b_vector = get_b_vector(read_mtx_file(argc, argv, 3));
    // long double * b_vector_og = (long double *) malloc(matrix->num_rows * sizeof(long double));
    // memcpy(b_vector_og, b_vector, matrix->num_rows * sizeof(long double));

    // bool naive = strcmp(argv[1], "naive") == 0;

    // if(naive){
    //     start_time = clock();
    //     lsolve_by_matrix(matrix, b_vector);
    //     end_time = clock();
    //     printf("Computation Time Naive: %d\n", (int)(end_time - start_time));
    // }else{
    //     start_time = clock();
    //     lsolve_by_matrix_parr(matrix, b_vector);
    //     end_time = clock();
    //     printf("Computation Time Parr: %d\n", (int)(end_time - start_time));
    // }

    // write_sol(b_vector, matrix->num_rows);

    // // matrix_sol_verifier(matrix->num_rows, matrix->num_nonzero, matrix->index_rows, 
    // //     matrix->cols,  matrix->val, b_vector, b_vector_og);
    
    // matrix_sol_write_inacc(matrix->num_rows, matrix->num_nonzero, matrix->index_rows, 
    //     matrix->cols,  matrix->val, b_vector, b_vector_og);
}