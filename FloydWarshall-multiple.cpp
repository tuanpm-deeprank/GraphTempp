// C Program for Floyd Warshall Algorithm
#include <stdio.h>
// C Program for Floyd Warshall Algorithm
#include <stdio.h>
#include <iostream>
#include <vector> 
#include <fstream>
#include <time.h>
#include <thread>

using namespace std;

const char * input = "DenseGraph-500-FLOAT.bin";
const char * output = "Floyd-Float-output.txt";

// Number of vertices in the graph
int n;
// Input, Output Matrix
vector< vector<float> > graph;
vector< vector<int> > path;
FILE * file;

// write solution to file 
void printSolution();
// load data from file
void loadData();
// Implementing FloyWarshall Algorithm
void FloydWarshall(); 

void printSolution1()
{
	// Print distance between 2 nodes
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
			printf("%4.2f ", graph[i][j]);
        }
        printf("\n");
    }
   
    // Print path between 2 nodes
    int src, dst;
   for (int u = 0; u < n; u++)
   {
       for (int v = 0; v < n; v++)
        {
            src = u;
            dst = v;
            printf("%d ", src);
            while(src != dst){
                src = path[src][dst];
                printf("%d ", src);
            }
            printf("\n");
        }
    }
}

// driver program to test above function
int main()
{
	// Loading Data and printing time using to load Data
    clock_t tStart = clock();
    loadData();
    printf("==== Time to load data: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    // Running algorithm and printing time using to run algorithm
    time_t start_time, end_time;
    time(&start_time);
    FloydWarshall();
    time(&end_time);
    printf("==== Time to run algorithm: %.2fs\n", difftime(end_time, start_time));
    
    return 0;
}

// Implementing printSolution Function
void printSolution()
{
	file = fopen(output, "w");
	// Print distance between 2 nodes
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
			fprintf(file, "%4.2f ", graph[i][j]);
        }
        fprintf(file, "\n");
    }
   
    // Print path between 2 nodes
    int src, dst;
   for (int u = 0; u < n; u++)
   {
       for (int v = 0; v < n; v++)
        {
            src = u;
            dst = v;
            fprintf(file, "%d ", src);
            while(src != dst){
                src = path[src][dst];
                fprintf(file, "%d ", src);
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

// Implementing loadData Function
void loadData() {
    ifstream fi(input);	
    float d;

	fi >> n;

    printf("Loading data...\n\n");
    path.resize(n);
    graph.resize(n);
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            fi >> d;
            graph[i].push_back(d);
            path[i].push_back(j);
        }
    }
    
    fi.close();
}

// Implementing FloydWarshall Algorithm
void FloydWarshall()
{
    int numthreads = thread::hardware_concurrency();
    printf("Num of threads: %d\n", numthreads);

    int rows = n / numthreads; 
    int extra = n % numthreads;
    int start = 0;
    int end = rows;

    printf("Running Algorithm...\n\n");

    auto code = [](int start, int end) -> void
    {
        for (int k = 0; k < n; k++)
        {
            // Pick all vertices as source one by one
            for (int i = start; i < end; i++)
            {
                // Pick all vertices as destination for the
                // above picked source
                for (int j = 0; j < n; j++)
                {
                    // If vertex k is on the shortest path from
                    // i to j, then update the value of graph[i][j]
                    if (graph[i][k] + graph[k][j] < graph[i][j]) {
                        graph[i][j] = graph[i][k] + graph[k][j];
                        path[i][j] = path[i][k];
                    }    
                }
            }
        }
    };

    vector<thread> workers;
    for(int t = 1; t <= numthreads; t++)
    {
        if (t == numthreads)
            end += extra;
        workers.push_back(thread(code, start, end));
        start = end;
        end = start + rows;
    }
    for(thread& th : workers)
        th.join();

    printf("Saving Output...\n\n");
    printSolution();
}
