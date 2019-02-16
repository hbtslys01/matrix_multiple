/**Author: Yunshan Liu
 * How to compile: run command: gcc yl2978_mm_rbyc.c -o rbyc -O3
 * How to execute: run command: ./rbyc , then a message will be prompted which 
 *      asks you to type the dimension of two same-size sauqre matrices.
 *      for example: 1600 means the matrices are 1600*1600.
 * Result: At last the time of matrices multiplication row by column will be
 *      printed out.
*/
#include <time.h>   /* for clock_gettime */
#include <stdio.h>  /* for printf */
#include <unistd.h> /* for sleep */
#include <stdlib.h>

#define BILLION 1000000000L // 1 second = 1 BILLION nanosecond
int main()
{
    int dim;
    printf("\nPlease enter the dimension of two square matrices to be multiplied: ");
    int a = scanf("%d", &dim);
    /*Dynamically allocate space for two input matrices and one result matrix.*/
    double *matrix1 = (double *)malloc(dim * dim * sizeof(double));
    double *matrix2 = (double *)malloc(dim * dim * sizeof(double));
    double *matrix_result = (double *)malloc(dim * dim * sizeof(double));
    srand48(1); //set the seed
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
        {
            /*Populate matrix1 & matrix2 with pudorandom number*/
            *(matrix1 + i * dim + j) = drand48();
            *(matrix2 + i * dim + j) = drand48();
        }
    struct timespec start, end;
    /* measure monotonic time */
    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            double element = 0.;
            for (int k = 0; k < dim; k++)
            {
                /*Calculate matrix multiplication row by column*/
                element += *(matrix1 + i * dim + k) * (*(matrix2 + k * dim + j));
            }
            /*Set value of the ith row, jth column of result matrix.*/
            *(matrix_result + i * dim + j) = element;
            //printf("\nresult[%d][%d] is: % lf", i, j, *(matrix_result + i * dim + j));
        }
    }
    /*Release space*/
    free(matrix1);
    free(matrix2);
    free(matrix_result);
    clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
    /*Calculate time of mutrix maltiplication by block. Unit: seconds.*/
    double diff = end.tv_sec - start.tv_sec + (double)(end.tv_nsec - start.tv_nsec) / BILLION;
    printf("\nelapsed time = %lf seconds\n", diff);
}
