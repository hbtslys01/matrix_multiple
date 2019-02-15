#include <time.h>   /* for clock_gettime */
#include <stdio.h>  /* for printf */
#include <unistd.h> //for sleep
#include <stdlib.h>

#define BILLION 1000000000L
int main()
{
    int dim;
    printf("\nPlease enter the dimension of two square matrices to be multiplied(It should be an integer): ");
    scanf("%d", &dim);
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
            //printf("\nmatrix1[%d][%d] is: % lf", i, j, matrix1[i][j]);
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
                element += matrix1[i][k] * matrix2[k][j];
            matrix_result[i][j] = element;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
    long diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("\nelapsed time = %ld nanoseconds\n", diff);
}
