
#include "ExamMoveStatistics.h"


#include "testset/TestSetDescription.h"
#include "utils/CurrentDateTime.h"
#include "neighbourhood/ETTPneighborEval.h"
//#include "neighbourhood/ETTPneighbor.h"
#include "neighbourhood/statistics/ETTPneighborWithStatistics.h"
#include "neighbourhood/statistics/ETTPneighborEvalWithStatistics.h"

//#include "neighbourhood/ETTPneighborhood.h"
#include "neighbourhood/statistics/ETTPneighborhoodWithStatistics.h"
#include "algorithms/mo/statistics/moTAWithStatistics.h"
#include "kempeChain/statistics/ETTPKempeChainHeuristicWithStatistics.h"
#include "init/ETTPInit.h"

#include <boost/range/algorithm.hpp>
#include <vector>

#include "algorithms/mo/moTA.h"
#include <vector>


// Temperature actualization
extern double Temp(double t, double Tmax, double R);
// Determine # evaluations
extern int getSANumberEvaluations(double tmax, double r, double k, double tmin);


boost::shared_ptr<string> ExamMoveStatistics::generateFilename() {
    // Creating the output filename
    stringstream sstream;
    sstream << outputDir << "/ExamMoveStatistics_" << testSet.getName() << "_cool_"
            << coolSchedule.initT << "_" << coolSchedule.alpha << "_"
            << coolSchedule.span << "_" << coolSchedule.finalT << ".txt";
    boost::shared_ptr<string> filename(new string());
    sstream >> *filename.get();
    cout << *filename.get() << endl;
    return filename;
}


// Ctor
ExamMoveStatistics::ExamMoveStatistics(TestSet const& _testSet, string const& _outputDir,
                                       int _numBins,
                                       moSimpleCoolingSchedule<eoChromosome> &_coolSchedule)
    : testSet(_testSet),                                  // Test set
      outputDir(_outputDir),                              // Output directory
      examInfoVector(testSet.getTimetableProblemData()->getNumExams()), // ExamInfo vector
      numBins(_numBins),
      coolSchedule(_coolSchedule),                        // Cooling schedule
      thresholdArraySize(numBins+1),
      thresholdArray(new double[thresholdArraySize]()),   // Threshold array has # thresholds equal to # of bins + 1
      outFilename(*generateFilename().get()),
      outFile(outFilename)
{
    // Initialize examInfoVector
    for (int examIndex = 0; examIndex < testSet.getTimetableProblemData()->getNumExams(); ++examIndex) {
        examInfoVector[examIndex] =
                boost::make_shared<ExamInfo>(examIndex,          // int _examIndex. Exams indexes start at 0
                                             thresholdArraySize, // int _thresholdArraySize
                                             thresholdArray);    // boost::shared_array<double> _thresholdArray
    }
}


eoChromosome& ExamMoveStatistics::getInitialSolution() { return initialSolution; }

eoChromosome& ExamMoveStatistics::getOptimizedSolution() { return optimizedSolution; }


// Generate initial and optimized solutions
void ExamMoveStatistics::generateInitialSolution() {
    // Solution initializer
    ETTPInit<eoChromosome> init(*testSet.getTimetableProblemData());
    // Generate initial solution
    init(initialSolution);
    // Evaluate solution
    eval(initialSolution);


//    cout << "In method [ExamMoveStatistics::generateInitialSolution()]" << endl;
//    cout << "initialSolution.fitness() = " << initialSolution.fitness() <<
//            ", isFeasible = " << initialSolution.isFeasible() << endl;

//    this->outFile << initialSolution << endl;

//    cin.get();

//    // Instantiate optimized solution as a copy of the initial solution
//    optimizedSolution = initialSolution;
///// IS NECESSARY?
//    // Evaluate solution
//    eval(optimizedSolution);

}



// Determine exams color degree necessary for sorting ExamInfo array by color degree
void ExamMoveStatistics::determineExamsColorDegree() {
    // Get exam graph
    AdjacencyList const& graph = getInitialSolution().getExamGraph();

    AdjacencyList::vertex_iterator vertexIt, vertexEnd;
    AdjacencyList::adjacency_iterator neighbourIt, neighbourEnd;
    tie(vertexIt, vertexEnd) = vertices(graph);
    int count = 0, vertexDegree;

    for (; vertexIt != vertexEnd; ++vertexIt)
    {
//        cout << *vertexIt << " is connected with ";
//        tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, g);
//        for (; neighbourIt != neighbourEnd; ++neighbourIt)
//        cout << *neighbourIt << " ";
//        cout << "\n";
        vertexDegree = degree(*vertexIt, graph);
//        cout << "Degree of vertex " << *vertexIt << ": " << vertexDegree << endl;
        // Set exam degree
        examInfoVector[*vertexIt]->setExamColorDegree(vertexDegree);
    }
//    cin.get();
}



