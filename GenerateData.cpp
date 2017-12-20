#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>

using namespace std;

const int NUM_OF_VERTEX = 64000;
int main(){

    ifstream fi;
    ofstream fo;
    long N, k;
    long u, v;
    long num_of_edges = 0;
    float weight;

    clock_t tStart = clock();

    fi.open("largeEWD.txt");
    fo.open("graph-64000.txt");
    fi >> N >> k;

    for(int i = 0; i < k; ++i){
        fi >> u >> v;
        fi >> weight;

        if(u < NUM_OF_VERTEX && v < NUM_OF_VERTEX){
            fo << u << " " << v << " " << weight << endl;
            ++num_of_edges;
        }
            
    }

    printf("\n\n==== Time to process data: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    cout << "N = " << N << " k = " << k << endl;
    cout << num_of_edges << endl;

    fi.close();
    fo.close();

    return 0;
}