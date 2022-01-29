#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <time.h>
#include <math.h>

#include "MatrixMarket/mmio.h"
#include "DynamicList/list.h"
#include "MatrixMarket/matrix.h"
#include "MatrixMarket/read_mtx.h"

#define ACCEPTABLE_DIFF 0.005

bool matrix_sol_verifier(int num_rows, int num_nonzero, int * index_rows, int * cols, long double * val, long double * sol, long double * b_vector){
    if (!num_rows || !index_rows || !val || !sol) return 0;

    // the index of the row during the iteration
    int i_row;

    // the index of elem in row
    int i_elem;

    // the last index of the current row
    int last_i_row;

    long double sum_row;
    int check_row = -1;

    for(i_row = 0; i_row < num_rows; i_row++){
        last_i_row = i_row == num_rows - 1 ? num_nonzero : index_rows[i_row + 1];

        if(i_row == check_row){
            printf("i_row --------: %d\n", i_row);
            printf("last_i_row: %d\n", last_i_row);
            printf("first i_elem: %d\n", index_rows[i_row]);
        }

        sum_row = 0;
        for(i_elem = index_rows[i_row]; i_elem < last_i_row; i_elem++){

            sum_row += val[i_elem] * sol[cols[i_elem]];
            // if(i_row == check_row && sol[cols[i_elem]] != 0) printf("i_elem: %d -- %Le = %Le * sol %Le -- sum: %Le\n", 
                // i_elem, val[i_elem] * sol[cols[i_elem]], val[i_elem], sol[cols[i_elem]], sum_row);
        }
        if(i_row == check_row) printf("## sum_row: %Le\n", sum_row);
        if((sum_row - b_vector[i_row]) >= ACCEPTABLE_DIFF){
            printf("Verification failed.\n");
            printf("Sum row[%d] = %Le\n", i_row, sum_row);
            printf("B_vector: %Le\n", b_vector[i_row]);
            printf("Differnce: %Le\n", (sum_row - b_vector[i_row]));
            return false;
        }
    }
    printf("Verified\n");
    return true;
}

void matrix_sol_write_inacc(int num_rows, int num_nonzero, int * index_rows, int * cols, long double * val, long double * sol, long double * b_vector){
    if (!num_rows || !index_rows || !val || !sol) return;

    // the index of the row during the iteration
    int i_row;

    // the index of elem in row
    int i_elem;

    // the last index of the current row
    int last_i_row;

    long double sum_row;

   FILE *fp;
   fp = fopen("inaccuracies.csv", "w");
   if(fp == NULL) exit(1);

    fprintf(fp, "Row, Difference\n");
    for(i_row = 0; i_row < num_rows; i_row++){
        last_i_row = i_row == num_rows - 1 ? num_nonzero : index_rows[i_row + 1];

        sum_row = 0;
        for(i_elem = index_rows[i_row]; i_elem < last_i_row; i_elem++){

            sum_row += val[i_elem] * sol[cols[i_elem]];
        }
        if((sum_row - b_vector[i_row]) >= ACCEPTABLE_DIFF){
            fprintf(fp, "%d, %Le\n", i_row, (sum_row - b_vector[i_row]));
        }
    }
   fclose(fp);
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
