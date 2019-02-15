#include <time.h>   /* for clock_gettime */
#include <stdio.h>  /* for printf */
#include <unistd.h> //for sleep
#include <stdlib.h>

#define BILLION 1000000000L
int main()
{
    int dim;
    printf("\nPlease enter the dimension of two square matrices to be multiplied: ");
    scanf("%d", &dim);
    double *matrix1 = (double *)malloc(dim * dim * sizeof(double));
    double *matrix2 = (double *)malloc(dim * dim * sizeof(double));
    double *matrix_result = (double *)malloc(dim * dim * sizeof(double));
    srand48(1);
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
        {
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
                element += *(matrix1 + i * dim + k) * (*(matrix2 + k * dim + j));
            *(matrix_result + i * dim + j) = element;
            //printf("\nresult[%d][%d] is: % lf", i, j, *(matrix_result + i * dim + j));
        }
    }
    free(matrix1);
    free(matrix2);
    free(matrix_result);
    clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
    double diff = end.tv_sec - start.tv_sec + (double)(end.tv_nsec - start.tv_nsec) / BILLION;
    printf("\nelapsed time = %lf seconds\n", diff);
}
