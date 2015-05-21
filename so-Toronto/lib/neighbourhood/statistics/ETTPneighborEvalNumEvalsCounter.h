#ifndef ETTPNEIGHBOREVALWITHSTATISTICS_H
#define ETTPNEIGHBOREVALWITHSTATISTICS_H

#include "neighbourhood/ETTPneighborEval.h"
#include "eval/eoNumberEvalsCounter.h"


//#define ETTPNEIGHBOREVALWITHSTATISTICS_DEBUG


/**
 * @brief Evaluation function used to evaluate the neighbour solution
 */
template <typename EOT>
class ETTPneighborEvalNumEvalsCounter : public ETTPneighborEval<EOT> {

public:

    ETTPneighborEvalNumEvalsCounter(eoNumberEvalsCounter &_numberEvalsCounter)
        : numberEvalsCounter(_numberEvalsCounter) { }

    /**
     * @brief operator () Eval the _solution moved with the neighbor and stock the result in the neighbor
     * @param _solution The current solution
     * @param _neighbor The neighbour solution. The neigbour doesn't contain the timetable data, it only
     *                  contains the Kempe chain information from which the neighbour solution can be built.
     */
    virtual void operator()(typename ETTPneighbor<EOT>::EOT &_solution, ETTPneighbor<EOT> &_neighbor) {
        // Invoke base class method to perform neighbour (incremental) evaluation
        ETTPneighborEval<EOT>::operator ()(_solution, _neighbor);
        // # evals statistics computation. Add 1 to # evals
        numberEvalsCounter.addNumEvalsToGenerationTotal(1);
     }


protected:
    /**
     * @brief numberEvalsCounter
     */
    eoNumberEvalsCounter &numberEvalsCounter;

};


#endif // ETTPNEIGHBOREVAL_H
