#ifndef ETTPNEIGHBOREVALWITHSTATISTICS_H
#define ETTPNEIGHBOREVALWITHSTATISTICS_H

#include <eval/moEval.h>
#include <eoEvalFunc.h>
#include "neighbourhood/statistics/ETTPneighborWithStatistics.h"
#include "kempeChain/ETTPKempeChain.h"


/**
 * @brief Evaluation function used to evaluate the neighbour solution
 */
template <typename EOT>
class ETTPneighborEvalWithStatistics : public moEval<ETTPneighborWithStatistics<EOT> > {

public:

    /**
     * @brief operator () Eval the _solution moved with the neighbor and stock the result in the neighbor
     * @param _solution The current solution
     * @param _neighbor The neighbour solution. The neigbour doesn't contain the timetable data, it only
     *                  contains the Kempe chain information from which the neighbour solution can be built.
     */
    virtual void operator()(typename ETTPneighborWithStatistics<EOT>::EOT &_solution, ETTPneighborWithStatistics<EOT> &_neighbor) {

//        cout << endl << "ETTPneighborEval" << endl;

        //====================================================
        //
        // Incremental evaluation of the neighbour solution
        //
        //====================================================

        // Obtain the Kempe chain object
        ETTPKempeChain<EOT> const& kempeChain = _neighbor.getKempeChain();

        // Evaluate move of solution to the neighbour.
        // This envolves doing:
        //   - move, temporarily, the solution to the neighbour
        //   - record ti and tj final time slots
        //   - evaluate neighbour (incrementally) and set neighbour fitness
        //   - undo solution move
        _neighbor.evaluateMove(_solution);

     }

};



#endif // ETTPNEIGHBOREVALWITHSTATISTICS_H
