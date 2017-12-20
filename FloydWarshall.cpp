// C Program for Floyd Warshall Algorithm
#include <stdio.h>
#include <iostream>
#include <vector> 
#include <fstream>
#include <time.h>

#define INF 99999.0

using namespace std;

// Number of vertices in the graph
int n;
// Input, Output Matrix
vector< vector<int> > graph;
vector< vector<float> > dist;
 
/* Define Infinite as a large enough value. This value will be used
  for vertices not connected to each other */
void printSolution();

// Node index start from 0
// Read input from file
void loadData(const char * fileName) {
	int k, u, v;
	float d;
	ifstream fi;
	
	fi.open(fileName);
	
	fi >> n >> k;
	
	dist.resize(n);
	for(int i = 0; i < n; ++i) {
		dist[i].resize(n, INF);
	}
		
	
	for(int i = 0; i < k; ++i) {
		fi >> u >> v;
		fi >> d;
		dist[u][v] = d;
	}
	
    for(int i = 0; i < n; ++i)
    	dist[i][i] = 0;
    	
    //printSolution();
	
}

void floydWarshall ()
{
    int i, j, k;
 
    clock_t tStart;
    for (k = 0; k < n; k++)
    {
        tStart = clock();
        // Pick all vertices as source one by one
        for (i = 0; i < n; i++)
        {
            // Pick all vertices as destination for the
            // above picked source
            for (j = 0; j < n; j++)
            {
                // If vertex k is on the shortest path from
                // i to j, then update the value of dist[i][j]
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
        if( k % 50 == 0)
            printf("\n\n==== Time to run iterator %d: %.2fs\n", k, (double)(clock() - tStart)/CLOCKS_PER_SEC);

    }
 
    // Print the shortest distance matrix
    // printSolution();
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
    clock_t tStart = clock();
    
	//loadData("graph-small.txt");	
    //loadData("graph-medium.txt");
    //loadData("graph-1000.txt");
    loadData("graph-10000.txt");
    //loadData("largeEWD.txt");
    //loadData("90000.txt");
    clock_t tEnd = clock();
    printf("==== Time to load data: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    tStart = clock();
    floydWarshall();
    tEnd = clock();
    printf("\n\n==== Time to run algorithm: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    return 0;
}
