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

// Số lượng định và tên file output
const char * filename = "DenseGraph-100.bin";
const int nVertices = 100;

std::random_device rd;
std::mt19937 gen(rd());
// Ham sinh gia tri float ngau nhien trong khoang (0.0, 5.0)
std::uniform_real_distribution<> dis(0.0, 5.0);

// Ham doc, ghi binary
template<typename T>
ostream& binary_write(ostream& stream, const T& value){
    return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}
template<typename T>
istream& binary_read(istream& stream, T& value){
    return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}
// Sinh graph dang binary
void DenseGraphBinary(){
    ofstream fo(filename, ofstream::binary);
    std::uniform_int_distribution<> idis(1, 255);

    binary_write(fo, nVertices);

    unsigned char d;
    for(int i = 0; i < nVertices; i++)
    {
        for(int j = 0; j < nVertices; ++j)
        {
            d = 0;
            if (i != j)
                d = idis(gen);
            binary_write(fo, d);   
        }
        
    }
    fo.close();
}
// Sinh graph dang float
void DenseGraphFloat(){
    FILE *file = fopen(filename, "wb");
    float d;

    fprintf(file, "%d\n", nVertices);
    for(int i = 0; i < nVertices; i++)
    {
        for(int j = 0; j < nVertices; ++j)
        {
            d = 0.0;
            if (i != j)
                d = dis(gen);
            fprintf(file, "%.2f ", d);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}
int main()
{
    time_t start = clock();
    DenseGraphBinary();
    printf("\n==== Time using to generate data: %.2fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
    return 0;
}
