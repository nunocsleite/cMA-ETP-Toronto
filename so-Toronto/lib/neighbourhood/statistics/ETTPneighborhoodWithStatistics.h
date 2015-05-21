#ifndef ETTPNEIGHBORHOODWITHSTATISTICS_H
#define ETTPNEIGHBORHOODWITHSTATISTICS_H

#include <neighborhood/moNeighborhood.h>
#include <neighborhood/moNeighbor.h>
#include <utils/eoRNG.h>
#include "neighbourhood/statistics/ETTPneighborWithStatistics.h"


/// TODO
/// ETTPneighbor should
/// be a parameter ??


template <typename EOT>
class ETTPNeighborhoodWithStatistics : public moNeighborhood<ETTPneighborWithStatistics<EOT> > {

public:

    ETTPNeighborhoodWithStatistics(ETTPKempeChainHeuristic<EOT> *_kempeChainHeuristic)
        : kempeChainHeuristic(_kempeChainHeuristic), feasibleNeigh(true) { }

    /**
     * @return true if the neighborhood is random (default false)
     */
    virtual bool isRandom() {
        return true;
    }

    /**
     * Test if a solution has (again) a Neighbor
     * @param _solution the related solution
     * @return true if _solution has a Neighbor
     */
    virtual bool hasNeighbor(typename moNeighborhood<ETTPneighborWithStatistics<EOT> >::EOT & _solution) {
        // In the beginning, this variable is true in order to call randomNeighbor method
        return feasibleNeigh;
    }

    /**
     * Initialization of the neighborhood
     * @param _solution the solution to explore
     * @param _current the first neighbor
     */
    virtual void init(typename moNeighborhood<ETTPneighborWithStatistics<EOT> >::EOT & _solution,
                      ETTPneighborWithStatistics<EOT> &_current) {

        // Initialise neighbour
//        _current.setKempeChainHeuristic(this->kempeChainHeuristic);

        _current.setKempeChainHeuristic(this->kempeChainHeuristic);

        // Generate a random neighbour
        randomNeighbor(_solution, _current);

        // Get selected exam to move
        examToMove = _current.getKempeChain().getEi();
    }

    /**
     * Give the next neighbor
     * @param _solution the solution to explore
     * @param _current the next neighbor
     */
    virtual void next(typename moNeighborhood<ETTPneighborWithStatistics<EOT> >::EOT & _solution,
                      ETTPneighborWithStatistics<EOT> &_current) {
        randomNeighbor(_solution, _current);

/// SEE
///
///

    }

    /**
     * Test if there is again a neighbor
     * @param _solution the solution to explore
     * @return true if there is again a neighbor not explored
     */
    virtual bool cont(typename moNeighborhood<ETTPneighborWithStatistics<EOT> >::EOT & _solution) {
        return true;
    }

    /**
     * Return the class Name
     * @return the class name as a std::string
     */
    virtual std::string className() const override {
        return "ETTPNeighborhoodWithStatistics";
    }

    /**
     * @brief getExamToMove Returns the current exam to be moved
     * @return
     */
    int getExamToMove() const {
        return examToMove;
    }

protected:

    /**
     * @brief randomNeighbor
     * @param _solution
     * @param _neighbor
     */
    void randomNeighbor(typename moNeighborhood<ETTPneighborWithStatistics<EOT> >::EOT & _solution,
                        ETTPneighborWithStatistics<EOT> &_neighbor) {
        // Generate a random neighbour
        _neighbor.build(_solution);
        // Using the Kempe chain heuristic, we always get a feasible neighbour
        feasibleNeigh = true;
    }

    /**
     * @brief feasibleNeigh Indicates if generated neighbour is feasible or not.
     * Because we are using a Kempe chain heuristic, the produced neighbour is
     * always feasible
     */
    bool feasibleNeigh;
    /**
     * @brief kempeChainHeuristic Kempe Chain neighbourhood
     */
    ETTPKempeChainHeuristic<EOT> *kempeChainHeuristic;

    /**
     * @brief examToMove Current exam to be moved
     */
    int examToMove;
};

#endif // ETTPNEIGHBORHOODWITHSTATISTICS_H
