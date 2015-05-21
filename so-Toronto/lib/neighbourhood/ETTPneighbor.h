#ifndef ETTPNEIGHBOR_H
#define ETTPNEIGHBOR_H

#include <neighborhood/moNeighbor.h>
#include "kempeChain/ETTPKempeChainHeuristic.h"
#include <eoEvalFunc.h>
#include <boost/shared_ptr.hpp>


//#define ETTPNEIGHBOR_DEBUG


/**
 * Neighbor computation for the ETTP problem (used in local search)
 */
template<class EOT, class Fitness = typename EOT::Fitness>
class ETTPneighbor: virtual public moNeighbor<EOT, typename EOT::Fitness> {

public:
    using moNeighbor<EOT, typename EOT::Fitness>::fitness;

    /**
     * Return the class Name
     * @return the class name as a std::string
     */
    virtual std::string className() const {
        return "ETTPneighbor";
    }


    /**
     * +Overriden method
     *
     * Apply move on a solution
     * @param _sol the solution to move
     */
    virtual void move(EOT& _sol) override {

//#ifdef ETTPNEIGHBOR_DEBUG
//        std::cout << "Befor moving to neighbour" << std::endl;
//        std::cout << "_sol.fitness() = " << _sol.fitness() << std::endl;
//        std::cout << "_sol.getProximityConflicts() = " << _sol.getProximityConflicts() << std::endl;
////        cin.get();
//#endif
        //
        // Move current solution _sol to this neighbour using
        // information maintained in the KempeChain structure
        //
        (*kempeChainHeuristic.get())(_sol); // Polymorphic behaviour

//#ifdef ETTPNEIGHBOR_DEBUG
//        std::cout << "After moved to neighbour" << std::endl;
//        std::cout << "_sol.fitness() = " << _sol.fitness() << std::endl;
//        std::cout << "_sol.getProximityConflicts() = " << _sol.getProximityConflicts() << std::endl;


//        std::cout << "neighbour.fitness() = " << fitness() << std::endl;

////        cin.get();
//#endif

    }


    /**
     * @brief build Build neighbour. This consists in using kempe chain heuristic
     * to create a kempe chain for a random move
     * @param _sol
     */
    virtual void build(EOT& _sol) {

#ifdef ETTPNEIGHBOR_DEBUG
    std::cout << "In ETTPneighbor::build(_sol)" << std::endl;
    std::cout << "kempeChainHeuristic.get() != nullptr ? " << (kempeChainHeuristic.get() != nullptr) << std::endl;
#endif

        (*kempeChainHeuristic.get()).build(_sol);
    }

    /**
     * @brief getKempeChain
     * @return The Kempe chain object relating the neighbor and the current solution
     */
    ETTPKempeChain<EOT> const& getKempeChain() {
        return kempeChainHeuristic->getKempeChain();
    }

    void setKempeChainHeuristic(boost::shared_ptr<ETTPKempeChainHeuristic<EOT> > const &_kempeChainHeuristic) {
        kempeChainHeuristic = _kempeChainHeuristic;

#ifdef ETTPNEIGHBOR_DEBUG
    std::cout << "In ETTPneighbor::setKempeChainHeuristic()" << std::endl;
    std::cout << "kempeChainHeuristic.get() != nullptr ? " << (kempeChainHeuristic.get() != nullptr) << std::endl;
#endif
    }


    long getProximityConflicts() const {
        return kempeChainHeuristic->getNeighborProximityConflicts();
    }

    /**
     * @brief evaluateMove Evaluate move of solution to the neighbour.
     * This envolves doing:
     *   - move, temporarily, the solution to the neighbour
     *   - record ti and tj final time slots
     *   - evaluate neighbour and set neighbour fitness
     *   - undo solution move
     * @param _sol
     */
    virtual void evaluateMove(EOT &_sol) {
        double neighFitness;
        // Evaluate move of solution to the neighbour
        kempeChainHeuristic->evaluateSolutionMove(_sol);
        // Get neighbor fitness
        neighFitness = kempeChainHeuristic->getNeighborFitness();
        // Set neighbor fitness
        fitness(neighFitness);
    }

protected:

    /**
     * @brief kempeChainHeuristic Kempe Chain neighbourhood
     */
    boost::shared_ptr<ETTPKempeChainHeuristic<EOT> > kempeChainHeuristic;
};


#endif // ETTPNEIGHBOR_H