// Generate thresholds for the specified cooling schedule
void ExamMoveStatistics::generateThresholds() {
    // SA
    // moSimpleCoolingSchedule<eoChromosome> cool2(0.1, 0.00001, 5, 0.0000001);
//    double tmax = 0.1, r = 0.00001, k = 5, tmin = 0.0000001;
    //    cout << getSANumberEvaluations(tmax, r, k, tmin) << endl;
    // number of evaluations: 6 907 760
    /////////////////////////////////////////////////////////
    // TA cooling schedule
//    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.99, 3, 2e-5); // Sch #1
//        moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.001, 5, 2e-5); Sch #2
//    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.00001, 5, 2e-5); // Uta 3.13, Sch #3
//    moSimpleCoolingSchedule<eoChromosome> coolSchedule(0.1, 0.000001, 5, 2e-5); // Uta 3.03, Sch #4

    //    const long maxNumEval = 6907760; // SA
    long maxNumEval;

//    double tmax = 0.1, r = 0.00001, k = 5, tmin = 2e-5; // Sch #3, #evaluations: 4258600
//    double tmax = 0.1, r = 0.000001, k = 5, tmin = 2e-5; // Sch #4, #evaluations: 42585970
    double tmax = coolSchedule.initT, r = coolSchedule.alpha,
           k = coolSchedule.span, tmin = coolSchedule.finalT;
    cout << "tmax = " << tmax << ", r = " << r << ", k = " << k << ", tmin = " << tmin << endl;
    cout << (maxNumEval = getSANumberEvaluations(tmax, r, k, tmin)) << endl;

    int numEvalPerThreshold = maxNumEval / numBins;
    long lastBin = maxNumEval % numBins;
    long numEval;
    long totalNumEval = 0;

    cout << "maxNumEval = " << maxNumEval << ", numBins = " << numBins
         << ", numEvalPerThreshold = " << numEvalPerThreshold << ", lastBin = " << lastBin << endl;


    // Add lower bound threshold
    thresholdArray[0] = tmax;
    cout << "i = " << 0 << ", threshold = " << thresholdArray[0] << endl;

    double t = 0, temp = tmax;
    totalNumEval = 0;
    bool continueCycle = true;

    for (int i = 1; i < thresholdArraySize; ++i) {
        long numberEvaluations = 0;
        for (;;) {
            for (int j = 1; j <= k; ++j) {
                ++numberEvaluations;
                if (i == thresholdArraySize-1) { // Last bin
                    if (numberEvaluations >= numEvalPerThreshold + lastBin) {
                        continueCycle = false;
                        break;
                    }
                }
                else {
                    if (numberEvaluations >= numEvalPerThreshold) {
                        continueCycle = false;
                        break;
                    }
                }
            }
            if (continueCycle == false)
                break;

            // Actualize temperature
            ++t;
            temp = Temp(t, tmax, r);
        }
        totalNumEval += numberEvaluations;
        cout << "i = " << i << ", numberEvaluations = " << numberEvaluations << ", threshold = " << temp
             << ", totalNumEval = " << totalNumEval << endl;
        // Add threshold to array
        thresholdArray[i] = temp;
        continueCycle = true;
    }
}




