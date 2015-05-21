#ifndef ETTPKEMPECHAINHEURISTICWITHSTATISTICS_H
#define ETTPKEMPECHAINHEURISTICWITHSTATISTICS_H

#include "eoFunctor.h"
#include "chromosome/eoChromosome.h"

#include "kempeChain/ETTPKempeChainHeuristic.h"
#include <boost/unordered_set.hpp>
#include <utils/eoRNG.h>

#include "statistics/MovedExam.h"

//#define ETTPKEMPECHAINHEURISTICWITHSTATISTICS_DEBUG




//////////////////////////////////////////////////////////
//
// (rf. Demeester paper)
// Concerning the uncapacitated Toronto examination time-
// tabling problem, we apply the Kempe chain based heuris-
// tics. These low-level heuristics perturb feasible solutions
// to the uncapacitated examination timetabling problem,
// without making them infeasible. Suppose a partial solu-
// tion that corresponds to the left hand side of Fig. 1. If
// we want to move exam e1 to time slot tj, the solution
// becomes infeasible, since exam e1 has students in com-
// mon with exams e6, e7, and e8 that are assigned to time
// slot tj. To overcome this, exams e6, e7, and e8 should be
// moved to time slot ti. This process is repeated until all the
// exams that have students in common are assigned to dif-
// ferent time slots. The result is depicted at the right hand
// side of Fig. 1. The Kempe Chain heuristic Uncap1 can be
// seen as moving an exam to another time slot whilst main-
// taining feasibility by repairing any infeasibilities that may
// have been introduced.
//
template <typename EOT>
class ETTPKempeChainHeuristicWithStatistics : public ETTPKempeChainHeuristic<EOT> {

public:

    // Ctor
    ETTPKempeChainHeuristicWithStatistics() : ETTPKempeChainHeuristic<EOT>() {

#ifdef ETTPKEMPECHAINHEURISTICWITHSTATISTICS_DEBUG
    std::cout << "In ETTPKempeChainHeuristicWithStatistics<EOT>::ctor()" << std::endl;
#endif

    }

    // Overriden method
    virtual void operator()(EOT& _chrom) override;

    MovedExam const& getMoveExamInfo() { return movedExam; }

private:
    MovedExam movedExam;

};




///
/// Overriden method
///
template <typename EOT>
void ETTPKempeChainHeuristicWithStatistics<EOT>::operator()(EOT & _chrom) {


#ifdef ETTPKEMPECHAINHEURISTICWITHSTATISTICS_DEBUG
    std::cout << "In ETTPKempeChainHeuristicWithStatistics<EOT>::operator()" << std::endl;
#endif

    // Invoke base class method
    ETTPKempeChainHeuristic<EOT>::operator()(_chrom);

    ///////////////////////////////////////////////////////
    // Statistics
    //
    // Update information about moved exam
    //
    movedExam.exam = this->kempeChain.getEi();
    movedExam.sourcePeriod = this->kempeChain.getTi();
    movedExam.destPeriod = this->kempeChain.getTj();
    ///////////////////////////////////////////////////////

}



#endif // ETTPKEMPECHAINHEURISTICWITHSTATISTICS_H
