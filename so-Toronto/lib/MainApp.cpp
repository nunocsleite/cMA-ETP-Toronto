
#include <string>
#include <iostream>

// Handling Ctrl+c signal
#include <signal.h>

bool volatile keepRunning = true;

void intHandler(int dummy=0) {

//    std::cout << "Catch signal" << std::endl;
//    std::cin.get();
    keepRunning = false;
}

using namespace std;



// Toronto benchmarks
extern void runTorontoDatasets(int _datasetIndex, string const& _testBenchmarksDir, const string& _outputDir);


void runAlgo(int _datasetIndex, string const& _testBenchmarksDir, const string& _outputDir) {
    // Handling Ctrl+c signal
    signal(SIGINT, intHandler);

    // Run optimization algorithm on the Toronto datasets
    runTorontoDatasets(_datasetIndex, _testBenchmarksDir, _outputDir);
}













