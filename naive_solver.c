#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <time.h>

#include "MatrixMarket/read_mtx.h"
#include "MatrixMarket/mmio.h"
#include "MatrixMarket/matrix.h"
#include "verifier.h"

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

int lsolve_parr(int matrix_dimension, int* index_rows, int* index_cols, int * cols, long double* val, long double *b_solution){
    if (!index_cols || !index_rows || !b_solution) return 0;

    // Index of both the row and the col that is being solved during the iteration
    int i_matrix;

    // Index of the col during the elimination of known values within next row
    int i_col;

    for (i_matrix = 0; i_matrix < matrix_dimension; i_matrix++){
        // solve the current col
        if(b_solution[i_matrix] != 0){
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

int lsolve_by_matrix_parr(Matrix matrix, long double * b_vector){
    return lsolve_parr(matrix.num_cols, matrix.index_rows, matrix.index_cols, matrix.cols, matrix.val, b_vector);
}

void write_sol(long double * sol, int length){
   int i;
   FILE *fp;
   fp = fopen("sol.csv", "w");
   if(fp == NULL) exit(1);
   fprintf(fp, "Row, Sol\n");
   for(i = 0; i < length; i++){
       if(sol[i] != 0) fprintf(fp, "%d, %Le\n", i + 1, sol[i]);
   }
   fclose(fp);
}

int main(int argc, char *argv[]){
    clock_t start_time, end_time;
    struct Matrix matrix = read_mtx_file(argc, argv, 1);

    // write_out_mtx(matrix);
    // write_out_indexes(matrix);

    long double * b_vector = get_b_vector(read_mtx_file(argc, argv, 2));
    long double * b_vector_og = (long double *) malloc(matrix.num_rows * sizeof(long double));
    memcpy(b_vector_og, b_vector, matrix.num_rows * sizeof(long double));

    bool naive = true;

    if(naive){
        start_time = clock();
        lsolve_by_matrix(matrix, b_vector);
        end_time = clock();
        printf("Computation Time Naive: %f\n", (double)(end_time - start_time));
    }else{
        start_time = clock();
        lsolve_by_matrix_parr(matrix, b_vector);
        end_time = clock();
        printf("Computation Time Parr: %f\n", (double)(end_time - start_time));
    }

    write_sol(b_vector, matrix.num_rows);

    // matrix_sol_verifier(matrix.num_rows, matrix.num_nonzero, matrix.index_rows, 
    //     matrix.cols,  matrix.val, b_vector, b_vector_og);
    
    matrix_sol_write_inacc(matrix.num_rows, matrix.num_nonzero, matrix.index_rows, 
        matrix.cols,  matrix.val, b_vector, b_vector_og);
    

}