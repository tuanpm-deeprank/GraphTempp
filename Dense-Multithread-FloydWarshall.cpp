// C Program for Floyd Warshall Algorithm
#include <stdio.h>
#include <iostream>
#include <vector> 
#include <fstream>
#include <time.h>
#include <thread>

#define INF 99999.0

using namespace std;

const int MAX_SIZE  = 10001;
// Number of vertices in the graph
int n;
// Input, Output Matrix
float graph[MAX_SIZE][MAX_SIZE];
vector< vector<float> > dist;
 
/* Define Infinite as a large enough value. This value will be used
  for vertices not connected to each other */
void printSolution();

// Node index start from 0
// Read input from file
template<typename T>
istream& binary_read(istream& stream, T& value){
    return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}
void loadData(const char * fileName) {
    ifstream fi(fileName, ifstream::binary);
    
    unsigned char d;
    binary_read(fi, n);
    cout << n << endl;
    for(int i = 0; i < n; ++i){
        if(i % 1000 == 0) printf("Reading data on row: %d\n", i);
        for(int j = 0; j < n; ++j){
            binary_read(fi, d);
            graph[i][j] = d;
        }
    }
}

void loadFLOATData(const char * fileName) {
    ifstream fi(fileName);
    
	fi >> n;
    cout << n << endl;
    float d;
    for(int i = 0; i < n; ++i){
        if(i % 1000 == 0) printf("Reading data on row: %d\n", i);
        for(int j = 0; j < n; ++j){
            fi >> d;
            graph[i][j] = d;
        }
    }
}
void floydWarshall ()
{
    int numthreads = thread::hardware_concurrency();
    printf("Num of threads: %d\n", numthreads);
    int rows = n / numthreads; 
    int extra = n % numthreads;
    int start = 0;
    int end = rows;

    auto code = [](int start, int end) -> void
    {
        for (int k = 0; k < n; k++)
        {
            clock_t t1 = clock();
            float disti_k_j, disti_j;
            // Pick all vertices as source one by one
            for (int i = start; i < end; i++)
            {
                //t2 = clock();
                // Pick all vertices as destination for the
                // above picked source
                for (int j = 0; j < n; j++)
                {
                    //t3 = clock();
                    // If vertex k is on the shortest path from
                    // i to j, then update the value of graph[i][j]
                    disti_k_j = graph[i][k] + graph[k][j];
                    disti_j = graph[i][j];
                    if (disti_k_j < disti_j)
                        graph[i][j] = graph[i][k] + graph[k][j];
                    //if(j % 1000 == 0) printf("\n\n==== Time to run iterator (3rd Loop): %.2fs\n", (double)(clock() - t3)/CLOCKS_PER_SEC);
                }
                //if(i % 100 == 0) printf("\n\n==== Time to run iterator (2nd Loop): %.2fs\n", (double)(clock() - t2)/CLOCKS_PER_SEC);
            }
            if(k % 1000 == 0)printf("\n\n==== Time to run iterator (1st Loop) k = %d - Thread = %d : %.2fs\n",k, this_thread::get_id(), (double)(clock() - t1)/CLOCKS_PER_SEC);
        }
    };

   
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
    // Print the shortest distance matrix
}
 
/* A utility function to print solution */
void printSolution()
{
    printf ("Following matrix shows the shortest distances"
            " between every pair of vertices \n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (dist[i][j] == INF)
                printf(" %5s ", "INF");
            else
                printf(" %4.3f ", dist[i][j]);
        }
        printf("\n");
    }
}
 
// driver program to test above function
int main()
{
	printf("Thread ID: %d\n", this_thread::get_id());
    clock_t tStart = clock();
    
	//loadData("graph-small.txt");	
    //loadData("graph-medium.txt");
    //loadData("graph-1000.txt");
    //loadData("graph-10000.txt");
    //loadData("largeEWD.txt");
    //loadData("90000.txt");
    loadFLOATData("DenseGraph-10000-FLOAT.bin");
    printf("==== Time to load data: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    tStart = clock();
    floydWarshall();
    printf("\n\n==== Time to run algorithm: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    return 0;
}

    
