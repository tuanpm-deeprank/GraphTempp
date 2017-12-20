#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    cout << "For Test" << endl;

    int start = 11;
    int end = 123;
    string s = "Output-" + to_string(11) + "-" + to_string(12) + ".txt";
    char c[100];
    sprintf(c, "Output-%d-%d.txt", start, end);
    const char *ce = c;
    string fn = c;
    const char * file = s.c_str();
    cout << file << endl;
    cout << fn << "  " << fn.length() << endl;

    return 0;
}
