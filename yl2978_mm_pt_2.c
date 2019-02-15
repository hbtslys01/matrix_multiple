#include <time.h>   /* for clock_gettime */
#include <stdio.h>  /* for printf */
#include <unistd.h> //for sleep
#include <stdlib.h>
#include <pthread.h>

#define BILLION 1000000000L
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

int dim, CurRow = 0;
double *matrix1, *matrix2, *matrix_result;
pthread_mutex_t mutex_row;

void *doMyWork(void *thr_arg)
{
    int cRow;
    //int tid = *((int *)thr_arg);
    while (1)
    {
        pthread_mutex_lock(&mutex_row);
        {

            if (CurRow >= dim)
            {
                pthread_mutex_unlock(&mutex_row);
                return (0);
                //pthread_exit((void *)tid);
            }
            cRow = CurRow;
            CurRow++;
        }
        pthread_mutex_unlock(&mutex_row);

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
    scanf("%d %d", &dim, &NumThreads);
    matrix1 = (double *)malloc(dim * dim * sizeof(double));
    matrix2 = (double *)malloc(dim * dim * sizeof(double));
    matrix_result = (double *)malloc(dim * dim * sizeof(double));
    taskids = (int *)malloc(NumThreads * sizeof(int));
    srand48(1);
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
        {
            *(matrix1 + i * dim + j) = drand48();
            *(matrix2 + i * dim + j) = drand48();
        }

    /* allocate memory for threads */
    //NumThreads = MIN(NumThreads, dim * dim);
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
        //printf("\nCreate thread %d ", counter);
        *(taskids + counter) = counter;
        pthread_create(&threads[counter], &attr, (void *)doMyWork, (void *)(taskids + counter));
    }
    pthread_attr_destroy(&attr); /* free attribute and wait for others */
    /* join threads */
    for (counter = 0; counter < NumThreads; counter++)
    {
        pthread_join(threads[counter], &status);
        //printf("\nJoin thread %d, status = %ld", counter, (long)status);
    }
    pthread_mutex_destroy(&mutex_row);
    /*for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            printf("\nresult[%d][%d] is: % lf", i, j, *(matrix_result + i * dim + j));*/
    clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
    double diff = end.tv_sec - start.tv_sec + (double)(end.tv_nsec - start.tv_nsec) / BILLION;
    printf("\nelapsed time = %lf seconds\n", diff);
    pthread_exit(0);
}
