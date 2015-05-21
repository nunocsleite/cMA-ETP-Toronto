#ifndef ETTPNEIGHBOREVAL_H
#define ETTPNEIGHBOREVAL_H

#include <eval/moEval.h>
#include <eoEvalFunc.h>
#include "neighbourhood/ETTPneighbor.h"
#include "kempeChain/ETTPKempeChain.h"


//#define ETTPNEIGHBOREVAL_DEBUG


/**
 * @brief Evaluation function used to evaluate the neighbour solution
 */
template <typename EOT>
class ETTPneighborEval : public moEval<ETTPneighbor<EOT> > {

public:

    /**
     * @brief operator () Eval the _solution moved with the neighbor and stock the result in the neighbor
     * @param _solution The current solution
     * @param _neighbor The neighbour solution. The neigbour doesn't contain the timetable data, it only
     *                  contains the Kempe chain information from which the neighbour solution can be built.
     */
    virtual void operator()(typename ETTPneighbor<EOT>::EOT &_solution, ETTPneighbor<EOT> &_neighbor) {

//        cout << endl << "ETTPneighborEval" << endl;

        //====================================================
        //
        // Incremental evaluation of the neighbour solution
        //
        //====================================================

        // Obtain the Kempe chain object
        ETTPKempeChain<EOT> const& kempeChain = _neighbor.getKempeChain();

#ifdef ETTPNEIGHBOREVAL_DEBUG
        std::cout << "///////////////////////////////////////////////////////////////////////" << std::endl;
        std::cout << "In [ETTPneighborEval::operator() (sol,neigh)] method:" << std::endl;
        std::cout << "   -> call _neighbor.evaluateMove(_solution) method" << std::endl;
#endif

        // Evaluate move of solution to the neighbour.
        // This envolves doing:
        //   - move, temporarily, the solution to the neighbour
        //   - record ti and tj final time slots
        //   - evaluate neighbour (incrementally) and set neighbour fitness
        //   - undo solution move
        _neighbor.evaluateMove(_solution);

#ifdef ETTPNEIGHBOREVAL_DEBUG
        std::cout << "///////////////////////////////////////////////////////////////////////" << std::endl;
        std::cout << "In [ETTPneighborEval::operator() (sol,neigh)] method:" << std::endl;
        std::cout << "   -> return from _neighbor.fitness() method" << std::endl;
        std::cout << "_neighbor.fitness() = " << _neighbor.fitness() << std::endl;
        std::cout << "///////////////////////////////////////////////////////////////////////" << std::endl;
        std::cin.get();
#endif
     }

};


#endif // ETTPNEIGHBOREVAL_H
