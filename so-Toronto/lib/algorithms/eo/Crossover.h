#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "chromosome/eoChromosome.h"
#include <eoOp.h>
#include "kempeChain/ETTPKempeChainHeuristic.h"
#include <utility>
#include <boost/unordered_map.hpp>
#include "neighbourhood/ETTPneighborhood.h"
#include "algorithms/mo/moTA.h"
#include "neighbourhood/ETTPneighborEval.h"
#include "algorithms/mo/moSimpleCoolingSchedule.h"
#include "testset/TestSetDescription.h"
#include <eoGenContinue.h>
#include <utils/eoCheckPoint.h>

// For debugging purposes
//#define _CROSSOVER_DEBUG


template <typename EOT>
class Crossover : public eoQuadOp<EOT>
{

public:

    /**
    * the class name (used to display statistics)
    */
    std::string className() const;

    /**
    * eoQuad crossover - _chromosome1 and _chromosome2 are the (future) offspring, i.e. _copies_ of the parents
    * @param _chromosome1 the first parent
    * @param _chromosome2 the second parent
    */
    bool operator()(EOT& _chromosome1, EOT& _chromosome2);

protected:

    /**
    * generation of an offspring
    * @param _parent1 the first parent
    * @param _parent2 the second parent
    */
    void generateOffspring(EOT &_parent1, EOT &_parent2);

    void insertExams(EOT &_offspring, const EOT &_parent);

    /**
     * @brief eoChromosome::removeExam Remove exam _ei from timetable ignoring period _randOffspringPeriod
     * @param _timetableOffspring
     * @param _ei
     * @param _randOffspringPeriod
     */
    int findExamPeriod(TimetableContainer &_timetable, int _ei, int _randOffspringPeriod) const;

};


template <typename EOT>
std::string Crossover<EOT>::className() const
{
    return "ETTP Crossover";
}


template <typename EOT>
bool Crossover<EOT>::operator()(EOT& _chromosome1, EOT& _chromosome2)
{

    bool oneAtLeastIsModified = true;
    // Computation of the offspring
    EOT chrom1 = _chromosome1; // Original chromosome 1
    EOT chrom2 = _chromosome2; // Original chromosome 2
    generateOffspring(_chromosome1, chrom2); // offspring 1 is the resulting chromosome 1
    generateOffspring(_chromosome2, chrom1); // offspring 2 is the resulting chromosome 2

    return oneAtLeastIsModified;
}


//===
// Abdullah et al., Sabar et. al feasible crossover
//===
template <typename EOT>
void Crossover<EOT>::generateOffspring(EOT &_parent1, EOT &_parent2)
{
    //
    // This crossover operator generates two offspring in the following way:
    // exams chosen from a random period in parent2 are inserted in a
    // random period in period1. Unfeasible exams are nor inserted and all
    // duplicated exams in the other periods in parent1 are deleted in order
    // to maintain feasibility.
    //
    // Instantiate a copy of parent1 to be used in the generation of offspring2
    EOT parent1Copy = _parent1;
    // We change directly _parent1 and _parent2 to be the new offsprings because
    // they're already a copy of the original parents.
    EOT& offspring1 = _parent1;
    EOT& offspring2 = _parent2;
    // Insert exams in offspring1 and compute offspring1 proximity cost incrementally
    insertExams(offspring1, _parent2);
    // Insert exams in offspring2 and compute offspring2 proximity cost incrementally
    insertExams(offspring2, parent1Copy);
}


template <typename EOT>
void Crossover<EOT>::insertExams(EOT &_offspring, const EOT &_parent)
{
    long examProximityConflictsOfRemovedExams = 0;
    long examProximityConflictsOfInsertedExams = 0;

    // Offspring timetable
    TimetableContainer &timetableOffspring = _offspring.getTimetableContainer();
    // Parent timetable
    TimetableContainer const &timetableParent = _parent.getTimetableContainer();
    // # exams
    const int numExams = _offspring.getNumExams();
    // Insert 3 random periods
    int randNumPeriods = 3; // TEST
//    int randNumPeriods = 1;
    for (int i = 1; i <= randNumPeriods; ++i) {
        // Get a random period from parent2
        int randParentPeriod = rng.random(_parent.getNumPeriods());
        // Generate a random index in the offspring
        int randOffspringPeriod = rng.random(_offspring.getNumPeriods());
        // Insert parent2 random period exams (those that are feasible)
        // into the new offspring and remove duplicated exams
        //
        // For each exam of period pi do
        for (int ei = 0; ei < numExams; ++ei) {
            // If exam ei is scheduled in parent
            if (timetableParent.isExamScheduled(ei, randParentPeriod)) {
                // Insert exam in offspring if it's not already insert and is feasible in period
                if (!timetableOffspring.isExamScheduled(ei, randOffspringPeriod) &&
                     _offspring.isExamFeasibleInPeriod(ei, randOffspringPeriod)) {
                    // Find exam ei period
                    int fromPeriod = findExamPeriod(timetableOffspring, ei, randOffspringPeriod);
                    if (fromPeriod == -1) {
                        std::cout << std::endl <<
                                     "In method [Crossover<EOT>::insertExams] - could not found duplicated exam" << std::endl;
                        std::cin.get();
                        std::cout << "Exiting..." << endl;
                        exit(-1);
                    }
                    // For incremental evaluation, we need to sum proximity conflicts of this exam to be removed
                    long sourceExamProximityConflictsRemoval = _offspring.getExamProximityConflicts(ei, fromPeriod);
                    // Sum proximity conflicts of this exam
                    examProximityConflictsOfRemovedExams += sourceExamProximityConflictsRemoval;
                    // Remove original exam in timetable in period fromPeriod
                    timetableOffspring.removeExam(ei, fromPeriod);

                    // Now, insert the exam
                    timetableOffspring.insertExam(ei, randOffspringPeriod);
                    // For incremental evaluation, we need to sum proximity conflicts of this inserted exam
                    // Determine the proximity conflicts
                    long sourceExamProximityConflictsInsertion = _offspring.getExamProximityConflicts(ei, randOffspringPeriod);
                    // Sum proximity conflicts of this exam
                    examProximityConflictsOfInsertedExams += sourceExamProximityConflictsInsertion;
                }
            }
        }
    }

    // Actualize proximity costs - Incremental evaluation
    long offspringExamProximityConflicts = _offspring.getProximityConflicts();
    // Now subtract to the solution proximity conflicts the conflicts of the removed exams
    offspringExamProximityConflicts -= examProximityConflictsOfRemovedExams;
    // ... and add the conflicts of the inserted exams
    offspringExamProximityConflicts += examProximityConflictsOfInsertedExams;
    // Actualize proximity costs
    _offspring.setExamProximityConflicts(offspringExamProximityConflicts);


/// DEBUG
///
    // Validate
//    _offspring.validate();

}


/**
 * @brief eoChromosome::removeExam Remove exam _ei from timetable ignoring period _randOffspringPeriod
 * @param _timetableOffspring
 * @param _ei
 * @param _randOffspringPeriod
 */
template <typename EOT>
int Crossover<EOT>::findExamPeriod(TimetableContainer &_timetable, int _ei, int _randOffspringPeriod) const {
    // For each period (except _randOffspringPeriod) do
    for (int pi = 0; pi < _timetable.getNumPeriods(); ++pi) {
        if (pi != _randOffspringPeriod) {
            // If exam ei is scheduled in period pi
            if (_timetable.isExamScheduled(_ei, pi)) {
                return pi;
            }
        }
    }
    return -1; // Exam not found
}


#endif // CROSSOVER_H
