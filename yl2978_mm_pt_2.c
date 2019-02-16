/**Author: Yunshan Liu
 * How this works: First comes first serves. Each thread calculates the whole row
 *      of the result matrix, which means it multiply one row of matrix1
 *      according to current row counter with entire matrix2. 
 * How to compile: run command: gcc yl2978_mm_pt_2.c -o pt2 lpthread -O3
 * How to execute: run command: ./pt2 , then a message will be prompted which 
 *      asks you to type the dimension of two same-size sauqre matrices, and the 
 *      number of threads. 
 *      for example: 1600 4 means the matrices are 1600*1600, and 4 threads will 
 *      be produced for calculation.
 * Result: At last the time of matrices multiplication in block manner will be
 *      printed out.
*/
#include <time.h>   /* for clock_gettime */
#include <stdio.h>  /* for printf */
#include <unistd.h> /* for sleep */
#include <stdlib.h>
#include <pthread.h>

#define BILLION 1000000000L

int dim, CurRow = 0;
double *matrix1, *matrix2, *matrix_result;
pthread_mutex_t mutex_row; // lock for current row counter

void *doMyWork(void *thr_arg)
{
    int cRow;
    while (1)
    {
        /*Lock current row counter.*/
        pthread_mutex_lock(&mutex_row);
        {

            if (CurRow >= dim) /*task finished*/
            {
                pthread_mutex_unlock(&mutex_row); //release the lock before return
                return (0);
            }
            cRow = CurRow;
            CurRow++;
        }
        pthread_mutex_unlock(&mutex_row);
        /*Calculate current row of result matrix. Multiply current row of matrix1
        with each column of matrix2.*/
        for (int j = 0; j < dim; j++)
        {
            double re = 0.0;
            for (int k = 0; k < dim; k++)
            {
                re += (*(matrix1 + cRow * dim + k)) * (*(matrix2 + k * dim + j));
            }
            *(matrix_result + cRow * dim + j) = re;
        }
    }
}

int main()
{
    pthread_t *threads;
    pthread_attr_t attr;
    void *status;
    int counter, NumThreads;
    int *taskids;
    struct timespec start, end;
    printf("\nPlease enter the dimension of two square matrices to be multiplied, number of threads to be used: ");
    int a = scanf("%d %d", &dim, &NumThreads);
    /*Dynamically allocate space for two input matrices and one result matrix.*/
    matrix1 = (double *)malloc(dim * dim * sizeof(double));
    matrix2 = (double *)malloc(dim * dim * sizeof(double));
    matrix_result = (double *)malloc(dim * dim * sizeof(double));
    taskids = (int *)malloc(NumThreads * sizeof(int)); // thread id
    srand48(1);                                        //set the seed
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
    pthread_mutex_init(&mutex_row, NULL);
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
    pthread_mutex_destroy(&mutex_row);
    /*Release space*/
    free(matrix1);
    free(matrix2);
    free(matrix_result);
    clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
    /*Calculate time of mutrix maltiplication by block. Unit: seconds.*/
    double diff = end.tv_sec - start.tv_sec + (double)(end.tv_nsec - start.tv_nsec) / BILLION;
    printf("\nelapsed time = %lf seconds\n", diff);
    pthread_exit(0);
}