//
// Compute exam move statistics and color map. The color map has on the yy axis
// the exams indexes sorted by conflict degree and on the xx axis the
// TA thresholds bins determined in order to have the same number of evaluations
// on each bin.
//
void ExamMoveStatistics::run() {
    // Generate initial solution
    generateInitialSolution();
    // Determine exams color degree necessary for sorting ExamInfo array by color degree
    determineExamsColorDegree();
    // Determine thresholds based on the max number of evaluations
    generateThresholds();

    //
    // Local search used: Threshold Accepting algorithm
    //

    //
    // moTA with statistics
    //
    // moTA parameters
    boost::shared_ptr<ETTPKempeChainHeuristicWithStatistics<eoChromosome> > kempeChainHeuristic(
                new ETTPKempeChainHeuristicWithStatistics<eoChromosome>());
    eoETTPEval<eoChromosome> fullEval; // eoEvalFunc used to evaluate the solutions
//    ETTPneighborhood<eoChromosome> neighborhood(kempeChainHeuristic);
    /// CHANGED: RECEIVE PTR
    ETTPNeighborhoodWithStatistics<eoChromosome>neighborhood(kempeChainHeuristic.get());
//    ETTPneighborEval<eoChromosome> neighEval;
    ETTPneighborEvalWithStatistics<eoChromosome> neighEval;
//    ETTPneighborEvalWithStatistics<eoChromosome> neighEval; // Count # evaluations

//    moTAWithStatistics<ETTPneighbor<eoChromosome> > ta(*this, neighborhood, fullEval, neighEval, coolSchedule);
    moTAWithStatistics<ETTPneighborWithStatistics<eoChromosome> > ta(*this, neighborhood, fullEval, neighEval, coolSchedule);

    /////// Write to output File ///////////////////////////////////////////
    cout << "Start Date/Time = " << currentDateTime() << endl;
    // Write Start time and algorithm parameters to file
    outFile << "Start Date/Time = " << currentDateTime() << endl;
    outFile << "TA parameters:" << endl;
    outFile << "cooling schedule: " << coolSchedule.initT << ", " << coolSchedule.alpha << ", "
            << coolSchedule.span << ", " << coolSchedule.finalT << endl;

    outFile << testSet << endl;
    outFile << "Exam Move Statistics" << endl;


    cout << "Before TA - initialSolution.fitness() = " << initialSolution.fitness() << endl;

    // Apply TA to the solution
//    ta(optimizedSolution);
    ta(initialSolution);

    // Validate solution
//    initialSolution.validate();

    cout << "After TA - initialSolution.fitness() = " << initialSolution.fitness() << endl;


/// Test
//    cout << "Before move - initialSolution.fitness() = " << initialSolution.fitness() << endl;

//    ETTPKempeChainHeuristic<eoChromosome> _kempeChainHeuristic;
//    _kempeChainHeuristic.build(initialSolution);
//    _kempeChainHeuristic(initialSolution);
//    eval(initialSolution);
//    cout << "After move - initialSolution.fitness() = " << initialSolution.fitness() << endl;
///

    // Write best solution to file
    outFile << "==============================================================" << endl;
    outFile << "Date/Time = " << currentDateTime() << endl;
//    outFile << optimizedSolution << endl;
    outFile << initialSolution << endl;
    outFile << "==============================================================" << endl;

    cout << "End Date/Time = " << currentDateTime() << endl;
    // Write to file
    outFile << "End Date/Time = " << currentDateTime() << endl;


    // Sort ExamInfo array in descending order by exam conflict degree
    sort();
    //////////////////////////////////////////////////////////////////////////////////

/// Test
//    initialSolution.countConflictsAllExams();
///
}



ostream& operator<<(ostream& _os, const ExamMoveStatistics& _examMoveStats) {
    // Output threshold array
    auto* ptr = _examMoveStats.thresholdArray.get();
    for (double thresh : boost::make_iterator_range(ptr, ptr + _examMoveStats.thresholdArraySize))
        _os << thresh << endl;

    return _os;
}



class Criterium
{
public:
    bool operator()(const ExamInfo& left,
                    const ExamInfo& right) const {

        return left.getExamColorDegree() > right.getExamColorDegree();
    }
};



// Sort ExamInfo array in descending order by exam conflict degree
void ExamMoveStatistics::sort() {
    // Create an auxiliary vector from the range (begin, end[ for sorting
    vector<boost::shared_ptr<ExamInfo> > examInfoVec(examInfoVector.begin(), examInfoVector.end());
    // boost::sort
    // For versions of the sort function without a predicate, ascending order is defined by
    // operator<() such that for all adjacent elements [x,y], y < x == false.
    // For versions of the sort function with a predicate, ascending order is defined by pred
    // such that for all adjacent elements [x,y], pred(y, x) == false.
    // Complexity: O(N log(N))
//    boost::sort(examInfoVec, greater<boost::shared_ptr<ExamInfo> >()); // Sort in *descending* order by exam conflict degree
    // We must define ExamInfo::operator>
    Criterium criterium;
    boost::sort(examInfoVec, [&criterium] (const boost::shared_ptr<ExamInfo> &l, const boost::shared_ptr<ExamInfo> &r)
        {
            return criterium(*l.get(), *r.get());
        }
    );

    // Print
//    copy(examInfoVec.begin(), examInfoVec.end(), ostream_iterator<boost::shared_ptr<ExamInfo>>(cout, "\n"));

    for_each(examInfoVec.begin(), examInfoVec.end(),
             [this](const boost::shared_ptr<ExamInfo>& p)
             {
                this->outFile << *p << endl;
//                cout << *p << endl;
             });

    outFile.close();
}



// Get index in the threshold array given a threshold
int ExamMoveStatistics::getThresholdIndex(double _threshold) const {
    // Threshold array is sorted in descending order. Example: [0.1, 0.01, 0.001, 0.0001, ..., 2e-5]
    if (_threshold > thresholdArray[0]) // If threshold is greater than the first threshold, print an error
        cerr << "Error: threshold > lower threshold: " << _threshold << " > " << thresholdArray[0] << endl;

    // Threshold index
    int threshIndex;
    for (threshIndex = 0; threshIndex < thresholdArraySize-1; ++threshIndex) {
        double upperThresh = thresholdArray[threshIndex+1]; // Get upper threshold
//        cout << "_threshold = " << _threshold << ", upperThresh = " << upperThresh << endl;
        if (_threshold > upperThresh)  // If threshold value is greater than the upper threshold stop
            break;
    }
    return threshIndex;
}



