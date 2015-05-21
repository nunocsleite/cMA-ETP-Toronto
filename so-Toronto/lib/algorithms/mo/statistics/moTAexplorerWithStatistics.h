#ifndef MOTAEXPLORER_WITH_STATISTICS_H
#define MOTAEXPLORER_WITH_STATISTICS_H


#include <explorer/moNeighborhoodExplorer.h>
#include <comparator/moSolNeighborComparator.h>
#include <coolingSchedule/moCoolingSchedule.h>
#include <neighborhood/moNeighborhood.h>

// In order to produce statistical info
//#include "neighbourhood/ETTPneighbor.h"
#include "neighbourhood/statistics/ETTPneighborWithStatistics.h"
#include "neighbourhood/ETTPneighborEval.h"
#include "chromosome/eoChromosome.h"
#include "statistics/ExamMoveStatistics.h"
#include "algorithms/mo/moTAexplorer.h"

#include "neighbourhood/ETTPneighborhood.h"


/**
 * Explorer for the Threshold Accepting algorithm
 * Fitness must be > 0
 *
 */
template <class Neighbor>
class moTAexplorerWithStatistics : public moTAexplorer<Neighbor>
{

public:

    /**
     * Constructor
     * @param _examMoveStatistics object that keeps statistical information
     * @param _neighborhood the neighborhood
     * @param _eval the evaluation function
     * @param _solNeighborComparator a solution vs neighbor comparator
     * @param _coolingSchedule the cooling schedule
     * @param _qmax the starting threshold
     */
    moTAexplorerWithStatistics(ExamMoveStatistics &_examMoveStatistics,
                typename moTAexplorer<Neighbor>::Neighborhood &_neighborhood,
                moEval<Neighbor> &_eval,
                moSolNeighborComparator<Neighbor> &_solNeighborComparator,
                moCoolingSchedule<typename moTAexplorer<Neighbor>::EOT> &_coolingSchedule)
        : moTAexplorer<Neighbor>(_neighborhood, _eval, _solNeighborComparator, _coolingSchedule),
          examMoveStatistics(_examMoveStatistics)
    {
          this->isAccept = false;

          if (!this->neighborhood.isRandom()) {
              std::cout << "moTAexplorerWithStatistics::Warning -> the neighborhood used is not random" << std::endl;
          }
      }

    /**
     * Destructor
     */
    ~moTAexplorerWithStatistics() { }



    /**
     * Explore one random solution in the neighborhood
     * @param _solution the solution
     */
    virtual void operator()(typename moTAexplorer<Neighbor>::EOT & _solution) {
        // Test if _solution has a Neighbor
        if (this->neighborhood.hasNeighbor(_solution)) {
            // Init on the first neighbor: supposed to be random solution in the neighborhood
            this->neighborhood.init(_solution, this->selectedNeighbor);
            // Eval the _solution moved with the neighbor and stock the result in the neighbor
            this->eval(_solution, this->selectedNeighbor);
        }
        else {
            // If _solution hasn't neighbor,
            this->isAccept = false;
        }
    }


    /**
     * Overriden method
     * Acceptance criterion of Threshold Accepting algorithm
     * @param _solution the solution
     * @return true if f (s′ ) − f (s) ≤ Q
     */
    virtual bool accept(typename moTAexplorer<Neighbor>::EOT & _solution) override {
//        cout << "accept method" << endl;

        if (this->neighborhood.hasNeighbor(_solution)) {
//            if (solNeighborComparator(_solution, selectedNeighbor)) { // accept if the current neighbor is better than the solution

            double e = this->selectedNeighbor.fitness() - _solution.fitness();

            if (e <= this->q) { // Minimization problem
                this->isAccept = true;

                // Static cast
                ETTPneighborWithStatistics<eoChromosome>* neigh =
                        static_cast<ETTPneighborWithStatistics<eoChromosome>* >(&this->selectedNeighbor);


//                cout << "Ei = " << neigh->movedExam.exam << ", ti = " << neigh->movedExam.sourcePeriod <<
//                        ", tj = " << neigh->movedExam.destPeriod << endl;

                examMoveStatistics.updateExamMove(neigh->getMovedExam().exam, this->q);

            }
            else {
                this->isAccept = false;
            }
        }
        return this->isAccept;
    }

private:
    // Added in order to produce statistical info
    ExamMoveStatistics &examMoveStatistics;
};


#endif // MOTAEXPLORER_WITH_STATISTICS_H
