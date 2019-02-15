#include <time.h>   /* for clock_gettime */
#include <stdio.h>  /* for printf */
#include <unistd.h> //for sleep
#include <stdlib.h>

#define BILLION 1000000000L
void mat_mul(double **mat1, double **mat2, double **result, int pos1, int com,
             int pos2, int size)
{
    for (int i = 0; i < size; i++)
    {
        int r_pos = pos1 + i;
        for (int j = 0; j < size; j++)
        {
            double element = 0.;
            int c_pos = pos2 + j;
            for (int k = 0; k < size; k++)
                element += mat1[r_pos][com + k] * mat2[com + k][c_pos];
            result[r_pos][c_pos] += element;
        }
    }
}
int main()
{
    int dim, size, num_block;
    printf("\nPlease enter the dimension of two square matrices to be multiplied, dimension of tiles(the former should be divisible by the latter): ");
    scanf("%d %d", &dim, &size);
    num_block = dim / size;
    double *matrix1[dim], *matrix2[dim], *matrix_result[dim];
    srand48(1);
    for (int i = 0; i < dim; i++)
    {
        matrix1[i] = (double *)malloc(dim * sizeof(double));
        matrix2[i] = (double *)malloc(dim * sizeof(double));
        matrix_result[i] = (double *)malloc(dim * sizeof(double));
    }
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
        {
            matrix1[i][j] = drand48();
            matrix2[i][j] = drand48();
            //printf("\nmatrix1[%d][%d] is: % lf; matrix2[%d][%d] is: % lf",
            //      i, j, matrix1[i][j], i, j, matrix2[i][j]);
        }
    struct timespec start, end;
    /* measure monotonic time */
    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    for (int i = 0; i < num_block; i++)
    {
        for (int j = 0; j < num_block; j++)
        {
            //printf("\nprocessing %d th, %d th block", i, j);
            for (int k = 0; k < num_block; k++)
            {
                mat_mul(matrix1, matrix2, matrix_result, i * size, k * size, j * size, size);
            }
        }
    }
    /*for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            printf("\nresult[%d][%d] is: % lf",
                   i, j, matrix_result[i][j]);*/
    clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
    double diff = end.tv_sec - start.tv_sec + (double)(end.tv_nsec - start.tv_nsec) / BILLION;
    printf("\nelapsed time = %lf seconds\n", diff);
}
