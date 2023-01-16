#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mmio.h"
#include "../DynamicList/list.h"
#include "matrix.h"
#include "../DynamicList/dynamic_list.h"
#include "read_mtx.h"

#define FIRST_SECTION_NUM_ROWS 10000
#define NUM_REACH_THREADS_L1 2
#define NUM_REACH_THREADS_L2 2

/** 
 * Free the matrix and any memory allocated to fields of matrix
 */
void free_matrix(Matrix * matrix){
    int i;
    free(matrix->rows);
    free(matrix->cols);
    free(matrix->index_rows);
    free(matrix->index_cols);
    // if(matrix->reach_threads_l1 != NULL){
    //     for(i = 0; i < matrix->num_reach_threads_l1; i++){
    //         free_list(matrix->reach_threads_l1[i]);
    //     }
    //     free(matrix->reach_threads_l1);
    // }
    // if(matrix->reach_threads_l2 != NULL){
    //     for(i = 0; i < matrix->num_reach_threads_l2; i++){
    //         free_list(matrix->reach_threads_l2[i]);
    //     }
    //     free(matrix->reach_threads_l2);
    // }
    // free(matrix->not_reach);
    free(matrix->val);
    free(matrix);
}

/**
 * Writes the matrix read in mtx format
 */
void print_mtx(Matrix * matrix){
    int i;
    mm_write_banner(stdout, matrix->matcode);
    mm_write_mtx_crd_size(stdout, matrix->num_rows, matrix->num_cols, matrix->num_nonzero);
    for (i = 0; i < matrix->num_nonzero; i++){
        fprintf(stdout, "%d %d %20.19Le\n", matrix->rows[i] + 1, matrix->cols[i] + 1, matrix->val[i]);
    }
}

// /**
//  * Prints the statistics of the lists associated with the matrix
//  */
// void print_mtx_reach_stats(Matrix * matrix){
//     int i;
//     printf("Num first section: %d ---------------------------\n", matrix->rows_in_first_section);
//     printf("Layer 1---------------\n");
//     for(i = 0; i < matrix->num_reach_threads_l1; i++){
//         printf("Reach[%d]\n", i);
//         print_stats(matrix->reach_threads_l1[i]);
//     }
//     printf("Layer 2---------------\n");
//     for(i = 0; i < matrix->num_reach_threads_l2; i++){
//         printf("Reach[%d]\n", i);
//         print_stats(matrix->reach_threads_l2[i]);
//     }
//     printf("Not Reach-------------\n");
//     print_stats(matrix->not_reach);
// }

// /**
//  * Prints the full report of the lists associated with the matrix
//  */
// void print_mtx_reach(Matrix * matrix){
//     int i;
//     printf("Num first section: %d ---------------------------\n", matrix->rows_in_first_section);
//     printf("Layer 1---------------\n");
//     for(i = 0; i < matrix->num_reach_threads_l1; i++){
//         printf("Reach[%d]----\n", i);
//         print_list(matrix->reach_threads_l1[i]);
//     }
//     printf("Layer 2---------------\n");
//     for(i = 0; i < matrix->num_reach_threads_l2; i++){
//         printf("Reach[%d]----\n", i);
//         print_list(matrix->reach_threads_l2[i]);
//     }
//     printf("Not Reach-------------\n");
//     print_list(matrix->not_reach);
// }

/**
 * Write out the row and col indexes
 */