// Update exame move
void ExamMoveStatistics::updateExamMove(int _exam, double _threshold) {
    // Get threshold index
    int threshIndex = getThresholdIndex(_threshold);
    // Update exam move count
    examInfoVector[_exam]->updateMove(threshIndex);
}



void ExamMoveStatistics::printExamMoveCountInfo(ofstream &_moveCountsPerExamFile) {

    //
    // Sort examInfo vector
    //

    // Create an auxiliary vector from the range (begin, end[ for sorting
    vector<boost::shared_ptr<ExamInfo> > examInfoVec(examInfoVector.begin(), examInfoVector.end());
    // boost::sort
    // For versions of the sort function without a predicate, ascending order is defined by
    // operator<() such that for all adjacent elements [x,y], y < x == false.
    // For versions of the sort function with a predicate, ascending order is defined by pred
    // such that for all adjacent elements [x,y], pred(y, x) == false.
    // Complexity: O(N log(N))
//    boost::sort(examInfoVec, greater<boost::shared_ptr<ExamInfo> >()); // Sort in *descending* order by exam conflict degree
    // We must define ExamInfo::operator>
    Criterium criterium;
    boost::sort(examInfoVec, [&criterium] (const boost::shared_ptr<ExamInfo> &l, const boost::shared_ptr<ExamInfo> &r)
        {
            return criterium(*l.get(), *r.get());
        }
    );
    ////////////////////////////////////////////////////////////////////////////////////

    // Print information of move counts in each threshold per exam in the format:
    // % <treshold 1 value>  // Threshold
    // <Exam 1 sequential index>  <move count in this threshold>
    // <Exam 2 sequential index>  <move count in this threshold>
    // ...
    //
    // % <treshold 2 value>  // Threshold
    // <Exam 1 sequential index>  <move count in this threshold>
    // <Exam 2 sequential index>  <move count in this threshold>
    // ...
    // This information is used to generate section graphs in each threshold

//    auto* ptr = thresholdArray.get();
//    int threshIndex = 0;
//    for (double thresh : boost::make_iterator_range(ptr, ptr + thresholdArraySize)) {
//        // Print threshold value
//        _moveCountsPerExamFile << "% " << thresh << " // Threshold" << endl;
//        // Print exams move count for the current threshold
//        for (int exam = 0; exam < testSet.getNumExams(); ++exam) {
//            // Print <Exam i sequential index>  <move count in this threshold>
///// TODO - Exams in Toronto start at 1
/////
//            _moveCountsPerExamFile << (exam+1) // Exams in Toronto start at 1
//                                   << '\t' << examInfoVector[exam]->getExamMoveCount(threshIndex) << endl;
//        }
//        _moveCountsPerExamFile << endl; // Write a new line
//        // Increment threshold index
//        ++threshIndex;
//    }


    //
    // Matlab surf
    //
    // surf(X,Y,Z) uses Z for the color data and surface height.
    // X and Y are vectors or matrices defining the x and y components of a surface.
    // If X and Y are vectors, length(X) = n and length(Y) = m, where [m,n] = size(Z).
    //

    auto* ptr = thresholdArray.get();
    // Print X vector - Thresholds
    _moveCountsPerExamFile << "% Thresholds" << endl
                           << "X = [" << endl;
    for (double thresh : boost::make_iterator_range(ptr, ptr + thresholdArraySize)) {
        // Print threshold value
        _moveCountsPerExamFile << thresh << endl;
    }
    _moveCountsPerExamFile << "]" << endl;


    // Print Y vector - Exam indexes
    _moveCountsPerExamFile << "% Exam indexes" << endl
                           << "Y = [" << endl;
    for (int exam = 0; exam < testSet.getTimetableProblemData()->getNumExams(); ++exam) {
        // Print exam index
        _moveCountsPerExamFile << exam << endl;
    }
    _moveCountsPerExamFile << "]" << endl;


    // Print Z matrix - Exam move count
    _moveCountsPerExamFile << "% Exam move count" << endl
                           << "Z = [" << endl;

    // Exams start at index 0
    for (int exam = 0; exam < testSet.getTimetableProblemData()->getNumExams(); ++exam) {
        int threshIndex = 0;
        for (; threshIndex < thresholdArraySize-1; ++threshIndex) {
        // Print exams move count for the current threshold and exam index
            _moveCountsPerExamFile << examInfoVec[exam]->getExamMoveCount(threshIndex) << ", ";
        }
        _moveCountsPerExamFile << examInfoVec[exam]->getExamMoveCount(threshIndex)
                               << "; " << endl; // Write last Z column and add a new line
    }
    _moveCountsPerExamFile << "]" << endl;

}














