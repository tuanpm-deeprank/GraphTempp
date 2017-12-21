// C Program for Floyd Warshall Algorithm
#include <stdio.h>
#include <iostream>
#include <vector> 
#include <fstream>
#include <time.h>

using namespace std;

const int MAX_NUM_OF_NODES  = 10000;
const float INF = 9999999.0;

const bool ON_DEBUG = true;
// Number of vertices in the graph
int n;
// Input, Output Matrix
float graph[MAX_NUM_OF_NODES][MAX_NUM_OF_NODES];
FILE * file;

// write solution to file 
void printSolution(vector< vector<int>> path);
// print Path between 2 nodes
void printPath(vector< vector<int>> path, int u, int v);
// load data from file
void loadData(const char * fileName);
// Implementing FloyWarshall Algorithm
void FloydWarshall(); 

// driver program to test above function
int main()
{
	// Loading Data and printing time using to load Data
    clock_t tStart = clock();
    loadData("DenseGraph-1000-FLOAT.bin");
    printf("==== Time to load data: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    // Running algorithm and printing time using to run algorithm
    tStart = clock();
    FloydWarshall();
    printf("\n\n==== Time to run algorithm: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    return 0;
}

// Implementing printSolution Function
void printSolution(vector< vector<int>> path)
{
	file = fopen("Floyd-Float-output.txt", "w");
	// Print distance between 2 nodes
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
			fprintf(file, " %4.2f ", graph[i][j]);
        }
        fprintf(file, "\n");
    }
    
    // Print path between 2 nodes
//    for (int u = 0; u < n; u++)
//    {
//    	printf("\nu = %d", u);
//    	clock_t t = clock();
//        for (int v = u+1; v < n; v++)
//        {
//        	printf("\nv = %d", v);
//        	printPath(path, u, v);
//        }
//        printf("\nTime %.2fs\n",(double)(clock() - t)/CLOCKS_PER_SEC);
//    }
}

// Implementing path between 2 nodes
void printPath(vector< vector<int>> path, int u, int v) {
	printf("%d %d %d\n", u, v, path[u][v]);
	if(u == v)
		return;
	if(path[u][v] == -1)
		fprintf(file, "%d %d ", u, v);
	else {
		printPath(path, u, path[u][v]);
		printPath(path, path[u][v], v);
	}
	
}
// Implementing loadData Function
void loadData(const char * fileName) {
    ifstream fi(fileName);
	
	fi >> n;
	
    if(ON_DEBUG) cout << n << endl;
    float d;
    for(int i = 0; i < n; ++i){
        if(ON_DEBUG)
			if(i % 1000 == 0) printf("Reading data on row: %d\n", i);
        for(int j = 0; j < n; ++j){
            fi >> d;
            graph[i][j] = d;
        }
    }
    
    fi.close();
}

// Implementing FloydWarshall Algorithm
void FloydWarshall()
{
    int i, j, k;
    clock_t t;
    vector< vector<int>> path;
    
    path.resize(n);
    for(int u = 0; u < n; ++u)
    {
    	path[u].resize(n);
    	for(int v = 0; v < n; ++v) {
    		path[u].push_back(-1);
		}
	}
 
    for (k = 0; k < n; k++)
    {
        t = clock();
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
                	path[i][j] = k;
				}    
            }
        }
        if(ON_DEBUG)
			if(k % 1000 == 0)
				printf("\n\n==== Time to run iterator on k = %d: %.2fs\n",k, (double)(clock() - t)/CLOCKS_PER_SEC);
    }
 
 	printf("Finish\n");
    // Print the shortest distance matrix
    printSolution(path);
}
