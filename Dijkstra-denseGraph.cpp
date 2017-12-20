#include <stdio.h>
#include <limits.h>
#include <vector>
#include <fstream>
#include <time.h>
#include <ctime>
#include <iostream>

using namespace std;

const float INF = 9999999.0;
// Number of vertices in the graph
const int MAX_SIZE = 64000;
int N;
unsigned char graph[MAX_SIZE][MAX_SIZE];

FILE *file = NULL;

  
// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int minDistance(float dist[], bool sptSet[])
{
   // Initialize min value
   int min_index;
   float min = INF;
  
   for (int v = 0; v < N; v++)
     if (sptSet[v] == false && dist[v] <= min)
         min = dist[v], min_index = v;
  
   return min_index;
}
/*  
// A utility function to print the constructed distance array
int printSolution(float dist[], int n)
{
   fprintf(file, "\nVertex   Distance from Source\n");
   for (int i = 0; i < n; i++)
      fprintf(file, "%d \t \t %4.3f\n", i, dist[i]);
}

void printPathFromVertex(int parent[], int j)
{
    // Base Case : If j is source
    if (parent[j]==-1)
        return;
 
    printPathFromVertex(parent, parent[j]);
 
    fprintf(file, "%d ", j);
} 
void printPath(int parent[], float dist[], int src){
	fprintf(file, "\n\nVertex\t\tDistance\tPath");
    for (int i = 1; i < N; i++)
    {
        fprintf(file, "\n%d -> %d \t\t %4.3f\t\t%d ", src, i, dist[i], src);
        printPathFromVertex(parent, i);
    }
}
    
// Print Path 
*/ 
// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(int src)
{
     float dist[N];     // The output array.  dist[i] will hold the shortest
                      // distance from src to i
  
     bool sptSet[N]; // sptSet[i] will true if vertex i is included in shortest
                     // path tree or shortest distance from src to i is finalized
  
  	int parent[N];
     // Initialize all distances as INFINITE and stpSet[] as false
     for (int i = 0; i < N; i++){
     	parent[src] = -1;
     	dist[i] = INF;
		sptSet[i] = false;
	 }
        
  
     // Distance of source vertex from itself is always 0
     dist[src] = 0;
  
     int graph_u_v;
     // Find shortest path for all vertices
     for (int count = 0; count < N-1; count++)
     {
       // Pick the minimum distance vertex from the set of vertices not
       // yet processed. u is always equal to src in first iteration.
       int u = minDistance(dist, sptSet);
  
       // Mark the picked vertex as processed
       sptSet[u] = true;
  
       // Update dist value of the adjacent vertices of the picked vertex.
       for (int v = 0; v < N; v++){
         graph_u_v = (int) graph[u][v];
         // Update dist[v] only if is not in sptSet, there is an edge from 
         // u to v, and total weight of path from src to  v through u is 
         // smaller than current value of dist[v]
         if (!sptSet[v] && graph_u_v && dist[u] != INT_MAX 
                                       && dist[u]+graph_u_v < dist[v])
        {
            parent[v] = u;                  	
            dist[v] = dist[u] + graph_u_v;
        }
       }
    }
  
     // print the constructed distance array
     //printSolution(dist, N);
     //printPath(parent, dist, src);
}

template<typename T>
istream& binary_read(istream& stream, T& value){
    return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}

void loadData(const char * fileName) 
{	
    ifstream fi(fileName, ifstream::binary);
    
    unsigned char d;
    binary_read(fi, N);
    cout << N << endl;
    for(int i = 0; i < N; ++i){
        if(i % 1000 == 0) printf("Reading data on row: %d\n", i);
        for(int j = 0; j < N; ++j){
            binary_read(fi, d);
            graph[i][j] = d;
        }
    }
    fi.close();
}

// driver program to test above function
int main()
{
    //file = fopen("Dijkstra-output.txt", "w");
	clock_t tStart = clock();
    loadData("DenseGraph-10000.bin");
    printf("==== Time to load data: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    

    tStart = clock();
    clock_t forIterator;
    for(int i = 0; i < N; ++i){
    	forIterator = clock();
        dijkstra(i);
        printf("\n\n== Finish Process on Vertice %d after %.2fs\n\n", i, (double)(clock() - forIterator)/CLOCKS_PER_SEC);
    }
    printf("\n\n==== Time to run algorithm: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    return 0;
}
