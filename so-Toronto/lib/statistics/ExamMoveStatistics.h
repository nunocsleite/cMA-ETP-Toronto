#ifndef EXAMMOVESTATISTICS_H
#define EXAMMOVESTATISTICS_H

#include <eo>

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "chromosome/eoChromosome.h"
#include "algorithms/mo/moSimpleCoolingSchedule.h"
#include "statistics/ExamInfo.h"
#include "testset/TestSet.h"
#include "eval/eoETTPEval.h"





class ExamMoveStatistics {

public:
    /**
     * @brief ExamMoveStatistics
     * @param _numThresholds
     * @param _coolSchedule
     * @return
     */
    ExamMoveStatistics(TestSet const& _testSet, std::string const& _outputDir,
                       int _numBins, moSimpleCoolingSchedule<eoChromosome> & _coolSchedule);

    //
    // Public interface
    //
    inline eoChromosome& getInitialSolution();
    inline eoChromosome& getOptimizedSolution();

    void run();
    // Update exame move
    void updateExamMove(int _exam, double _threshold);


    friend std::ostream& operator<<(std::ostream& _os, const ExamMoveStatistics& _examMoveStats);

    void printExamMoveCountInfo(std::ofstream &_moveCountsPerExamFile);

    // Get index in the threshold array given a threshold
    int getThresholdIndex(double _threshold) const;

private:
    boost::shared_ptr<std::string> generateFilename();
    // Determine exams color degree necessary for sorting ExamInfo array by color degree
    void determineExamsColorDegree();
    // Generate initial and optimized solutions
    void generateInitialSolution();
    // Generate thresholds for the specified cooling schedule
    void generateThresholds();
    // Sort ExamInfo array in descending order by exam conflict degree
    void sort();
//    // Get index in the threshold array given a threshold
//    int getThresholdIndex(double _threshold) const;


    //
    // Fields
    //
    eoETTPEval<eoChromosome> eval;              // Objective function evaluation
    eoChromosome initialSolution;               // Initial solution
    eoChromosome optimizedSolution;             // Optimized solution
    TestSet const& testSet;                     // Test set
    std::string const& outputDir;                    // Output directory


/// TODO
///
///
///

public:
    //< ExamInfo array. Each examInfo object contains information about exam move counts for each threshold
    std::vector<boost::shared_ptr<ExamInfo> > examInfoVector;

private:
    //< Colling schedule used to generate the statistics
    moSimpleCoolingSchedule<eoChromosome> & coolSchedule;
    int numBins;                                // Threshold array has # thresholds equal to # of bins + 1
    int thresholdArraySize;
    boost::shared_array<double> thresholdArray; // Threshold array sorted in descending order
//    const double EPSILON_ERROR = 1e-8;        // Error
    std::string outFilename;                    // Output filename
    std::ofstream outFile;                      // Output file
};


#endif // EXAMMOVESTATISTICS_H
