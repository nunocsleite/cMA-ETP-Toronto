#include <eo>

#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "testset/TestSetDescription.h"
#include "init/ETTPInit.h"

#include "neighbourhood/ETTPneighborEval.h"
#include "eval/eoETTPEval.h"
#include "neighbourhood/ETTPneighbor.h"
#include "toronto/TorontoTestSet.h"
#include "algorithms/eo/eoCellularEARing.h"
#include "algorithms/eo/eoCellularEAMatrix.h"
#include "algorithms/eo/Mutation.h"
#include "algorithms/eo/Crossover.h"

//#include "eoSimpleGA.h"
//#include "algorithms/eo/eoGenerationContinue.h"
#include "algorithms/eo/eoGenerationContinuePopVector.h"
#include "eoSelectOne.h"
#include "algorithms/eo/eoSelectBestOne.h"
//#include <eoDetSelect.h>
#include "algorithms/eo/eoDeterministicTournamentSelectorPointer.h" // eoDeterministicTournamentSelector using boost::shared_ptr

// For counting the # evaluations
#include "eval/eoNumberEvalsCounter.h"
#include "eval/statistics/eoETTPEvalWithStatistics.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "statistics/ExamMoveStatistics.h"



using namespace std;



#define MAINAPP_DEBUG


extern int getSANumberEvaluations(double tmax, double r, double k, double tmin);
//
// Rong Qu's exam evaluator
//
extern void runExamEvaluator(string const& _torontoDatasetPath, string const& _outputDir, string const& _torontoDatasetName);



// These function is defined below
void runCellularEA(string const& _outputDir, TestSet const& _testSet);
void generateExamMoveStatistics(const string &_outputDir, const TestSet &_testSet);



void runTorontoDatasets(int _datasetIndex, string const& _testBenchmarksDir, string const& _outputDir) {

    vector<TestSetDescription> torontoTestSet;
    //
    // To evaluate the algorithms, we use Version I of Toronto benchmarks
    // TestSetDescription info: <name>, <description>, <# time slots>
    //
    torontoTestSet.push_back(TestSetDescription("car-f-92", "Carleton University",                   32));
    torontoTestSet.push_back(TestSetDescription("car-s-91", "Carleton University",                   35));
    torontoTestSet.push_back(TestSetDescription("ear-f-83", "Earl Haig Collegiate",                  24));
    torontoTestSet.push_back(TestSetDescription("hec-s-92", "Ecole des Hautes Etudes Commerciales",  18));
    torontoTestSet.push_back(TestSetDescription("kfu-s-93", "King Fahd University",                  20));
    torontoTestSet.push_back(TestSetDescription("lse-f-91", "London School of Economics",            18));
    torontoTestSet.push_back(TestSetDescription("pur-s-93", "Purdue University",                     42));
    torontoTestSet.push_back(TestSetDescription("rye-s-93", "Ryerson University",                    23));
    torontoTestSet.push_back(TestSetDescription("sta-f-83", "St. Andrews High school",               13));
    torontoTestSet.push_back(TestSetDescription("tre-s-92", "Trent University",                      23));
    torontoTestSet.push_back(TestSetDescription("uta-s-92", "University of Toronto, Arts & Science", 35));
    torontoTestSet.push_back(TestSetDescription("ute-s-92", "University of Toronto, Engineering",    10));
    torontoTestSet.push_back(TestSetDescription("yor-f-83", "York Mills Collegiate",                 21));
    /////////////////////////////////////////////////////////////////////////////////////////////////
//    copy(torontoTestSet.begin(), torontoTestSet.end(), ostream_iterator<TestSetDescription>(cout, "\n"));

    vector<TestSetDescription>::iterator it = torontoTestSet.begin() + _datasetIndex;
    // Create TestSet instance
    TorontoTestSet testSet((*it).getName(), (*it).getDescription(), _testBenchmarksDir);
    // Load dataset
    TorontoTestSet* ptr = &testSet;
    ptr->load();
    // Fixed length timetables
    int numPeriods = (*it).getPeriods();
    // Set the # periods in the timetable problem data
    ptr->getTimetableProblemData()->setNumPeriods(numPeriods);
    // Print timetable problem data
//    cout << *(ptr->getTimetableProblemData().get()) << endl;
    // Print testset info
//    cout << testSet << endl;

    // Generate exam move statistics
//    generateExamMoveStatistics(_outputDir, testSet);

    // Run test set
    runCellularEA(_outputDir, testSet);


    // Parameters: string const& _torontoDatasetPath, string const& _outputDir, string const& _torontoDatasetName
//    runExamEvaluator(testSet.getRootDirectory()+"/", _outputDir, testSet.getName());



//   runStandardEA(outFile, testSet);
}


