#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <time.h>

#include "MatrixMarket/mmio.h"
#include "DynamicList/list.h"
#include "MatrixMarket/matrix.h"
#include "DynamicList/dynamic_list.h"
#include "MatrixMarket/read_mtx.h"
#include "naive_solver.h"
#include "optimised_solver.h"
#include "verifier.h"

int main(int argc, char *argv[]){
    if(strcmp(argv[1], "test_mtx") == 0){
<<<<<<< HEAD
        test_read_mtx(argc, argv);
=======
        test_read_mtx(argc, argv, 2);
>>>>>>> f3694d8 (Finished with the project to a satisfactory level. Added a report/documentation. Added full layering feature for the optimised version. Including most of the input files in this commit as well.)
        return 0;
    }

    clock_t start_time, end_time;
    Matrix * matrix = read_mtx_file(argc, argv, 2);
<<<<<<< HEAD
    printf("Reach of %d rows\n", matrix->rows_in_first_section);
    print_stats(matrix->reach_first_section);
    printf("Not Reach of %d rows\n", matrix->rows_in_first_section);
    print_stats(matrix->not_reach_first_section);

    // write_out_mtx(matrix);
    // write_out_indexes(matrix);
=======
>>>>>>> f3694d8 (Finished with the project to a satisfactory level. Added a report/documentation. Added full layering feature for the optimised version. Including most of the input files in this commit as well.)

    long double * b_vector = get_b_vector(read_mtx_file(argc, argv, 3));
    long double * b_vector_og = (long double *) malloc(matrix->num_rows * sizeof(long double));
    memcpy(b_vector_og, b_vector, matrix->num_rows * sizeof(long double));

    if(strcmp(argv[1], "naive") == 0){
        start_time = clock();
        lsolve_by_matrix(matrix, b_vector);
        end_time = clock();
<<<<<<< HEAD
        printf("Computation Time Naive: %d\n", (int)(end_time - start_time));
    }else{
        start_time = clock();
        lsolve_by_matrix_parr(matrix, b_vector);
        end_time = clock();
        printf("Computation Time Parr: %d\n", (int)(end_time - start_time));
=======
        printf("Computation Time: %d ms\n", (int)(end_time - start_time));
    }else{
        print_mtx_reach_stats(matrix);
        start_time = clock();
        lsolve_by_matrix_parr(matrix, b_vector);
        end_time = clock();
        printf("Computation Time: %d ms\n", (int)(end_time - start_time));
>>>>>>> f3694d8 (Finished with the project to a satisfactory level. Added a report/documentation. Added full layering feature for the optimised version. Including most of the input files in this commit as well.)
    }

    write_sol(b_vector, matrix->num_rows);

    // matrix_sol_verifier(matrix->num_rows, matrix->num_nonzero, matrix->index_rows, 
    //     matrix->cols,  matrix->val, b_vector, b_vector_og);
    
    matrix_sol_write_inacc(matrix->num_rows, matrix->num_nonzero, matrix->index_rows, 
        matrix->cols,  matrix->val, b_vector, b_vector_og);
<<<<<<< HEAD
=======

    printf("Solution produced in sol.csv\n");
    printf("Inaccuracies recorded in inaccuracies.csv\n");
>>>>>>> f3694d8 (Finished with the project to a satisfactory level. Added a report/documentation. Added full layering feature for the optimised version. Including most of the input files in this commit as well.)
}