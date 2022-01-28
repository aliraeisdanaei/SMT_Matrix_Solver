#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>

#include "MatrixMarket/read_mtx.h"
#include "MatrixMarket/mmio.h"
#include "MatrixMarket/matrix.h"

/*
* Lower triangular solver val=b
* L is stored in the compressed column storage format
* Inputs are:
* n : the matrix dimension
* index_cols : the column pointer of L
* index_rows : the row index of L
* val : the values of L
* In/Out:
* b_solution : the right hand-side b at start and the solution x at the end.
*/
int lsolve (int matrix_dimension, int* index_rows, int* index_cols, int * cols, long double* val, long double *b_solution){
    if (!index_cols || !index_rows || !b_solution) return 0;

    // Index of both the row and the col that is being solved during the iteration
    int i_matrix;

    // Index of the col during the elimination of known values within next row
    int i_col;

    for (i_matrix = 0; i_matrix < matrix_dimension; i_matrix++){
        // solve the current col
        if(b_solution[i_matrix] != 0){
            // printf("b[%d] = %Le * %Le\n", i_matrix, val[index_cols[i_matrix]], b_solution[i_matrix] / val[index_cols[i_matrix]]);
            b_solution[i_matrix] /= val[index_cols[i_matrix]];

            if(i_matrix < matrix_dimension){
                // for the next row subtract from b all of the values of the cols that we already know
                for (i_col = index_cols[i_matrix] + 1; i_col < index_cols[i_matrix + 1]; i_col++){
                    b_solution[i_matrix + 1] -= val[i_col] * b_solution[cols[i_col]];
                }
            } 
        } 
    }
    return 1;
}

int lsolve_by_matrix (Matrix matrix, long double * b_vector){
    return lsolve(matrix.num_cols, matrix.index_rows, matrix.index_cols, matrix.cols, matrix.val, b_vector);
}