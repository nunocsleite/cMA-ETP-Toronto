#ifndef ETTPNEIGHBORHOOD_H
#define ETTPNEIGHBORHOOD_H

#include <neighborhood/moNeighborhood.h>
#include <neighborhood/moNeighbor.h>
#include "neighbourhood/ETTPneighbor.h"
#include <utils/eoRNG.h>


//#define ETTPNEIGHBORHOOD_DEBUG


template <typename EOT>
class ETTPneighborhood : public moNeighborhood<ETTPneighbor<EOT> > {

public:

    ETTPneighborhood(boost::shared_ptr<ETTPKempeChainHeuristic<EOT> > const &_kempeChainHeuristic)
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
    virtual bool hasNeighbor(typename moNeighborhood<ETTPneighbor<EOT> >::EOT & _solution) {
        // In the beginning, this variable is true in order to call randomNeighbor method
        return feasibleNeigh;
    }

    /**
     * Initialization of the neighborhood
     * @param _solution the solution to explore
     * @param _current the first neighbor
     */
    virtual void init(typename moNeighborhood<ETTPneighbor<EOT> >::EOT & _solution, ETTPneighbor<EOT> &_current) {

#ifdef ETTPNEIGHBORHOOD_DEBUG
        std::cout << "In [ETTPneighborhood::init()] method:" << std::endl;
#endif

        // Initialise neighbour
        _current.setKempeChainHeuristic(this->kempeChainHeuristic);

        // Generate a random neighbour
        randomNeighbor(_solution, _current);

#ifdef ETTPNEIGHBORHOOD_DEBUG
//        std::cout << "In [ETTPneighborhood::init()] method:" << std::endl;
//        std::cout << "Validate solution" << std::endl;
//        _solution.validate();
#endif


    }


    /**
     * Give the next neighbor
     * @param _solution the solution to explore
     * @param _current the next neighbor
     */
    virtual void next(typename moNeighborhood<ETTPneighbor<EOT> >::EOT & _solution, ETTPneighbor<EOT> &_current) {
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
    virtual bool cont(typename moNeighborhood<ETTPneighbor<EOT> >::EOT & _solution) {
        return true;
    }

    /**
     * Return the class Name
     * @return the class name as a std::string
     */
    virtual std::string className() const {
        return "ETTPNeighborhood";
    }

protected:

    /**
     * @brief randomNeighbor
     * @param _solution
     * @param _neighbor
     */
    void randomNeighbor(typename moNeighborhood<ETTPneighbor<EOT> >::EOT & _solution, ETTPneighbor<EOT> &_neighbor) {
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
    boost::shared_ptr<ETTPKempeChainHeuristic<EOT> > const &kempeChainHeuristic;
};


#endif // ETTPNEIGHBORHOOD_H


