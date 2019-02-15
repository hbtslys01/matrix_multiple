#include <time.h>   /* for clock_gettime */
#include <stdio.h>  /* for printf */
#include <unistd.h> //for sleep
#include <stdlib.h>

#define BILLION 1000000000L
int dim;
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
                element += *(mat1 + r_index + com + k) * (*(mat2 + c_index + k * dim + pos2 + j));
                //printf("\n %lf * %lf ", *(mat1 + r_index + com + k), *(mat2 + c_index + k * dim + pos2 + j));
            }
            //element += mat1[r_pos][com + k] * mat2[com + k][c_pos];
            //result[r_pos][c_pos] += element;
            *(result + r_index + pos2 + j) += element;
            //printf("\n result[%d][%d] = %lf", r_pos, c_pos, *(result + r_index + c_pos + pos2 + j));
        }
    }
}
int main()
{
    int size, num_block;
    printf("\nPlease enter the dimension of two square matrices to be multiplied, dimension of tiles(the former should be divisible by the latter): ");
    scanf("%d %d", &dim, &size);
    num_block = dim / size;
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
    for (int i = 0; i < num_block; i++)
    {
        for (int j = 0; j < num_block; j++)
        {
            for (int k = 0; k < num_block; k++)
            {
                mat_mul(matrix1, matrix2, matrix_result, i * size, k * size, j * size, size);
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
    double diff = end.tv_sec - start.tv_sec + (double)(end.tv_nsec - start.tv_nsec) / BILLION;
    printf("\nelapsed time = %lf seconds\n", diff);
}
