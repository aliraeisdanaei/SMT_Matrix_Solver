#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <omp.h>

#include "MatrixMarket/mmio.h"
#include "MatrixMarket/matrix.h"
#include "MatrixMarket/read_mtx.h"

int lsolve_parr(int matrix_dimension, int* index_rows, int* index_cols, int * cols, long double* val, long double *b_solution){
    if (!index_cols || !index_rows || !b_solution) return 0;

    // Index of both the row and the col that is being solved during the iteration
    int i_matrix;

    // Index of the col during the elimination of known values within next row
    int i_col;
    long double known_values; 
    int start_known, end_known;
    int num_parrallel = 0;


    for (i_matrix = 0; i_matrix < matrix_dimension - 1; i_matrix++){
        // solve the current col
        if(b_solution[i_matrix] != 0){
            b_solution[i_matrix] /= val[index_cols[i_matrix]];

            // if(i_matrix < matrix_dimension){
                start_known = index_cols[i_matrix] + 1;
                end_known = index_cols[i_matrix + 1];

                // if(end_known - start_known > 150){
                //     num_parrallel++;
                //     #pragma omp parrallel private(known_values)
                //     {
                //         // for the next row subtract from b all of the values of the cols that we already know
                //         known_values = 0;
                //         #pragma omp for
                //         for (i_col = start_known; i_col < end_known; i_col++){
                //             known_values += val[i_col] * b_solution[cols[i_col]];
                //         }
                //         #pragma omp critical
                //         {
                //             b_solution[i_matrix + 1] -= known_values;
                //         }
                //     }
                // }else{
                    // for the next row subtract from b all of the values of the cols that we already know
                    known_values = 0;
                    for (i_col = start_known; i_col < end_known; i_col++){
                        known_values += val[i_col] * b_solution[cols[i_col]];
                    }
                    b_solution[i_matrix + 1] -= known_values;
                // }
            // } 
        } 
    }
    b_solution[matrix_dimension - 1] /= val[index_cols[matrix_dimension - 1]];

    printf("Doing parrallel %d times\n", num_parrallel);
    return 1;
}

int lsolve_by_matrix_parr(Matrix * matrix, long double * b_vector){
    return lsolve_parr(matrix->num_cols, matrix->index_rows, matrix->index_cols, matrix->cols, matrix->val, b_vector);
}