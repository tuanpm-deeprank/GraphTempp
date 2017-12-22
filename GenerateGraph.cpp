#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <fstream>
#include <time.h>
#include <set>

using namespace std;


const char * filename = "DenseGraph-3000.bin";
const int nVertices = 10;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 5.0);

template<typename T>
ostream& binary_write(ostream& stream, const T& value){
    return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

// Do thi tu mot dinh bat ki co duong di den N-1 dinh con lai
// D(u, V) = D(V, u)
void DenseAndSymmetricGraph(int numOfVertices){
    FILE *file = fopen("DenseSymmetricGraph.bin", "w");

    // Tu mot 1 dinh ta ghi ra thong tin ve duong di tu dinh do den n-1 dinh con lai
    int k = numOfVertices * (numOfVertices - 1)/2;
    fprintf(file, "%d\n", numOfVertices);
    fprintf(file, "%d\n", k);

    float d;
    for(int i = 0; i < numOfVertices/100; i++)
    {
        time_t t1 = clock();
        for(int j = i; j < numOfVertices; ++j)
        {
            if (i != j) // chi xet cac dinh j != i
            {
                d = dis(gen);
                fprintf(file, "%d %d %.2f\n", i, j, d); // Do thi doi xung d[i, j] = d[j, i]
                //fprintf(file, "%d %d %.2f\n", j, i, d);
            }
        }
        printf("Time in iterator : %.2fs\n", (double)(clock() - t1)/CLOCKS_PER_SEC);
    }
    fclose(file);
}

// Do thi tu mot dinh bat ki co duong di den N-1 dinh con lai
// D(u, v) != D(v, u)
void DenseGraph(int numOfVertices){
    ofstream fo(filename, ofstream::binary);
    //FILE *file = fopen("DenseGraph2.bin", "wb");

    std::uniform_int_distribution<> idis(1, 255);

    // Tu mot 1 dinh ta ghi ra thong tin ve duong di tu dinh do den n-1 dinh con lai
    //long long k = (long long)numOfVertices * (numOfVertices - 1);
    //cout << numOfVertices << " " << k << endl;
    //fo << numOfVertices << endl;
    //fo << k << endl;
    //fprintf(file, "%d\n", numOfVertices);
    //printf("Num of edges: %lld\n", k);
    //fprintf(file, "%lld\n", k);
    binary_write(fo, numOfVertices);
    //binary_write(fo, k);

    unsigned char d;
    for(int i = 0; i < numOfVertices; i++)
    {
        if( i % 1000 == 0 ) printf("Processing on Vertex: %d\n", i);
        for(int j = 0; j < numOfVertices; ++j)
        {
            d = 0;
            if (i != j) // cac dinh i != j sinh ngau nhien weight
                d = idis(gen);
            if(i < 8  && j < 8)
                cout << (int)d << " ";
           // fprintf(file, "%d %d %u\n", i, j, d);
            //fo << i << " " << j << " " << d << endl;
            //binary_write(fo, i);
            //binary_write(fo, j);
            binary_write(fo, d);
                
        }
        if(i < 8) cout << endl;
    }
    //fclose(file);
    fo.close();
}
void DenseGraph_FLOAT(int numOfVertices){
    FILE *file = fopen("DenseGraph-10-FLOAT.bin", "wb");
   
    fprintf(file, "%d\n", numOfVertices);
 
    float d;
    for(int i = 0; i < numOfVertices; i++)
    {
        if( i % 1000 == 0 ) printf("Processing on Vertex: %d\n", i);
        for(int j = 0; j < numOfVertices; ++j)
        {
            d = 0.0;
            if (i != j) // cac dinh i != j sinh ngau nhien weight
                d = dis(gen);
            if(i< 8  && j < 8)
                cout << d << " ";
            fprintf(file, "%.2f ", d);
        }
        fprintf(file, "\n");
        if(i < 8) cout << endl;
    }
    fclose(file);
}

class Edge {
public:
    int u;
    int v;

    Edge(): u(0), v(0) {}
    Edge(int s, int f): u(s), v(s) {}

    bool operator< (const Edge & edge) const {
        return ((this->u != edge.u) || (this->v != edge.v));
    }

    void setValue(int s, int f){
        this->u = s;
        this->v = f;
    }
};
// Do thi thua
// Truyen vao so canh
void SparseGraph(int numOfVertices, int numofEdges){
    std::uniform_int_distribution<> idis(0, numOfVertices-1);
    FILE *file = fopen("SparseGraph.txt", "w");
    
    fprintf(file, "%d\n", numOfVertices);
    fprintf(file, "%d\n", numofEdges);

    float d;
    int u, v;
    set<Edge> existingEdge;
    for(int i = 1; i <= numofEdges; i++)
    {
        Edge e;
        do
        {
            u = idis(gen);
            v = idis(gen);
            e.setValue(u, v);
        }while( (u == v) || (existingEdge.find(e) != existingEdge.end()));  // Dam bao rang u != v, va khong co cap dinh nao bi lap lai
        d = dis(gen);
        existingEdge.insert(e);
        fprintf(file, "%d %d %.2f\n", u, v, d);
    }
    fclose(file);

}
template<typename T>
istream& binary_read(istream& stream, T& value){
    return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}

int main()
{
    int N = 64000;

    time_t start = clock();
    //DenseAndSymmetricGraph(64000);
    //DenseGraph(nVertices);
    //SparseGraph(20, 20);
    DenseGraph_FLOAT(nVertices);
    printf("\n==== Time using to generate data: %.2fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);

    //  ifstream fi("DenseGraph2.bin", ifstream::binary);
    //  ofstream fo("DenseGraph3.bin", ofstream::binary);
    //  int n;
    //  long long k;
    //  int u, v;
    //  unsigned char d;
    //  fi >> n >> k;
    //  binary_write(fo, n);
    //  for(int i = 0; i < n; ++i)
    //  {
    //     for(int j = 0; j < n; ++j) 
    //     {
    //         fi >> u >> v >> d;
    //         binary_write(fo, d);
    //     }
    // }
    // fo.close();
    // fi.close();
    //binary_read(fi, n);
    //binary_read(fi, k);
    // fi >> n >> k;
    // printf("%d %lld\n", n, k);
    // fi.close();
    //ifstream fi(filename);
    //FILE *file = fopen("DenseGraph4.bin", "w");
    
    ifstream fi("DenseGraph-500-FLOAT.bin");
    int n;
    fi >> n;
    float d;
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < n; ++j) {
            fi >> d;
            if(j < 8)
                cout << d << " ";
        }
        cout << endl;
    }
//     int n;
//     unsigned char d;
//     binary_read(fi, n);
//     cout << n << endl;
//     //long long k = (long long)n*(n-1);
//     //fprintf(file, "%d\n", n);
//     //fprintf(file, "%lld\n", k);
//     for(int i = 0; i < 8; ++i){
//         for(int j = 0; j < n; ++j){
//             binary_read(fi, d);
//             if(j < 8) cout << (int)d << " ";
//             //fprintf(file, "%d %d %u\n", i, j, d);
//         }
//         cout << endl;
//     }
//    
    return 0;
}