// Generate exam move statistics
void generateExamMoveStatistics(string const& _outputDir, TestSet const& _testSet) {
    ///////////////////////////////////////////////////
    //
    // Generate exam move statistics
    //
    ///////////////////////////////////////////////////
    cout << "///////////////////////////////////////////////////" << endl;
    cout << "//" << endl;
    cout << "// Generate exam move statistics" << endl;
    cout << "//" << endl;
    cout << "///////////////////////////////////////////////////" << endl;
    // Number of thresholds
    int numBins = 10;
    // TA cooling schedule
    //    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.99, 3, 2e-5); // Sch #0
    //      moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.5, 0.001, 5, 2e-5); // Sch #11
    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.001, 5, 2e-5); // Sch #1, 40 seg
    //      moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.0001, 5, 2e-5); // Sch #2, 6m30s
    //    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.00001, 5, 2e-5); // Uta 3.13, Sch #3
    //    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.5, 0.00001, 5, 2e-5); // Uta 3..., Sch #4
//        moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.000001, 5, 2e-5); // Uta 3.03, Sch #5
    //    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.0000001, 5, 2e-5); // Sch #6

    ExamMoveStatistics examMoveStats(_testSet, _outputDir, numBins, coolSchedule);
    examMoveStats.run();
    // Creating the output file containing threshold information
    stringstream sstream;
    sstream << _outputDir << "/ExamMoveStatistics_" << _testSet.getName() << "_cool_"
            << coolSchedule.initT << "_" << coolSchedule.alpha << "_"
            << coolSchedule.span << "_" << coolSchedule.finalT << "_Thresholds.txt";
    string filename;
    sstream >> filename;
    ofstream thresholdFile(filename);
    // Export to file
    thresholdFile << examMoveStats;

    // Creating the output file containing information of move counts in each threshold per exam
    stringstream sstream2;
    sstream2 << _outputDir << "/ExamMoveStatistics_" << _testSet.getName() << "_cool_"
            << coolSchedule.initT << "_" << coolSchedule.alpha << "_"
            << coolSchedule.span << "_" << coolSchedule.finalT << "_MoveCountsPerExam.txt";
    string filename2;
    sstream2 >> filename2;
    ofstream moveCountsPerExamFile(filename2);
    // Export to file
    examMoveStats.printExamMoveCountInfo(moveCountsPerExamFile);
    //    moveCountsPerExamFile << examMoveStats;

    cout << "END" << endl;
}



