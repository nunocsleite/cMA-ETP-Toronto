#ifndef ETTPNEIGHBORWITHSTATISTICS_H
#define ETTPNEIGHBORWITHSTATISTICS_H


#include "neighbourhood/ETTPneighbor.h"
#include "statistics/MovedExam.h"

#include "kempeChain/statistics/ETTPKempeChainHeuristicWithStatistics.h"



/**
 * Neighbor computation for the ETTP problem (used in local search)
 *
 * This neighbor computes statistics about moved exams
 *
 */
template<class EOT, class Fitness = typename EOT::Fitness>
class ETTPneighborWithStatistics : virtual public moNeighbor<EOT, typename EOT::Fitness> {

public:
    using moNeighbor<EOT, typename EOT::Fitness>::fitness;

    /**
     * Return the class Name
     * @return the class name as a std::string
     */
    virtual std::string className() const override {
        return "ETTPneighborWithStatistics";
    }


    /**
     * +Overriden method
     *
     * Apply move on a solution
     * @param _sol the solution to move
     */
    virtual void move(EOT& _sol) override {
        //
        // Move current solution _sol to this neighbour using
        // information maintained in the KempeChain structure
        //

//        (*kempeChainHeuristic.get())(_sol); // Polymorphic behaviour

        kempeChainHeuristic->operator ()(_sol); // Polymorphic behaviour

    }


    /**
     * @brief build Build neighbour. This consists in using kempe chain heuristic
     * to create a kempe chain for a random move
     * @param _sol
     */
    virtual void build(EOT& _sol) {

//        (*kempeChainHeuristic.get()).build(_sol);
        kempeChainHeuristic->build(_sol);
    }

    /**
     * @brief getKempeChain
     * @return The Kempe chain object relating the neighbor and the current solution
     */
    ETTPKempeChain<EOT> const& getKempeChain() {
        return kempeChainHeuristic->getKempeChain();
    }


//    void setKempeChainHeuristic(boost::shared_ptr<ETTPKempeChainHeuristic<EOT> > const &_kempeChainHeuristic) {
//        kempeChainHeuristic = _kempeChainHeuristic;
//    }

    void setKempeChainHeuristic(ETTPKempeChainHeuristic<EOT> *_kempeChainHeuristic) {
        kempeChainHeuristic = _kempeChainHeuristic;
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
        ////////////////////////////////////
        // Compute statistical info about move exams
        //
        // Get moved exam
        //
        ////////////////////////////////////

        //        movedExam = this->kempeChainHeuristic->getMoveExamInfo();

        ETTPKempeChainHeuristicWithStatistics<EOT>* kempeChainHeuristicWithStat =
            static_cast<ETTPKempeChainHeuristicWithStatistics<EOT>* >(kempeChainHeuristic);

        if (kempeChainHeuristicWithStat == nullptr) {
            throw runtime_error("kempeChainHeuristicWithStat == nullptr");
        }

        movedExam = kempeChainHeuristicWithStat->getMoveExamInfo();
    }

    /**
     * @brief getMovedExam
     * @return Returns the MovedExam object
     */
    MovedExam getMovedExam() const
    {
        return movedExam;
    }


protected:

    /**
     * @brief kempeChainHeuristic Kempe Chain neighbourhood
     */
//    boost::shared_ptr<ETTPKempeChainHeuristic<EOT> > kempeChainHeuristic;
    ETTPKempeChainHeuristic<EOT> * kempeChainHeuristic;

    /**
     * @brief movedExam The moved exam
     */
    MovedExam movedExam;
};




#endif // ETTPNEIGHBORWITHSTATISTICS_H

