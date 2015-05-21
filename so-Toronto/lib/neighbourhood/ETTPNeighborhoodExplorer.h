#ifndef ETTPNEIGHBORHOODEXPLORER_H
#define ETTPNEIGHBORHOODEXPLORER_H


#include <explorer/moNeighborhoodExplorer.h>



/**
 * Explore the neighborhood according to the local search algorithm 
 *
 * During this exploration,
 *   the parameters are updated
 *   one neighbor is selected
 *   a comparason with the solution is made to acccept or not this new neighbor
 * 
 * The current neighbor (currentNeigbor) is the neighbor under consideration during the search (in operator()(EOT &))
 *
 * The selected neighbor (selectedNeighbor) is the neighbor selected in method operator()(EOT &). 
 * If this neighbor is accepted, then the solution is moved on this neighbor (in move(EOT &))
 *
 */
template <class Neighbor>
class ETTPNeighborhoodExplorer : public moNeighborhoodExplorer<Neighbor>
{
public:

    typedef typename Neighbor::EOT EOT;
    typedef moNeighborhood<Neighbor> Neighborhood;


    /**
     * @brief ETTPNeighborhoodExplorer
     */
    ETTPNeighborhoodExplorer()
        : moNeighborhoodExplorer<Neighbor>() { }

    /**
     * Constructor with a Neighborhood and evaluation function
     * @param _neighborhood the neighborhood
     * @param _eval the evaluation function
     */
    ETTPNeighborhoodExplorer(Neighborhood& _neighborhood, moEval<Neighbor>& _eval)
        : moNeighborhoodExplorer<Neighbor>(_neighborhood, _eval) { }


    /**
     * Overriden method
     *
     * Move a solution on the selected neighbor
     * This method can be re-defined according to the metaheuritic
     *
     * @param _solution the solution to explore
     */
    virtual void move(typename Neighbor::EOT& _solution) override {
        // Call base class move
        moNeighborhoodExplorer<Neighbor>::move(_solution);

        // We must also update # exam proximity conflicts because of incremental evaluation
        _solution.setExamProximityConflicts(this->selectedNeighbor.getProximityConflicts());

//        cout << "In [ETTPNeighborhoodExplorer] _solution.fitness() = " << _solution.fitness() << endl;

    }

};

#endif
