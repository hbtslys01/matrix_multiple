/**Author: Yunshan Liu
 * How to compile: run command: gcc yl2978_mm_tile.c -o tile -O3
 * How to execute: run command: ./tile , then a message will be prompted which 
 *      asks you to type the dimension of two same-size sauqre matrices, and the 
 *      block size. Note that the former should be divisible by the latter.
 *      for example: 1600 40 means the matrices are 1600*1600, which are diviced 
 *      into 40*40 small matrices.
 * Result: At last the time of matrices multiplication in block manner will be
 *      printed out.
*/
#include <time.h>   /* for clock_gettime */
#include <stdio.h>  /* for printf */
#include <unistd.h> /* for sleep */
#include <stdlib.h>

#define BILLION 1000000000L // 1 second = 1 BILLION nanosecond
int dim;                    //dimention of square matrix
void mat_mul(double *mat1, double *mat2, double *result, int pos1, int com,
             int pos2, int size)
{
    int c_index = com * dim;
    for (int i = 0; i < size; i++)
    {
        int r_pos = pos1 + i;
        int r_index = (pos1 + i) * dim;
        for (int j = 0; j < size; j++)
        {
            double element = 0.;
            int c_pos = pos2 + j;
            for (int k = 0; k < size; k++)
            {
                /*offset of matrix1 element to be fetched*/
                int mat1_offset = r_index + com + k;
                /*offset of matrix2 element to be fetched*/
                int mat2_offset = c_index + k * dim + pos2 + j;
                element += *(mat1 + mat1_offset) * (*(mat2 + mat2_offset));
            }
            /*Set value of the ith row, jth column of result matrix.*/
            *(result + r_index + pos2 + j) += element;
        }
    }
}
int main()
{
    /*Block size and #block*/
    int size, num_block;
    printf("\nPlease enter the dimension of two square matrices to be multiplied, dimension of tiles(the former should be divisible by the latter): ");
    int a = scanf("%d %d", &dim, &size);
    num_block = dim / size; // amount of blocks
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
    for (int i = 0; i < num_block; i++)
    {
        for (int j = 0; j < num_block; j++)
        {
            for (int k = 0; k < num_block; k++)
            {
                /*Calculate matrix multiplication by block.*/
                mat_mul(matrix1, matrix2, matrix_result, i * size, k * size, j * size, size);
            }
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
