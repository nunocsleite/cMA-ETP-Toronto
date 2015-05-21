#ifndef MUTATION_H
#define MUTATION_H

#include "chromosome/eoChromosome.h"
#include <eoOp.h>
#include <string>

#include "neighbourhood/ETTPneighborhood.h"
#include "neighbourhood/ETTPneighborEval.h"

#include "kempeChain/ETTPKempeChainHeuristic.h"


template <typename EOT>
class Mutation : public eoMonOp<EOT>
{

public:

    /**
    * the class name (used to display statistics)
    */
    std::string className() const;

    /**
    * eoMon mutation - _chromosome is the chromosome to mutate.
    * @param _chromosome the chromosome
    */
    bool operator()(EOT& _chromosome);
};



template <typename EOT>
std::string Mutation<EOT>::className() const
{
    return "ETTP Mutation";
}


/**
 * ORIGINAL IMPLEMENTATION
 *
 */
//template <typename EOT>
//bool Mutation<EOT>::operator()(EOT& _chrom)
//{
////    cout << "Mutation" << endl;

//    bool chromosomeIsModified = true;
//    TimetableContainer & timetableCont = _chrom.getTimetableContainer();
//    //
//    // Swap two periods
//    //
//    // Generate a random index in frogs
//    int randPeriod1 = rng.random(_chrom.getNumPeriods());
//    int randPeriod2 = rng.random(_chrom.getNumPeriods());
//    // Get a copy of period 1
//    auto copyPeriod1 = timetableCont.getPeriodExams(randPeriod1);
//    // Period # exams
//    int sizePeriod1 = timetableCont.getPeriodSize(randPeriod1);
//    // Insert period 2 exams into period 1
//    timetableCont.replacePeriod(randPeriod1, randPeriod2);
//    // Insert copyPeriod1 exams into period 2
//    timetableCont.replacePeriod(randPeriod2, copyPeriod1, sizePeriod1);

////    _chrom.computeProximityCosts();

///// TODO - INCREMENTAL EVALUATION
/////
/////
/////


//    // return 'true' if at least one genotype has been modified
//    return chromosomeIsModified;
//}




template <typename EOT>
bool Mutation<EOT>::operator()(EOT& _chrom)
{
//    cout << "Mutation" << endl;

    bool chromosomeIsModified = true;
/*    TimetableContainer & timetable = _chrom.getTimetableContainer();
    long examProximityConflictsOfRemovedExams = 0;
    long examProximityConflictsOfInsertedExams = 0;
    //
    // Swap two periods
    //
    // Generate a random index in frogs
    int randPeriod1 = rng.random(_chrom.getNumPeriods());
    int randPeriod2 = rng.random(_chrom.getNumPeriods());
    // # exams
    const int numExams = _chrom.getNumExams();
    // Period1 exams to move
    std::vector<int> period1ExamsToMove;
    // Period2 exams to move
    std::vector<int> period2ExamsToMove;

    // For each exam of period randPeriod2 do
    for (int ei = 0; ei < numExams; ++ei) {
        ///
        /// Move exams from period2 to period1
        ///
        // If exam ei is scheduled in period randPeriod2
        if (timetable.isExamScheduled(ei, randPeriod2)) {
            // For incremental evaluation, we need to sum proximity conflicts of this exam to be removed
            long sourceExamProximityConflictsRemoval = _chrom.getExamProximityConflicts(ei, randPeriod2);
            // Sum proximity conflicts of this exam
            examProximityConflictsOfRemovedExams += sourceExamProximityConflictsRemoval;
            // Remove original exam in timetable in period fromPeriod
            timetable.removeExam(ei, randPeriod2);
            // Record exam for later insertion
            period2ExamsToMove.push_back(ei);
        }
        // If exam ei is scheduled in period randPeriod1
        else if (timetable.isExamScheduled(ei, randPeriod1)) {
            // For incremental evaluation, we need to sum proximity conflicts of this exam to be removed
            long sourceExamProximityConflictsRemoval = _chrom.getExamProximityConflicts(ei, randPeriod1);
            // Sum proximity conflicts of this exam
            examProximityConflictsOfRemovedExams += sourceExamProximityConflictsRemoval;
            // Remove original exam in timetable in period fromPeriod
            timetable.removeExam(ei, randPeriod1);
            // Record exam for later insertion
            period1ExamsToMove.push_back(ei);
        }
    }
    // Insert period1 exams in period2
    for (int ei : period1ExamsToMove) {
        // Now, insert the exam
        timetable.insertExam(ei, randPeriod2);
        // For incremental evaluation, we need to sum proximity conflicts of this inserted exam
        // Determine the proximity conflicts
        long sourceExamProximityConflictsInsertion = _chrom.getExamProximityConflicts(ei, randPeriod2);
        // Sum proximity conflicts of this exam
        examProximityConflictsOfInsertedExams += sourceExamProximityConflictsInsertion;
    }
    // Insert period2 exams in period1
    for (int ei : period2ExamsToMove) {
        // Now, insert the exam
        timetable.insertExam(ei, randPeriod1);
        // For incremental evaluation, we need to sum proximity conflicts of this inserted exam
        // Determine the proximity conflicts
        long sourceExamProximityConflictsInsertion = _chrom.getExamProximityConflicts(ei, randPeriod1);
        // Sum proximity conflicts of this exam
        examProximityConflictsOfInsertedExams += sourceExamProximityConflictsInsertion;
    }

    // Actualize proximity costs - Incremental evaluation
    long chromExamProximityConflicts = _chrom.getProximityConflicts();
    // Now subtract to the solution proximity conflicts the conflicts of the removed exams
    chromExamProximityConflicts -= examProximityConflictsOfRemovedExams;
    // ... and add the conflicts of the inserted exams
    chromExamProximityConflicts += examProximityConflictsOfInsertedExams;
    // Actualize proximity costs
    _chrom.setExamProximityConflicts(chromExamProximityConflicts);
*/
/// DEBUG
//    _chrom.validate();


    ETTPKempeChainHeuristic<EOT> kempe;
    kempe.build(_chrom);
    kempe.evaluateSolutionMove(_chrom);
    kempe(_chrom);
    _chrom.setExamProximityConflicts(kempe.getNeighborProximityConflicts());
    _chrom.fitness(_chrom.getProximityCost());

    // Return 'true' if at least one genotype has been modified
    return chromosomeIsModified;
}



#endif // MUTATION_H