void runCellularEA(const string &_outputDir, TestSet const& _testSet) {
    //
    // cEA parameters
    //
    ////// Pop size = 16  ////////
    // Rect
    const int NLINES = 2;
    const int NCOLS = 8;
    // Matrix
//    const int NLINES = 4;
//    const int NCOLS = 4;
//    // Ring
//    const int NLINES = 1;
//    const int NCOLS = 16;
    //////////////////////////////

    ////// Pop size = 36  ////////
    // Rect
//    const int NLINES = 3;
//    const int NCOLS = 12;
    // Matrix
//    const int NLINES = 6;
//    const int NCOLS = 6;
    // Ring
//    const int NLINES = 1;
//    const int NCOLS = 36;
    //////////////////////////////

    ////// Pop size = 64  ////////
    // Rect
//    const int NLINES = 2;
//    const int NCOLS = 32;
    // Matrix
//    const int NLINES = 8;
//    const int NCOLS = 8;
    // Ring
//    const int NLINES = 1;
//    const int NCOLS = 64;
    //////////////////////////////

    ////// Pop size = 100  ////////
//    // Rect
//    const int NLINES = 5;
//    const int NCOLS = 20;
//    // Matrix
//    const int NLINES = 10;
//    const int NCOLS = 10;
//    // Ring
//    const int NLINES = 1;
//    const int NCOLS = 100;
    //////////////////////////////

    ////// Pop size = 225  ////////
    // Rect
//    const int NLINES = 9;
//    const int NCOLS = 25;
//    // Matrix
//    const int NLINES = 15;
//    const int NCOLS = 15;

    // Ring
//    const int NLINES = 1;
//    const int NCOLS = 225;
    //////////////////////////////

//    const int NLINES = 10;
//    const int NCOLS = 100;

//    const int NLINES = 50;
//    const int NCOLS = 200;


    const int POP_SIZE = NLINES*NCOLS;  // Population size
    const int L = 5000000; // Number of generations

    // Crossover probability
//        const double cp = 0.7;
//    const double cp = 0.6;
//    const double cp = 0;
    const double cp = 0.4; // TEST
//    const double cp = 0.15;

    // Mutation probability
//    const double mp = 1;
//        const double mp = 0.5;
    const double mp = 0.1;
//    const double mp = 0; // TEST

    // Improve probability
    const double ip = 0.1; // TEST
//    const double ip = 0;
//    const double ip = 0.5;
//      const double ip = 1;
    // TA parameters
//      moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.0001, 5, 2e-5); // CoolSch #1 < 30 min

//    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.99, 3, 2e-5);
//    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.1, 5, 2e-5);
//    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.01, 5, 2e-5);
//    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.001, 5, 2e-5);//  Uta 3.68 1h
//      moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.0001, 5, 2e-5);//
//    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.00001, 5, 2e-5); // CoolSch #2 Uta 3.13,
    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.000001, 5, 2e-5); // CoolSch #3 Uta 3.03   // TEST

    // Creating the output file in the specified output directory
    stringstream sstream;
    sstream << _outputDir << _testSet.getName() << "_NLINES_" << NLINES << "_NCOLS_" << NCOLS
            << "_cp_" << cp << "_mp_" << mp << "_ip_" << ip
            << "_cool_" << coolSchedule.initT << "_" << coolSchedule.alpha << "_"
            << coolSchedule.span << "_" << coolSchedule.finalT << ".txt";

    string filename;
    sstream >> filename;
    ofstream outFile(filename);

    cout << "Start Date/Time = " << currentDateTime() << endl;
    // Write Start time and algorithm parameters to file
    outFile << "Start Date/Time = " << currentDateTime() << endl;
    // Print Test set info
    cout << _testSet << endl;
    // Write testset info to file
    outFile << _testSet << endl;
    // getSANumberEvaluations(double tmax, double r, double k, double tmin)
    long numEvalsTA = getSANumberEvaluations(coolSchedule.initT, coolSchedule.alpha,
                                             coolSchedule.span, coolSchedule.finalT);
    /////////////////////////// Writing the cGA parameters ////////////////////////////////////////////////////////
    cout << "cGA parameters:" << endl;
    cout << "NLINES = " << NLINES << ", NCOLS = " << NCOLS << endl;
    cout << "cp = " << cp << ", mp = " << mp << ", ip = " << ip << endl;
    cout << "TA parameters:" << endl;
    cout << "cooling schedule: " << coolSchedule.initT << ", " << coolSchedule.alpha << ", "
            << coolSchedule.span << ", " << coolSchedule.finalT << endl;
    cout << "# evals per TA local search: " << numEvalsTA << endl;
    ///
    outFile << "cGA parameters:" << endl;
    outFile << "NLINES = " << NLINES << ", NCOLS = " << NCOLS << endl;
    outFile << "cp = " << cp << ", mp = " << mp << ", ip = " << ip << endl;
    outFile << "TA parameters:" << endl;
    outFile << "cooling schedule: " << coolSchedule.initT << ", " << coolSchedule.alpha << ", "
            << coolSchedule.span << ", " << coolSchedule.finalT << endl;
    outFile << "# evals per TA local search: " << numEvalsTA << endl;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Solution initializer
    ETTPInit<eoChromosome> init(*_testSet.getTimetableProblemData().get());
    // Generate initial population
//    eoPop<eoChromosome> pop(POP_SIZE, init);
      // We can't work with eoPop of shared_ptr because shared_ptr is not an EO
