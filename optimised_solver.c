#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <omp.h>

#include "MatrixMarket/mmio.h"
#include "optimised_orderings.h"
#include "MatrixMarket/matrix.h"

void solve_cols_in_list (int matrix_dimension, long double* val, long double *b_solution, int ** cols_list, int list_len){
    int i_b, i_val, i_sol, i_list;

    // long double known_values; 
    // #pragma omp parallel num_threads(8)
    {
        // #pragma omp for
        for (i_list = 0; i_list < list_len; i_list++){
            printf("at least here i_list: %d\n", i_list);
            i_val = cols_list[i_list][0];
            printf("%d\n", i_val);
            i_sol = cols_list[i_list][1]; 
            printf("%d\n", i_sol);
            i_b = cols_list[i_list][2]; 
            printf("%d\n", i_b);
            // b_solution[i_b] -= val[i_val] * b_solution[i_sol];
        }
    }
}

/**
 * Solves the Left side Matrix, in the list
 * It is assumed that all the rows above the list have already been solved
 * matrix_dimension: the matrix dimension
 * index_cols: the column pointer of L
 * index_rows: the row index of L
 * val: the values of L
 * In/Out:
 * b_solution : the right hand-side b at start and the solution x at the end.
 */
void solve_in_list (int matrix_dimension, int* index_cols, int * cols, long double* val, long double *b_solution, int * list, int list_len){
    // Index of both the row and the col that is being solved during the iteration
    int i_matrix;

    // Index of the col during the elimination of known values within next row
    int i_col;

    int i_list;

    long double known_values; 
    int start_known, end_known;
    for (i_list = 0; i_list < list_len; i_list++){
        i_matrix = list[i_list];
        // printf("i_matrix = %d\n", i_matrix);
        // solve the current col
        if(b_solution[i_matrix] != 0){
            // printf("b[%d] = %Le * %Le\n", i_matrix, val[index_cols[i_matrix]], b_solution[i_matrix] / val[index_cols[i_matrix]]);
            b_solution[i_matrix] /= val[index_cols[i_matrix]];

            if(i_matrix < matrix_dimension){
                // for the next row subtract from b all of the values of the cols that we already know
                start_known = index_cols[i_matrix] + 1;
                end_known = index_cols[i_matrix + 1];
                known_values = 0;
                for (i_col = start_known; i_col < end_known; i_col++){
                    known_values += val[i_col] * b_solution[cols[i_col]];
                }
                b_solution[i_matrix + 1] -= known_values;
            } 
        } 
    }
}

/**
 * Solves the lower trinagle matrix, in accordance to the layering
 * solves the first section first, 
 * then parrelly solves each of the reach lists in layer 1, then layer 2
 * after which all rows not in the layers will be solved
 * In/Out:
 * b_solution : the right hand-side b at start and the solution x at the end.
 */
void lsolve_by_matrix_parr(Matrix * matrix, long double * b_solution){
    int i;

    printf("here00\n");
    //lets first get the first section done
    printf("first list: %d\n", first_rows_sync[0]);
    // solve_in_list(matrix->num_cols, matrix->index_cols, matrix->cols, matrix->val, b_solution, first_rows_sync, first_rows_sync_len);
    printf("here0\n");
    printf("first cols: %d\n", first_cols_async[0][0]);
    solve_cols_in_list(matrix->num_cols, matrix->val, b_solution, first_cols_async, first_cols_async_len);
    printf("here1\n");
    // we can then work on our second layer
    #pragma omp parallel num_threads(number_threads)
    {
        #pragma omp for
        for(int i = 0; i < number_threads; i++){
            solve_in_list(matrix->num_cols, matrix->index_cols, matrix->cols, matrix->val, b_solution, threads[i], threads_lens[i]);
        }
    }

    printf("here3\n");
    solve_cols_in_list(matrix->num_cols, matrix->val, b_solution, second_cols_async, second_cols_async_len);

    printf("here4\n");
    solve_in_list(matrix->num_cols, matrix->index_cols, matrix->cols, matrix->val, b_solution, second_rows_sync, first_rows_sync_len);
}