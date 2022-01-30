#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <omp.h>

#include "MatrixMarket/mmio.h"
#include "DynamicList/list.h"
#include "MatrixMarket/matrix.h"
#include "DynamicList/dynamic_list.h"
#include "MatrixMarket/read_mtx.h"

/**
 * Solves the Left side Matrix, upto n_rows
 * matrix_dimension: the matrix dimension
 * index_cols: the column pointer of L
 * index_rows: the row index of L
 * val: the values of L
 * In/Out:
 * b_solution : the right hand-side b at start and the solution x at the end.
 */
void solve_first_n_rows (int matrix_dimension, int* index_cols, int * cols, long double* val, long double *b_solution, int n_rows){
    // Index of both the row and the col that is being solved during the iteration
    int i_matrix;

    // Index of the col during the elimination of known values within next row
    int i_col;

    long double known_values; 
    int start_known, end_known;
    for (i_matrix = 0; i_matrix < n_rows; i_matrix++){
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
 * Solves the Left side Matrix, in the list
 * It is assumed that all the rows above the list have already been solved
 * matrix_dimension: the matrix dimension
 * index_cols: the column pointer of L
 * index_rows: the row index of L
 * val: the values of L
 * In/Out:
 * b_solution : the right hand-side b at start and the solution x at the end.
 */
void solve_in_list (int matrix_dimension, int* index_cols, int * cols, long double* val, long double *b_solution, List  * list){
    // Index of both the row and the col that is being solved during the iteration
    int i_matrix;

    // Index of the col during the elimination of known values within next row
    int i_col;

    int i_list;

    long double known_values; 
    int start_known, end_known;
    for (i_list = 0; i_list < list->num_elem; i_list++){
        i_matrix = get_elem(list, i_list);
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

    //lets first get the first section done
    solve_first_n_rows(matrix->num_cols, matrix->index_cols, matrix->cols, matrix->val, b_solution, matrix->rows_in_first_section);

    // lets then get all of the rows reachable by the first section done parrallely
    #pragma omp parallel num_threads(matrix->num_reach_threads_l1)
    {
        #pragma omp for
        for(int i = 0; i < matrix->num_reach_threads_l1; i++){
            solve_in_list(matrix->num_cols, matrix->index_cols, matrix->cols, matrix->val, b_solution, matrix->reach_threads_l1[i]);
        }
    }

    // we can then work on our second layer
    #pragma omp parallel num_threads(matrix->num_reach_threads_l2)
    {
        #pragma omp for
        for(int i = 0; i < matrix->num_reach_threads_l1; i++){
            solve_in_list(matrix->num_cols, matrix->index_cols, matrix->cols, matrix->val, b_solution, matrix->reach_threads_l2[i]);
        }
    }

    // now lets get all of the not_reach
    solve_in_list(matrix->num_cols, matrix->index_cols, matrix->cols, matrix->val, b_solution, matrix->not_reach);
}