void print_indexes(Matrix * matrix){
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

/**
 * Return is a long double array of the val array, with all of the indexes not present in the val filled in with zero
 * matrix is assumed to be 1 dimensional
 * the matrix is freed
 */
long double * get_b_vector(Matrix * matrix){ 
    long double * b_vector = (long double *) malloc(matrix->num_rows * sizeof(long double));
    int i;
    for(i = 0; i < matrix->num_rows; b_vector[i++] = 0.0);
    for(i = 0; i < matrix->num_nonzero; i++) b_vector[matrix->rows[i]] = matrix->val[i];
    free_matrix(matrix);
    return b_vector;
}

/**
 * Return is i / n % n
 * when used with an increment i, i will repeat n times, then increment by the modulus of n
 * i.e. n = 3 Return(with i being an increment(increasing after each call)): 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, ...
 */
int get_repeated_num(int i, int n){
    return (i / n) % n;
}

/**
 * Sets the layer 1 reach of the matrix
 * the layer 1 reach is all of the elements that can be reach independently by multiple threads
 * after the first num_rows_solved
 */
// void set_reach_l1(Matrix * matrix, int num_rows_solved){
//     int i = 0;
//     matrix->num_reach_threads_l1 = NUM_REACH_THREADS_L1;
//     unsigned int size_reach = matrix->rows_in_first_section / matrix->num_reach_threads_l1;
//     matrix->not_reach = get_new_dynamic_list(size_reach);
//     matrix->reach_threads_l1 = (List **) malloc(matrix->num_reach_threads_l1 * sizeof(List *));
//     for(i = 0; i < matrix->num_reach_threads_l1; i++){
//         matrix->reach_threads_l1[i] = get_new_dynamic_list(size_reach);
//     }

//     int crnt_row = 0;

//     bool reachable_row = true;


//     // the indexes in the val of the row
//     int i_row_start, i_row_end, i_within_row;

//     int i_col;

//     for(crnt_row = num_rows_solved; crnt_row < matrix->num_rows; crnt_row++){
//         if(crnt_row > num_rows_solved){
//             //we are at a row that is not the first one
//             if(reachable_row){
//                 // the last row was reachable, so lets add it
//                 // printf("reach thread index %d\n", get_repeated_num(crnt_row, matrix->num_reach_threads_l1));
//                 insert_last_elem(matrix->reach_threads_l1[get_repeated_num(crnt_row, matrix->num_reach_threads_l1)], crnt_row - 1);
//             }else{
//                 // last row not reachable
//                 insert_last_elem(matrix->not_reach, crnt_row - 1);
//             }
//             reachable_row = true;
//         }
//         i_row_start = matrix->index_rows[crnt_row];
//         i_row_end = crnt_row == matrix->num_rows - 1 ? 
//             matrix->num_nonzero : matrix->index_rows[crnt_row + 1]; // the last row ends at the num_non_zero

//         // printf("start: %d end: %d\n", i_row_start, i_row_end);
//         for(i_within_row = i_row_start; i_within_row < i_row_end; i_within_row++){
//             i_col = matrix->cols[i_within_row];
//             // printf("row: %d col: %d i_within_row: %d\n", crnt_row, i_col, i_within_row);

//             if(i_col > num_rows_solved){
//                 // the col is greater than the cols solved by the rows in the first section
//                 // printf("the col is greater than the cols solved by the rows in the first section\n");
//                 if(i_col != crnt_row){
//                     //cannot be solved by itself
//                     // printf("cannot be solved by itself\n");
//                     if(! elem_in_list(matrix->reach_threads_l1[get_repeated_num(crnt_row, matrix->num_reach_threads_l1)], i_col)){
//                         // elem cannot be reached by what is already in the reach thread
//                         reachable_row = false;
//                     }
//                 }
//             }
//         }
//     } 

//     //last row still left
//     if(reachable_row){
//         // the last row was reachable, so lets add it
//         insert_last_elem(matrix->reach_threads_l1[get_repeated_num(crnt_row, matrix->num_reach_threads_l1)], crnt_row - 1);
//     }else{
//         // last row not reachable
//         insert_last_elem(matrix->not_reach, crnt_row - 1);
//     }
// }

// /**
//  * Sets the layer 2 reach of the matrix
//  * the layer 2 reach is all of the elements that can be reach independently by multiple threads
//  * after the first num_rows_solved, and the first layer
//  */
// void set_reach_l2(Matrix * matrix, int num_rows_solved){
//     int i = 0;
//     matrix->num_reach_threads_l2 = NUM_REACH_THREADS_L2;
//     unsigned int size_reach = matrix->rows_in_first_section / matrix->num_reach_threads_l2;
//     matrix->reach_threads_l2 = (List **) malloc(matrix->num_reach_threads_l2 * sizeof(List *));
//     for(i = 0; i < matrix->num_reach_threads_l2; i++){
//         matrix->reach_threads_l2[i] = get_new_dynamic_list(size_reach);
//     }

//     List * new_not_reach = get_new_dynamic_list(size_reach);

//     int crnt_row = 0;

//     bool reachable_row = true;


//     // the indexes in the val of the row
//     int i_row_start, i_row_end, i_within_row;

//     int i_col;

//     int i_list;

//     // for(crnt_row = num_rows_solved; crnt_row < matrix->num_rows; crnt_row++){
//     for(i_list = 0; i_list < matrix->not_reach->num_elem; i_list++){
//         crnt_row = get_elem(matrix->not_reach, i_list);
//         if(crnt_row != get_elem(matrix->not_reach, 0)){
//             //we are at a row that is not the first one
//             if(reachable_row){
//                 // the last row was reachable, so lets add it
//                 insert_last_elem(matrix->reach_threads_l2[get_repeated_num(i_list, matrix->num_reach_threads_l2)], crnt_row - 1);
//             }else{
//                 // last row not reachable
//                 insert_last_elem(new_not_reach, crnt_row - 1);
//             }
//             reachable_row = true;
//         }
//         i_row_start = matrix->index_rows[crnt_row];
//         i_row_end = crnt_row == matrix->num_rows - 1 ? 
//             matrix->num_nonzero : matrix->index_rows[crnt_row + 1]; // the last row ends at the num_non_zero

//         for(i_within_row = i_row_start; i_within_row < i_row_end; i_within_row++){
//             i_col = matrix->cols[i_within_row];

//             if(i_col > num_rows_solved){
//                 // the col is greater than the cols solved by the rows in the first section
//                 if(i_col != crnt_row){
//                     //cannot be solved by itself
//                     if(! elem_in_list_of_lists(matrix->reach_threads_l1, matrix->num_reach_threads_l1, i_col)){
//                         // cannot be reached by the previous layer
//                         if(! elem_in_list(matrix->reach_threads_l2[get_repeated_num(i_list, matrix->num_reach_threads_l2)], i_col)){
//                             // elem cannot be reached by what is already in the reach thread
//                             reachable_row = false;
//                         }
//                     } 
//                 }
//             }
//         }
//     } 

//     //last row still left over
//     if(reachable_row){
//         // the last row was reachable, so lets add it
//         insert_last_elem(matrix->reach_threads_l2[get_repeated_num(i_list, matrix->num_reach_threads_l2)], crnt_row - 1);
//     }else{
//         // last row not reachable
//         insert_last_elem(new_not_reach, crnt_row - 1);
//     }

//     List * old_not_reach = matrix->not_reach;
//     matrix->not_reach = new_not_reach;
//     free_list(old_not_reach);
// }

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

    // matrix->rows_in_first_section = FIRST_SECTION_NUM_ROWS < matrix->num_rows ? FIRST_SECTION_NUM_ROWS : 1;

    matrix->rows = (int *) malloc(matrix->num_nonzero * sizeof(int));
    matrix->cols = (int *) malloc(matrix->num_nonzero * sizeof(int));
    matrix->val = (long double *) malloc(matrix->num_nonzero * sizeof(long double));

    matrix->index_cols = (int *) malloc(matrix->num_cols * sizeof(int));
    matrix->index_rows = (int *) malloc(matrix->num_rows * sizeof(int));

    for(i = 0; i < matrix->num_rows; matrix->index_rows[i++] = -1);
    for(i = 0; i < matrix->num_cols; matrix->index_cols[i++] = -1);

    for (i = 0; i < matrix->num_nonzero; i++){
        fscanf(f, "%d %d %Le\n", &i_row, &i_col, &tmp_val);

        matrix->rows[i] = i_row - 1;  
        matrix->cols[i] = i_col - 1;
        matrix->val[i] = tmp_val;

        if(matrix->index_rows[matrix->rows[i]] == -1){
            //reached a new row
            matrix->index_rows[matrix->rows[i]] = i;

        }

        if(matrix->index_cols[matrix->cols[i]] == -1){
            //reached a new col
            matrix->index_cols[matrix->cols[i]] = i;
        }
    }

    // set_reach_l1(matrix, matrix->rows_in_first_section);
    // set_reach_l2(matrix, matrix->rows_in_first_section);
    if (f !=stdin) fclose(f);
    return matrix;
}

/**
 * Tests the reading function
 * argc - the number of arguments
 * argv - the arguments
 * i_argv - the index the read file is in
 */
void test_read_mtx(int argc, char *argv[], int i_agrv){
    Matrix * matrix = read_mtx_file(argc, argv, i_agrv);
    print_mtx(matrix);
    print_indexes(matrix);
    // print_mtx_reach(matrix);
}