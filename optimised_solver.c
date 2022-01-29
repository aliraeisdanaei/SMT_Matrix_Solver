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

int lsolve_parr(int matrix_dimension, int* index_rows, int* index_cols, int * cols, 
    long double* val, List * reach, List * not_reach, int first_section_length, long double *b_solution){

    // Index of both the row and the col that is being solved during the iteration
    int i_matrix;

    // Index of the col during the elimination of known values within next row
    int i_col;

    int i_list;

    long double known_values; 
    int start_known, end_known;

    //lets first get the first section done
    for (i_matrix = 0; i_matrix < first_section_length; i_matrix++){
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

    // lets then get all of the rows reachable by the first section done parrallely
    #pragma omp parallel private(i_matrix, i_col, known_values, start_known, end_known) num_threads(8)
    {

    #pragma omp for
    for (i_list = 0; i_list < reach->num_elem; i_list++){
        i_matrix = get_elem(reach, i_list);
        printf("Num threads: %d\n", omp_get_num_threads());
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

    // now lets get all of the n_reach
    for (i_list = 0; i_list < not_reach->num_elem; i_list++){
        i_matrix = get_elem(not_reach, i_list);
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

    return 1;
}

int lsolve_by_matrix_parr(Matrix * matrix, long double * b_vector){
    return lsolve_parr(matrix->num_cols, matrix->index_rows, matrix->index_cols, 
        matrix->cols, matrix->val, matrix->reach_first_section, matrix->not_reach_first_section, matrix->rows_in_first_section, b_vector);
}