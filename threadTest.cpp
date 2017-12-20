#include <iostream>
#include <vector>
#include <stdio.h>
#include <thread>
#include <time.h>

using namespace std;

int main() {

    int N = 1000;
    int **A, **B, **C;

    A = new int *[N];
    B = new int *[N];
    C = new int *[N];
    for(int i = 0; i < N; ++i){
        A[i] = new int [N];
        B[i] = new int [N];
        C[i] = new int [N];
    }

    time_t st = clock();
    for(int i = 0; i < N ; i++)
        for(int j = 0; j < N; j++)
        {
            C[i][j] = 0;
            
            for(int k = 0; k < N; ++k)
                C[i][j] += (A[i][k] * B[k][j]); 
        }
    printf("\n\n==== Time to Serial Version: %.2fs\n", (double)(clock() - st)/CLOCKS_PER_SEC);



    
    int numthreads = thread::hardware_concurrency();
    printf("Num of threads: %d\n", numthreads);
    int rows = N / numthreads; 
    int extra = N % numthreads;
    int start = 0;
    int end = rows;

    auto code = [ N, &C, &A, &B](int start, int end) -> void
    {
        for(int i = start; i < end ; i++)
            for(int j = 0; j < N; j++)
            {
                C[i][j] = 0;
                
                for(int k = 0; k < N; ++k)
                    C[i][j] += (A[i][k] * B[k][j]); 
            }
    };

    st = clock();
    vector<thread> workers;
    for( int t = 1; t <= numthreads; t++)
    {
        if (t == numthreads)
            end += extra;
        workers.push_back(thread(code, start, end));
        start = end;
        end = start + rows;
    }

    for(thread& th : workers)
        th.join();

    printf("\n\n==== Time to Parallel Version (%d threads): %.2fs\n", numthreads, (double)(clock() - st)/CLOCKS_PER_SEC);
    return 0;
}