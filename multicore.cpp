// C / C++ program for Dijkstra's shortest path algorithm for adjacency
// list representation of graph

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <thread>


using namespace std;

const int NUM_OF_THREADS = 4;

FILE * file = NULL;
int N;

// A structure to represent a node in adjacency list
struct AdjListNode
{
	int dest;
	float weight;
	struct AdjListNode* next;
};

// A structure to represent an adjacency list
struct AdjList
{
	struct AdjListNode *head; // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
	int V;
	struct AdjList* array;
};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, float weight)
{
	struct AdjListNode* newNode =
			(struct AdjListNode*) malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
	struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
	graph->V = V;

	// Create an array of adjacency lists. Size of array will be V
	graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

	// Initialize each adjacency list as empty by making head as NULL
	for (int i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, float weight)
{
	// Add an edge from src to dest. A new node is added to the adjacency
	// list of src. The node is added at the begining
	struct AdjListNode* newNode = newAdjListNode(dest, weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	// If our graph is undirected, we need to add an edge from dest to src
	/* 
	newNode = newAdjListNode(src, weight);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
	*/
}

// Structure to represent a min heap node
struct MinHeapNode
{
	int v;
	float dist;
};

// Structure to represent a min heap
struct MinHeap
{
	int size;	 // Number of heap nodes present currently
	int capacity; // Capacity of min heap
	int *pos;	 // This is needed for decreaseKey()
	struct MinHeapNode **array;
};

// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, float dist)
{
	struct MinHeapNode* minHeapNode =
		(struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap* minHeap =
		(struct MinHeap*) malloc(sizeof(struct MinHeap));
	minHeap->pos = (int *)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array =
		(struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size &&
		minHeap->array[left]->dist < minHeap->array[smallest]->dist )
	smallest = left;

	if (right < minHeap->size &&
		minHeap->array[right]->dist < minHeap->array[smallest]->dist )
	smallest = right;

	if (smallest != idx)
	{
		// The nodes to be swapped in min heap
		MinHeapNode *smallestNode = minHeap->array[smallest];
		MinHeapNode *idxNode = minHeap->array[idx];

		// Swap positions
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;

		// Swap nodes
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
	}
}

// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
	return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	if (isEmpty(minHeap))
		return NULL;

	// Store the root node
	struct MinHeapNode* root = minHeap->array[0];

	// Replace root node with last node
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node
	minHeap->pos[root->v] = minHeap->size-1;
	minHeap->pos[lastNode->v] = 0;

	// Reduce heap size and heapify root
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

// Function to decreasy dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, float dist)
{
	// Get the index of v in heap array
	int i = minHeap->pos[v];

	// Get the node and update its dist value
	minHeap->array[i]->dist = dist;

	// Travel up while the complete tree is not hepified.
	// This is a O(Logn) loop
	while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
	{
		// Swap this node with its parent
		minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
		minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
		swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

		// move to parent index
		i = (i - 1) / 2;
	}
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
if (minHeap->pos[v] < minHeap->size)
	return true;
return false;
}

// A utility function used to print the solution
void printArr(float dist[], int n, int src)
{
	fprintf(file, "\n\nVertex   Distance from vertex %d \n", src);
	for (int i = 0; i < n; i++)
	   fprintf(file, "%d \t \t \t \t %4.3f\n", i, dist[i]);
}

// print Path
void printPathFromVertex(int parent[], int j)
{
    // Base Case : If j is source
    if (parent[j]==-1)
        return;
 
    printPathFromVertex(parent, parent[j]);
 
    fprintf(file, "%d ", j);
} 

void printPath(int parent[], float dist[], int src){
	float INF = 9999999.0;
	fprintf(file, "\n\nVertex\t\tDistance\tPath");
    for (int i = 1; i < N; i++)
    {
		fprintf(file, "\n%d -> %d \t\t ", src, i);
		if(dist[i] == INF)
			fprintf(file, "INF\t\t");
		else{
			fprintf(file, "%4.3f\t\t%d ", dist[i], src);
			printPathFromVertex(parent, i);
		}
    }
}



// The main function that calulates distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
void dijkstra(struct Graph* graph, int src)
{
	float INF = 9999999.0;
	struct MinHeap* minHeap;
	struct MinHeapNode* minHeapNode;
	struct AdjListNode* pCrawl;

    int V = graph->V;// Get the number of vertices in graph
    
    float dist[V];
    int parent[V];
    
    parent[src] = -1;
	// minHeap represents set E
	minHeap = createMinHeap(V);

	// Initialize min heap with all vertices. dist value of all vertices 
	for (int v = 0; v < V; ++v)
	{
        dist[v] = INF;
		minHeap->array[v] = newMinHeapNode(v, dist[v]);
		minHeap->pos[v] = v;
	}
	// Make dist value of src vertex as 0 so that it is extracted first
	minHeap->array[src] = newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
    dist[src] = 0;
    
	decreaseKey(minHeap, src, dist[src]);

	// Initially size of min heap is equal to V
	minHeap->size = V;

	// In the followin loop, min heap contains all nodes
	// whose shortest distance is not yet finalized.
	while (!isEmpty(minHeap))
	{
		// Extract the vertex with minimum distance value
		minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v; // Store the extracted vertex number

		// Traverse through all adjacent vertices of u (the extracted
		// vertex) and update their distance values
		pCrawl = graph->array[u].head;
		while (pCrawl != NULL)
		{
			int v = pCrawl->dest;

			// If shortest distance to v is not finalized yet, and distance to v
			// through u is less than its previously calculated distance
			if (isInMinHeap(minHeap, v) && dist[u] != INF && 
										pCrawl->weight + dist[u] < dist[v])
			{
				dist[v] = dist[u] + pCrawl->weight;
				
				parent[v] = u;

				// update distance value in min heap also
				decreaseKey(minHeap, v, dist[v]);
			}
			pCrawl = pCrawl->next;
		}
        delete minHeapNode;
        minHeapNode = NULL;
        delete pCrawl;
		pCrawl = NULL;
	}

	// print the calculated shortest distances
    // printArr(dist, V, src);
    // print path
    // printPath(parent, dist, src);
    // Clear memory
    delete minHeap->array;
    delete minHeap->pos;
    delete minHeap;
	minHeap = NULL;
}

struct Graph *graphs[NUM_OF_THREADS];

void loadData(const char * fileName) {
	int k, u, v;
	float d;
	ifstream fi;

	fi.open(fileName);
	
	fi >> N >> k;
	
	for(int i = 0; i < NUM_OF_THREADS; ++i)
		graphs[i] = createGraph(N);
	
	for(int i = 0; i < k; ++i) {
		fi >> u >> v;
		fi >> d;
		for(int j = 0; j < NUM_OF_THREADS; ++j)
			addEdge(graphs[j], u, v, d);
	}

	fi.close();
}


void runDijkstra(int from, int to, struct Graph * graph){
	for(int i = from; i < to; ++i)
	{
		dijkstra(graph, i);
	}
}
// Driver program to test above functions
int main()
{
	int i;
	thread threads[NUM_OF_THREADS];

	clock_t tStart = clock();
	//loadData("graph-small.txt");
    //loadData("graph-medium.txt");
    //loadData("graph-1000.txt");
	//loadData("graph-10000.txt");
	//loadData("graph-64000.txt");
	//loadData("largeEWD.txt");
	loadData("90000.txt");
    printf("==== Time to load data: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


	tStart = clock();

	dijkstra(graphs[0], 1000);
	printf("==== Nomarlize Time: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	for(i = 0; i < (NUM_OF_THREADS-1); ++i){
		threads[i] = thread(runDijkstra, i*N/4, (i+1)*N/4, graphs[i]);
	}
	threads[i] = thread(runDijkstra, i*N/4, N, graphs[i]);
	for(i = 0; i < NUM_OF_THREADS; ++i)
		if(threads[i].joinable())
			threads[i].join();
	printf("\n==== Time to run algorithm using %d threads: %.2fs\n", NUM_OF_THREADS, (double)(clock() - tStart)/CLOCKS_PER_SEC);
	
	return 0;
}

