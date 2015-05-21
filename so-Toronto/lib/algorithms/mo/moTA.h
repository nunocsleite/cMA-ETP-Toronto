#ifndef MOTA_H
#define MOTA_H

#include <algo/moLocalSearch.h>
#include "algorithms/mo/moTAexplorer.h"
#include <continuator/moTrueContinuator.h>
#include <eval/moEval.h>
#include <eoEvalFunc.h>
#include "algorithms/mo/moSimpleCoolingSchedule.h"


/**
 * Threshold Accepting algorithm
 */
template<class Neighbor>
class moTA: public moLocalSearch<Neighbor>
{
public:

    typedef typename Neighbor::EOT EOT;
    typedef moNeighborhood<Neighbor> Neighborhood ;


    /**
     * Simple constructor for a threshold accepting
     * @param _neighborhood the neighborhood
     * @param _fullEval the full evaluation function
     * @param _eval neighbor's evaluation function
     * @param _cool a cooling schedule
     */

    moTA(Neighborhood& _neighborhood, eoEvalFunc<EOT>& _fullEval, moEval<Neighbor>& _eval,
         moCoolingSchedule<EOT>& _cool):
            moLocalSearch<Neighbor>(explorer, trueCont, _fullEval),
            defaultCool(0, 0, 0, 0),
            explorer(_neighborhood, _eval, defaultSolNeighborComp, _cool)
    {}


private:
    moTrueContinuator<Neighbor> trueCont;
    moSimpleCoolingSchedule<EOT> defaultCool;
    moSolNeighborComparator<Neighbor> defaultSolNeighborComp;
    moTAexplorer<Neighbor> explorer;
};



#endif // MOTA_H
