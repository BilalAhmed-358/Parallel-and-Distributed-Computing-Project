// gcc -g -Wall -fopenmp -o matrix matrix.c

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{
    double start_time = omp_get_wtime();
    int thread_count = stoi(argv[1], NULL, 10);
    int i = 0, j = 0, p = 0, q = 0;
    int sum = 0, count = 0;
    int size_of_matrix = 30;
    int filter_size = 5;
    int complete_filter = filter_size * filter_size;
    int middle_filter = filter_size / 2;
    int core_matrix = size_of_matrix - middle_filter;

    int **matrix_o = (int **)malloc(size_of_matrix * sizeof(int *));
    int **matrix_d = (int **)malloc(size_of_matrix * sizeof(int *));
    int **filter = (int **)malloc(filter_size * sizeof(int *));

    ofstream matrix;
    matrix.open("Original_matrix.txt");
    for (i = 0; i < size_of_matrix; i++)
    {
        matrix_o[i] = (int *)malloc(size_of_matrix * sizeof(int));
        matrix_d[i] = (int *)malloc(size_of_matrix * sizeof(int));
        for (j = 0; j < size_of_matrix; j++)
        {
            matrix_o[i][j] = rand() % 256;
            matrix << matrix_o[i][j] << " ";
        }
        matrix << endl;
    }
    matrix.close();
    // printing out the matrix
    // for (i = 0; i < size_of_matrix; i++)
    // {
    //     printf("[ ");
    //     for (j = 0; j < size_of_matrix; j++)
    //     {
    //         printf(" %05d", matrix_o[i][j]);
    //     }
    //     printf("]\n");
    // }

    ofstream filter_matrix;
    filter_matrix.open("filter_matrix.txt");
    for (i = 0; i < filter_size; i++)
    {
        filter[i] = (int *)malloc(filter_size * sizeof(int));
        for (j = 0; j < filter_size; j++)
        {
            if (j % 2 == 0 && i % 2 == 0)
                filter[i][j] = 1;
            else
                filter[i][j] = 0;

            filter_matrix << filter[i][j] << " ";
        }
        filter_matrix << endl;
    }
    filter_matrix.close();

    // printout the filter matrix
    // for (i = 0; i < filter_size; i++)
    // {
    //     cout << "[ ";
    //     for (j = 0; j < filter_size; j++)
    //     {
    //         cout << filter[i][j] << " ";
    //     }
    //     cout << "]" << endl;
    // }
    ofstream ij;
    ij.open("ij.txt");
#pragma omp parallel num_threads(thread_count) private(sum, i, j, p, q, count) shared(matrix_d, matrix_o)
    {
#pragma omp for schedule(dynamic)
        for (i = middle_filter; i < core_matrix; i++)
        {
            for (j = middle_filter; j < core_matrix; j++)
            {
                sum = 0;
                for (p = 0; p < filter_size; p++)
                {
                    for (q = 0; q < filter_size; q++)
                    {
                        sum += filter[p][q] * matrix_o[i - middle_filter + p][j - middle_filter + q];
                        cout << sum << endl;
                    }
                }
                matrix_d[i][j] = sum / complete_filter;
            }
        }

        // printf("Elapsed time with %d threads is %f\n", thread_count, omp_get_wtime() - start_time);

        //#	pragma omp parallel for num_threads(thread_count) private(sum,i,j,p,q,count) shared(matrix_d,matrix_o) schedule(dynamic)

#pragma omp for schedule(dynamic)
        for (i = 0; i < middle_filter; i++)
        {
            for (j = 0; j < middle_filter; j++)
            {
                // printf("\n%d: Testing for i=%d,j=%d", omp_get_thread_num(), i, j);
                sum = 0, count = 0;
                for (p = 0; p < filter_size; p++)
                {
                    for (q = 0; q < filter_size; q++)
                    {
                        // printf("\n\t%d: Testing for i=%d,j=%d, p=%d, q=%d", omp_get_thread_num(), i, j, p, q);
                        if (i - middle_filter + p >= 0 && i - middle_filter + p < size_of_matrix && j - middle_filter + q >= 0 && j - middle_filter + q < size_of_matrix)
                        {
                            // printf("\n\t%d: Calculating for i=%d, j=%d, p=%d, q=%d", omp_get_thread_num(), i, j, p, q);
                            count++;
                            sum += filter[p][q] * matrix_o[i - middle_filter + p][j - middle_filter + q];
                        }
                    }
                }
                matrix_d[i][j] = sum / count;
            }
        }

#pragma omp for schedule(dynamic)
        for (i = 0; i < middle_filter; i++)
        {
            for (j = core_matrix; j < size_of_matrix; j++)
            {
                // printf("\n%d: Testing for i=%d,j=%d",omp_get_thread_num(),i,j);//PAUSA
                sum = 0, count = 0;
                for (p = 0; p < filter_size; p++)
                    for (q = 0; q < filter_size; q++)
                    {
                        // printf("\n\t%d: Testing for i=%d,j=%d, p=%d, q=%d",omp_get_thread_num(),i,j,p,q);PAUSA
                        if (i - middle_filter + p >= 0 && i - middle_filter + p < size_of_matrix && j - middle_filter + q >= 0 && j - middle_filter + q < size_of_matrix)
                        {
                            // printf("\n\t%d: Calculating for i=%d, j=%d, p=%d, q=%d", omp_get_thread_num(), i, j, p, q);
                            count++;
                            sum += filter[p][q] * matrix_o[i - middle_filter + p][j - middle_filter + q];
                        }
                    }
                matrix_d[i][j] = sum / count;
            }
        }

#pragma omp for schedule(dynamic)
        for (i = core_matrix; i < size_of_matrix; i++)
        {
            for (j = 0; j < middle_filter; j++)
            {

                // printf("\n%d: Testing for i=%d,j=%d",omp_get_thread_num(),i,j);//PAUSA
                sum = 0, count = 0;
                for (p = 0; p < filter_size; p++)
                    for (q = 0; q < filter_size; q++)
                    {
                        // printf("\n\t%d: Testing for i=%d,j=%d, p=%d, q=%d",omp_get_thread_num(),i,j,p,q);PAUSA
                        if (i - middle_filter + p >= 0 && i - middle_filter + p < size_of_matrix && j - middle_filter + q >= 0 && j - middle_filter + q < size_of_matrix)
                        {
                            // printf("\n\t%d: Calculating for i=%d, j=%d, p=%d, q=%d",omp_get_thread_num(),i,j,p,q);PAUSA
                            count++;
                            sum += filter[p][q] * matrix_o[i - middle_filter + p][j - middle_filter + q];
                        }
                    }
                matrix_d[i][j] = sum / count;
            }
        }

#pragma omp for schedule(dynamic)
        for (i = core_matrix; i < size_of_matrix; i++)
        {
            for (j = core_matrix; j < size_of_matrix; j++)
            {
                // printf("\n%d: Testing for i=%d,j=%d",omp_get_thread_num(),i,j);//PAUSA
                sum = 0, count = 0;
                for (p = 0; p < filter_size; p++)
                    for (q = 0; q < filter_size; q++)
                    {
                        // printf("\n\t%d: Testing for i=%d,j=%d, p=%d, q=%d",omp_get_thread_num(),i,j,p,q);PAUSA
                        if (i - middle_filter + p >= 0 && i - middle_filter + p < size_of_matrix && j - middle_filter + q >= 0 && j - middle_filter + q < size_of_matrix)
                        {
                            // printf("\n\t%d: Calculating for i=%d, j=%d, p=%d, q=%d",omp_get_thread_num(),i,j,p,q);PAUSA
                            count++;
                            sum += filter[p][q] * matrix_o[i - middle_filter + p][j - middle_filter + q];
                        }
                    }
                matrix_d[i][j] = sum / count;
            }
        }
    }

    printf("Elapsed time with %d threads is %f\n", thread_count, omp_get_wtime() - start_time);

    // printing matrixd
    ofstream mat_d;
    mat_d.open("Matrix_d.txt");
    for (i = 0; i < size_of_matrix; i++)
    {
        // printf("[ ");
        for (j = 0; j < size_of_matrix; j++)
        {
            mat_d << matrix_d[i][j] << " ";
        }
        mat_d << endl;
        // printf("]\n");
    }
    return 0;
}