//    boost::shared_ptr<eoPop<boost::shared_ptr<eoChromosome> > > pop(new eoPop<boost::shared_ptr<eoChromosome> >());
    // Solution: Work with vector<shared_ptr<EOT> > directly
    boost::shared_ptr<vector<boost::shared_ptr<eoChromosome> > > pop(new vector<boost::shared_ptr<eoChromosome> >());

    auto & solutionPop = *pop.get();
    for (int i = 0; i < POP_SIZE; ++i) {
        // Create solution object and insert it in the vector
        solutionPop.push_back(boost::shared_ptr<eoChromosome>(new eoChromosome()));
        // Initialize chromosome
        init((*solutionPop.back().get()));
    }

    // # evaluations counter
    eoNumberEvalsCounter numEvalCounter;
    // Objective function evaluation
//    eoETTPEval<eoChromosome> eval;
    // Objective function evaluation
    eoETTPEvalWithStatistics<eoChromosome> eval(numEvalCounter);
    // Evaluate population
//    for (int i = 0; i < pop.size(); ++i) {
//        eval(pop[i]);
//    }
    for (int i = 0; i < solutionPop.size(); ++i) {
        eval(*solutionPop[i].get());
    }

    /////////////////////////////////////////////////////////////////////////////////
    // Print population information to output and
    // save population information into file
    std::cout << std::endl << "Initial population" << std::endl;
    outFile << std::endl << "Initial population" << std::endl;
    int k = 0;
    for (int i = 0; i < NLINES; ++i) {
        for (int j = 0; j < NCOLS; ++j, ++k) {
            std::cout << (*solutionPop[k].get()).fitness() << "\t";
            outFile << (*solutionPop[k].get()).fitness() << "\t";
        }
        std::cout << std::endl;
        outFile << std::endl;
    }
    /////////////////////////////////////////////////////////////////////////////////
    //
    // Build CellularGA
    //
    // Terminate after concluding L time loops or 'Ctrl+C' signal is received
//    eoGenerationContinue<eoChromosome> terminator(L);
//    eoCheckPoint<eoChromosome> checkpoint(terminator);
    // The eoGenerationContinuePopVector object, instead of using an eoPop to represent the population,
    // uses a vector. A vector is used in order to swap offspring and population efficiently using pointers
    eoGenerationContinuePopVector<eoChromosome> terminator(L);

    // Declare 1-selectors
    //
    // Binary deterministic tournament selector used in neighbour selection
//    eoDetTournamentSelect<eoChromosome> detSelectNeighbour;
    // Work with pointers for efficiency
    eoDetTournamentSelectSharedPtr<eoChromosome> detSelectNeighbourPtr;

    // Crossover and mutation
    Mutation<eoChromosome> mutation;
    Crossover<eoChromosome> crossover;
    eoSelectBestOne<eoChromosome> selectBestOne;

    boost::shared_ptr<eoCellularEA<eoChromosome> > cGA;
    // Build the corresponding type of cGA object depending on the layout (Ring or Matrix)
    if (NLINES == 1) { // Ring cGA
//    eoCellularEARing<eoChromosome> cEA(checkpoint, eval, detSelectNeighbour,
//                      crossover, mutation,
//                     selectBestOne,
//                     selectBestOne
//                    );
    }
    else {
        cGA = boost::make_shared<eoCellularEAMatrix<eoChromosome> >(
                    outFile, NLINES, NCOLS,
                    cp, mp, ip, coolSchedule,
                    terminator,
//                    checkpoint,
                    eval,
//                    detSelectNeighbour,
                    detSelectNeighbourPtr, // Work with pointers for efficiency
                    crossover, mutation,
                    selectBestOne, // To choose one from the both children
                    selectBestOne,  // Which to keep between the new child and the old individual?
                    numEvalCounter
        );

    }

    // Run the algorithm
    (*cGA.get())(pop);


    //
    // Create solution file and run Rong Qu's exam evaluator
    //
    string solutionFilename = _testSet.getName() + ".sol";
    ofstream solutionFile(_outputDir + solutionFilename);
    // Write best solution to file
    (*cGA.get()).getBestSolution()->printToFile(solutionFile);
    // Close solution file
    solutionFile.close();
    // Parameters: string const& _torontoDatasetPath, string const& _outputDir, string const& _torontoDatasetName
    runExamEvaluator(_testSet.getRootDirectory()+"/", _outputDir, _testSet.getName());


    // Write end Date/Time
    cout << "End Date/Time = " << currentDateTime() << endl;
    // Write to file
    outFile << "End Date/Time = " << currentDateTime() << endl;
    // Close output file
    outFile.close();
}


