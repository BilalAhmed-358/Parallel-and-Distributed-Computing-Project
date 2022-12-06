#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{
    string one = "thread0and1.txt";
    string two = "thread2and3.txt";
    string three = "thread4and5.txt";
    string four = "thread6and7.txt";
    ofstream one_;
    ofstream two_;
    ofstream three_;
    ofstream four_;
    one_.open(one);
    two_.open(two);
    three_.open(three);
    four_.open(four);
    double start_time = omp_get_wtime();
    int thread_count = stoi(argv[1], NULL, 10);
    int i = 0, j = 0, p = 0, q = 0;
    int sum = 0, count = 0;
    int size_of_matrix = 2000;
    int filter_size = 5;
    int convolution_matrix_size = size_of_matrix - filter_size + 1;
    int complete_filter = filter_size * filter_size;
    int middle_filter = convolution_matrix_size / 2;
    int core_matrix = size_of_matrix - middle_filter;

    int **matrix_o = (int **)malloc(size_of_matrix * sizeof(int *));
    int **convolution_matrix = (int **)malloc(convolution_matrix_size * sizeof(int *));
    int **filter = (int **)malloc(filter_size * sizeof(int *));

    ofstream matrix;
    matrix.open("Original_matrix.txt");
    for (i = 0; i < size_of_matrix; i++)
    {
        matrix_o[i] = (int *)malloc(size_of_matrix * sizeof(int));
        convolution_matrix[i] = (int *)malloc(convolution_matrix_size * sizeof(int));
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
#pragma omp parallel num_threads(thread_count) private(sum, i, j, p, q, count) shared(convolution_matrix, matrix_o)
    {
        if (omp_get_thread_num() == 0 || omp_get_thread_num() == 1)
        {
#pragma omp parallel for schedule(dynamic)
            for (i = 0; i < middle_filter; i++)
            {

                for (j = 0; j < middle_filter; j++)
                {
                    sum = 0, count = 0;
                    for (p = 0; p < filter_size; p++)
                    {
                        for (q = 0; q < filter_size; q++)
                        {
                            count++;
                            sum += filter[p][q] * matrix_o[i][j];
                        }
                    }
                    convolution_matrix[i][j] = sum / count;
                }
            }
            if (omp_get_thread_num() == 0)
            {
                for (i = 0; i < middle_filter; i++)
                {
                    for (j = 0; j < middle_filter; j++)
                    {
                        one_ << convolution_matrix[i][j] << " ";
                        // cout << convolution_matrix[i][j] << " ";
                    }
                    one_ << endl;
                    // cout << endl;
                }
            }
        }

        if (omp_get_thread_num() == 2 || omp_get_thread_num() == 3)
        {
#pragma omp parallel for schedule(dynamic)
            for (i = 0; i < middle_filter; i++)
            {
                for (j = middle_filter; j < convolution_matrix_size; j++)
                {
                    sum = 0, count = 0;
                    for (p = 0; p < filter_size; p++)
                    {
                        for (q = 0; q < filter_size; q++)
                        {
                            count++;
                            sum += filter[p][q] * matrix_o[i][j];
                        }
                    }
                    convolution_matrix[i][j] = sum / count;
                }
            }
            if (omp_get_thread_num() == 2)
            {
                for (i = 0; i < middle_filter; i++)
                {
                    for (j = middle_filter; j < convolution_matrix_size; j++)
                    {
                        two_ << convolution_matrix[i][j] << " ";
                        // cout << convolution_matrix[i][j] << " ";
                    }
                    two_ << endl;
                    // cout << endl;
                }
            }
        }

        if (omp_get_thread_num() == 4 || omp_get_thread_num() == 5)
        {
#pragma omp parallel for schedule(dynamic)
            for (i = middle_filter; i < convolution_matrix_size; i++)
            {
                for (j = 0; j < middle_filter; j++)
                {
                    sum = 0, count = 0;
                    for (p = 0; p < filter_size; p++)
                    {
                        for (q = 0; q < filter_size; q++)
                        {
                            count++;
                            sum += filter[p][q] * matrix_o[i][j];
                        }
                    }

                    convolution_matrix[i][j] = sum / count;
                }
            }
            if (omp_get_thread_num() == 4)
            {
                for (i = middle_filter; i < convolution_matrix_size; i++)
                {
                    for (j = 0; j < middle_filter; j++)
                    {
                        // cout << convolution_matrix[i][j] << " ";
                        three_ << convolution_matrix[i][j] << " ";
                    }
                    three_ << endl;
                    // cout << endl;
                }
            }
        }

        if (omp_get_thread_num() == 6 || omp_get_thread_num() == 7)
        {
#pragma omp parallel for schedule(dynamic)
            for (i = middle_filter; i < convolution_matrix_size; i++)
            {
                for (j = middle_filter; j < convolution_matrix_size; j++)
                {
                    sum = 0, count = 0;
                    for (p = 0; p < filter_size; p++)
                        for (q = 0; q < filter_size; q++)
                        {
                            count++;
                            sum += filter[p][q] * matrix_o[i][j];
                        }
                    convolution_matrix[i][j] = sum / count;
                }
            }
            if (omp_get_thread_num() == 6)
            {
                for (i = middle_filter; i < convolution_matrix_size; i++)
                {
                    for (j = middle_filter; j < convolution_matrix_size; j++)
                    {
                        // cout << convolution_matrix[i][j] << " ";
                        four_ << convolution_matrix[i][j] << " ";
                    }
                    // cout << endl;
                    four_ << endl;
                }
            }
        }
    }
    one_.close();
    two_.close();
    three_.close();
    four_.close();
    printf("Time take to parallely calculate convolution of the matrix using %d threads is %f seconds.\n", thread_count, omp_get_wtime() - start_time);

    // Storing covolution matrix in file
    ofstream mat_d;
    mat_d.open("ConvolutionMatrix.txt");
    for (i = 0; i < convolution_matrix_size; i++)
    {
        for (j = 0; j < convolution_matrix_size; j++)
        {
            // cout << convolution_matrix[i][j] << endl;
            mat_d << convolution_matrix[i][j] << " ";
        }
        mat_d << endl;
    }
    return 0;
}
