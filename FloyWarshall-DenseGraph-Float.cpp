// C Program for Floyd Warshall Algorithm
#include <stdio.h>
#include <iostream>
#include <vector> 
#include <fstream>
#include <time.h>

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

// driver program to test above function
int main()
{
	// Loading Data and printing time using to load Data
    clock_t tStart = clock();
    loadData();
    printf("==== Time to load data: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    // Running algorithm and printing time using to run algorithm
    tStart = clock();
    FloydWarshall();
    printf("==== Time to run algorithm: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
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
    int i, j, k;
 
    printf("Running Algorithm...\n\n");
    for (k = 0; k < n; k++)
    {
        // Pick all vertices as source one by one
        for (i = 0; i < n; i++)
        {
            // Pick all vertices as destination for the
            // above picked source
            for (j = 0; j < n; j++)
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
    // Print the shortest distance matrix
    printf("Saving Ouput...\n\n");
    printSolution();
}
