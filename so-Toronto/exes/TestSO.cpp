
#include <cmath>
#include <iostream>
#include <stdlib.h>

using namespace std;


extern void runAlgo(int _datasetIndex, string const& _testBenchmarksDir, string const& _outputDir);



int main(int argc, char* argv[])
{
    if (argc != 4) {
        cout << "Usage: ./exes <test set index [0..N-1]>   <test benchmarks directory>   <output directory>" << endl;
        cout << "   Example: ./exes 2 ./../../ETTP-Benchmarks/Toronto/all_file ./Run1/" << endl;
    }
    // Get dataset index
    int datasetIndex = atoi(argv[1]);
    // Get test benchmarks directory
    string testBenchmarksDir = argv[2];
    // Get output directory
    string outputDir = argv[3];

    runAlgo(datasetIndex, testBenchmarksDir, outputDir);

    return 0;
}









