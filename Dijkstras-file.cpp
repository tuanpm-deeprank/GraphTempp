#include <stdio.h>
#include <limits.h>
#include <vector>
#include <fstream>
#include <time.h>
#include <ctime>

using namespace std;

const float INF = 9999999.0;
// Number of vertices in the graph
int N;
vector< vector<float> > graph;
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
  
     // Find shortest path for all vertices
     for (int count = 0; count < N-1; count++)
     {
       // Pick the minimum distance vertex from the set of vertices not
       // yet processed. u is always equal to src in first iteration.
       int u = minDistance(dist, sptSet);
  
       // Mark the picked vertex as processed
       sptSet[u] = true;
  
       // Update dist value of the adjacent vertices of the picked vertex.
       for (int v = 0; v < N; v++)
  
         // Update dist[v] only if is not in sptSet, there is an edge from 
         // u to v, and total weight of path from src to  v through u is 
         // smaller than current value of dist[v]
         if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX 
                                       && dist[u]+graph[u][v] < dist[v]){
            parent[v] = u;                  	
            dist[v] = dist[u] + graph[u][v];
        }
     }
  
     // print the constructed distance array
     printSolution(dist, N);
     printPath(parent, dist, src);
}

void loadData(const char * fileName) {
	int k, u, v;
	float d;
	ifstream fi;
	
	fi.open(fileName);
	
	fi >> N >> k;
	
	graph.resize(N);
	for(int i = 0; i < N; ++i) {
		graph[i].resize(N, INF);
	}
	
	for(int i = 0; i < k; ++i) {
		fi >> u >> v;
		fi >> d;
        graph[u][v] = d;
    }
    
    fi.close();
}

// driver program to test above function
int main()
{
    file = fopen("Dijkstra-output.txt", "w");

	clock_t tStart = clock();
    
	//loadData("graph-small.txt");	
    //loadData("graph-medium.txt");
    //loadData("graph-1000.txt");
    //loadData("graph-10000.txt");
    loadData("graph-64000.txt");

    clock_t tEnd = clock();
    printf("==== Time to load data: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    

    /*
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    fo << "\n\n================ LOG TIME ================\n"
	 << (now->tm_year + 1900) << '-' 
     << (now-> tm_mon + 1) << '-'
     << now-> tm_mday << "  "
     << now -> tm_hour << ":"
     << now -> tm_min << ":"
     << now -> tm_sec;
    */ 

    tStart = clock();
    for(int i = 0; i < N; ++i){
    	// printf("\n\n== Start from %d \n\n", i);
    	// fo << "\n\n******  Start from vertex " << i << "  ******\n";
    	dijkstra(i);
    }
    tEnd = clock();
    printf("\n\n==== Time to run algorithm: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    
  
  
    return 0;
}
