/**Author: Yunshan Liu
 * How this works: First comes first serves. Each thread calculates only one
 *      element of the result matrix, which means it multiply one row of matrix1
 *      according to current row counter with one column of matrix2 according to
 *      current column counter. 
 * How to compile: run command: gcc yl2978_mm_pt.c -o pt lpthread -O3
 * How to execute: run command: ./pt , then a message will be prompted which 
 *      asks you to type the dimension of two same-size sauqre matrices, and the 
 *      number of threads. 
 *      for example: 1600 4 means the matrices are 1600*1600, and 4 threads will 
 *      be produced for calculation.
 * Result: At last the time of matrices multiplication in block manner will be
 *      printed out.
*/
#include <time.h>   /* for clock_gettime */
#include <stdio.h>  /* for printf */
#include <unistd.h> //for sleep
#include <stdlib.h>
#include <pthread.h>

#define BILLION 1000000000L

int row_counter, col_counter; // row/col counter
int dim, CurCol = 0, CurRow = 0;
double *matrix1, *matrix2, *matrix_result;
pthread_mutex_t mutex_col; // lock for current column counter

void *doMyWork(void *thr_arg)
{
    int cRow, cCol;
    int tid = *((int *)thr_arg);
    while (1)
    {
        /*Lock current column counter. Note: no need to lock row counter because
        row counter is changed only when column counter is changed.*/
        pthread_mutex_lock(&mutex_col);
        {
            cCol = CurCol;
            cRow = CurRow;
            if (CurCol >= dim) /*Reach the end of the row*/
            {
                if (CurRow >= dim - 1) /*task finished*/
                {
                    pthread_mutex_unlock(&mutex_col);
                    return (0);
                }
                CurCol = 0;
                cCol = CurCol;
                CurRow++;
                cRow = CurRow;
            }
            CurCol++;
        }
        pthread_mutex_unlock(&mutex_col);
        double re = 0.0; //for accumulation
        /*Calculate current element of result matrix. Multiply current row of matrix1
        with current column of matrix2.*/
        for (int k = 0; k < dim; k++)
        {
            re += (*(matrix1 + cRow * dim + k)) * (*(matrix2 + k * dim + cCol));
        }
        *(matrix_result + cRow * dim + cCol) = re;
    }
}

int main()
{
    pthread_t *threads;
    pthread_attr_t attr;
    void *status;
    int counter, cRow, cCol, NumThreads;
    int *taskids;
    struct timespec start, end;
    printf("\nPlease enter the dimension of two square matrices to be multiplied, number of threads to be used: ");
    int a = scanf("%d %d", &dim, &NumThreads);
    /*Dynamically allocate space for two input matrices and one result matrix.*/
    matrix1 = (double *)malloc(dim * dim * sizeof(double));
    matrix2 = (double *)malloc(dim * dim * sizeof(double));
    matrix_result = (double *)malloc(dim * dim * sizeof(double));
    taskids = (int *)malloc(NumThreads * sizeof(int));
    srand48(1); //set the seed
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
        {
            /*Populate matrix1 & matrix2 with pudorandom number*/
            *(matrix1 + i * dim + j) = drand48();
            *(matrix2 + i * dim + j) = drand48();
        }

    /* allocate memory for threads */
    threads = (pthread_t *)malloc(sizeof(pthread_t) * NumThreads);
    /*attribute and mutex initialization*/
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&mutex_col, NULL);
    /* measure monotonic time */
    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    /* fork threads */
    for (counter = 0; counter < NumThreads; counter++)
    {
        /*Set thread id as the counter.*/
        *(taskids + counter) = counter;
        /*Creat #NumThreads threads, each of which will complish tasts defined in 
            function doMyWork.*/
        pthread_create(&threads[counter], &attr, (void *)doMyWork, (void *)(taskids + counter));
    }
    /*Release resource*/
    pthread_attr_destroy(&attr); /* free attribute and wait for others */
    /* join threads */
    for (counter = 0; counter < NumThreads; counter++)
    {
        /*Wait for every thread to terminate*/
        pthread_join(threads[counter], &status);
    }
    /*Release resource*/
    pthread_mutex_destroy(&mutex_col);
    clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
    /*Calculate time of mutrix maltiplication by block. Unit: seconds.*/
    double diff = end.tv_sec - start.tv_sec + (double)(end.tv_nsec - start.tv_nsec) / BILLION;
    printf("\nelapsed time = %lf seconds\n", diff);
    pthread_exit(0);
}
